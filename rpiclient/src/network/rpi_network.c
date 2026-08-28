/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network.c
 * Copyright (C) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>
 *
 * rpiclient-gtk is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rpiclient-gtk is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "rpi_network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct
{
    gint channel_id;
    gint value;
} StatusDispatchData;

static gint socket_fd = -1;
static GThread *rx_thread = NULL;
static GMutex net_mutex;
static volatile gboolean is_running = FALSE;
static gint request_id = 0;
static RPINetworkStatusCallback global_status_callback = NULL;
static gpointer global_status_callback_data = NULL;

static gpointer rx_worker_thread(gpointer data);
static gboolean on_dispatch_status_idle(gpointer user_data);
static void parse_and_dispatch_response(const gchar *json_line);

void rpi_network_set_status_callback(RPINetworkStatusCallback callback, gpointer user_data)
{
    g_mutex_lock(&net_mutex);
    global_status_callback = callback;
    global_status_callback_data = user_data;
    g_mutex_unlock(&net_mutex);
}

gboolean rpi_network_is_connected(void)
{
    gboolean connected;
    g_mutex_lock(&net_mutex);
    connected = (socket_fd >= 0 && is_running);
    g_mutex_unlock(&net_mutex);
    return connected;
}

gboolean rpi_network_connect(const gchar *host, gint port)
{
    if (rpi_network_is_connected())
    {
        rpi_network_disconnect();
    }

    g_mutex_lock(&net_mutex);

    struct hostent *server = gethostbyname(host ? host : "127.0.0.1");
    if (!server)
    {
        g_warning("Network: Unable to resolve hostname '%s'", host ? host : "127.0.0.1");
        g_mutex_unlock(&net_mutex);
        return FALSE;
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        g_warning("Network: Failed to create socket.");
        g_mutex_unlock(&net_mutex);
        return FALSE;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], (size_t)server->h_length);
    serv_addr.sin_port = htons((uint16_t)port);

    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        g_warning("Network: Failed to connect to %s:%d", host ? host : "127.0.0.1", port);
        close(socket_fd);
        socket_fd = -1;
        g_mutex_unlock(&net_mutex);
        return FALSE;
    }

    is_running = TRUE;
    rx_thread = g_thread_new("rpi_net_rx", rx_worker_thread, NULL);

    g_message("Network: Successfully connected to %s:%d", host ? host : "127.0.0.1", port);
    g_mutex_unlock(&net_mutex);

    // Send initial keep-alive handshake
    rpi_network_send_keepalive();

    return TRUE;
}

void rpi_network_disconnect(void)
{
    g_mutex_lock(&net_mutex);
    if (socket_fd >= 0)
    {
        is_running = FALSE;
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        socket_fd = -1;
    }
    g_mutex_unlock(&net_mutex);

    if (rx_thread)
    {
        g_thread_join(rx_thread);
        rx_thread = NULL;
    }

    g_message("Network: Disconnected.");
}

static gboolean send_raw_json(const gchar *json_str)
{
    g_mutex_lock(&net_mutex);
    if (socket_fd < 0 || !is_running)
    {
        g_mutex_unlock(&net_mutex);
        return FALSE;
    }

    size_t len = strlen(json_str);
    ssize_t sent = write(socket_fd, json_str, len);
    g_mutex_unlock(&net_mutex);

    return (sent == (ssize_t)len);
}

gboolean rpi_network_send_channel(gint channel_id, gint value)
{
    if (!rpi_network_is_connected())
    {
        return FALSE;
    }

    gchar buffer[256] = {0};
    g_mutex_lock(&net_mutex);
    request_id++;
    gint cur_id = request_id;
    g_mutex_unlock(&net_mutex);

    g_snprintf(
        buffer,
        sizeof(buffer),
        "{\"jsonrpc\": \"2.0\", \"method\": \"rpi_cmd\", \"params\": {\"action\": \"SET_CHANNEL\", \"channel\": %d, \"value\": %d}, \"id\": %d}\n",
        channel_id,
        value,
        cur_id
    );

    return send_raw_json(buffer);
}

gboolean rpi_network_send_keepalive(void)
{
    if (!rpi_network_is_connected())
    {
        return FALSE;
    }

    gchar buffer[128] = {0};
    g_mutex_lock(&net_mutex);
    request_id++;
    gint cur_id = request_id;
    g_mutex_unlock(&net_mutex);

    g_snprintf(
        buffer,
        sizeof(buffer),
        "{\"jsonrpc\": \"2.0\", \"method\": \"rpi_cmd\", \"params\": {\"action\": \"REQ\"}, \"id\": %d}\n",
        cur_id
    );

    return send_raw_json(buffer);
}

static gboolean on_dispatch_status_idle(gpointer user_data)
{
    StatusDispatchData *data = (StatusDispatchData *)user_data;
    if (data)
    {
        if (global_status_callback)
        {
            global_status_callback(data->channel_id, data->value, global_status_callback_data);
        }
        g_free(data);
    }
    return G_SOURCE_REMOVE;
}

static void parse_and_dispatch_response(const gchar *json_line)
{
    if (!json_line || strlen(json_line) == 0)
    {
        return;
    }

    // Look for status response: {"jsonrpc": "2.0", "result": {"status": "ACK", "channel": X, "value": Y}, ...}
    const gchar *channel_pos = g_strstr_len(json_line, -1, "\"channel\":");
    const gchar *value_pos = g_strstr_len(json_line, -1, "\"value\":");

    if (channel_pos && value_pos)
    {
        gint ch = 0;
        gint val = 0;

        if (sscanf(channel_pos, "\"channel\": %d", &ch) == 1 || sscanf(channel_pos, "\"channel\":%d", &ch) == 1)
        {
            if (sscanf(value_pos, "\"value\": %d", &val) == 1 || sscanf(value_pos, "\"value\":%d", &val) == 1)
            {
                StatusDispatchData *data = g_malloc0(sizeof(StatusDispatchData));
                if (data)
                {
                    data->channel_id = ch;
                    data->value = val;
                    g_idle_add(on_dispatch_status_idle, data);
                }
            }
        }
    }
}

static gpointer rx_worker_thread(gpointer data)
{
    (void)data;
    gchar rx_buffer[1024] = {0};
    gchar line_buffer[1024] = {0};
    size_t line_len = 0;

    while (is_running)
    {
        ssize_t bytes_read = read(socket_fd, rx_buffer, sizeof(rx_buffer) - 1);
        if (bytes_read <= 0)
        {
            if (is_running)
            {
                g_message("Network: Connection closed by remote server.");
            }
            break;
        }

        rx_buffer[bytes_read] = '\0';

        for (ssize_t i = 0; i < bytes_read; i++)
        {
            gchar c = rx_buffer[i];
            if (c == '\n')
            {
                line_buffer[line_len] = '\0';
                parse_and_dispatch_response(line_buffer);
                line_len = 0;
            }
            else if (line_len < sizeof(line_buffer) - 1)
            {
                line_buffer[line_len++] = c;
            }
        }
    }

    g_mutex_lock(&net_mutex);
    if (socket_fd >= 0)
    {
        close(socket_fd);
        socket_fd = -1;
    }
    is_running = FALSE;
    g_mutex_unlock(&net_mutex);

    return NULL;
}
