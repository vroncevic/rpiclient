/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network_receiver.c
 * Copyright (C) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>
 *
 * rpiclient is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rpiclient is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "rpi_network_receiver.h"
#include "../client/rpi_network_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const gsize NET_RX_BUFFER_SIZE = 1024;

typedef struct {
  gint channel_id;
  gint value;
} StatusDispatchData;

static GThread *rx_thread = NULL;
static RPINetworkStatusCallback global_status_callback = NULL;
static gpointer global_status_callback_data = NULL;

static gpointer rx_worker_thread(gpointer data);
static gboolean on_dispatch_status_idle(gpointer user_data);
static void parse_and_dispatch_response(const gchar *json_line);

void rpi_network_set_status_callback(RPINetworkStatusCallback callback,
                                     gpointer user_data) {
  rpi_network_client_lock();
  global_status_callback = callback;
  global_status_callback_data = user_data;
  rpi_network_client_unlock();
}

void rpi_network_receiver_start(void) {
  if (!rx_thread) {
    rx_thread = g_thread_new("rpi_net_rx", rx_worker_thread, NULL);
  }
}

void rpi_network_receiver_stop(void) {
  if (rx_thread) {
    g_thread_join(rx_thread);
    rx_thread = NULL;
  }
}

static gboolean on_dispatch_status_idle(gpointer user_data) {
  StatusDispatchData *data = (StatusDispatchData *)user_data;
  if (data) {
    if (global_status_callback) {
      global_status_callback(data->channel_id, data->value,
                             global_status_callback_data);
    }
    g_free(data);
  }
  return G_SOURCE_REMOVE;
}

static void parse_and_dispatch_response(const gchar *json_line) {
  if (!json_line || strlen(json_line) == 0) {
    return;
  }

  const gchar *channel_pos = g_strstr_len(json_line, -1, "\"channel\":");
  const gchar *value_pos = g_strstr_len(json_line, -1, "\"value\":");

  if (channel_pos && value_pos) {
    gint ch = 0;
    gint val = 0;

    if (sscanf(channel_pos, "\"channel\": %d", &ch) == 1 ||
        sscanf(channel_pos, "\"channel\":%d", &ch) == 1) {
      if (sscanf(value_pos, "\"value\": %d", &val) == 1 ||
          sscanf(value_pos, "\"value\":%d", &val) == 1) {
        StatusDispatchData *data = g_malloc0(sizeof(StatusDispatchData));
        if (data) {
          data->channel_id = ch;
          data->value = val;
          g_idle_add(on_dispatch_status_idle, data);
        }
      }
    }
  }
}

static gpointer rx_worker_thread(gpointer data) {
  (void)data;
  gchar rx_buffer[NET_RX_BUFFER_SIZE];
  gchar line_buffer[NET_RX_BUFFER_SIZE];
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(line_buffer, 0, sizeof(line_buffer));
  size_t line_len = 0;

  while (rpi_network_client_is_running()) {
    gint fd = rpi_network_client_get_fd();
    if (fd < 0) {
      break;
    }

    ssize_t bytes_read = read(fd, rx_buffer, sizeof(rx_buffer) - 1);
    if (bytes_read <= 0) {
      if (rpi_network_client_is_running()) {
        g_message("Network: Connection closed by remote server.");
      }
      break;
    }

    rx_buffer[bytes_read] = '\0';

    for (ssize_t i = 0; i < bytes_read; i++) {
      gchar c = rx_buffer[i];
      if (c == '\n') {
        line_buffer[line_len] = '\0';
        parse_and_dispatch_response(line_buffer);
        line_len = 0;
      } else if (line_len < sizeof(line_buffer) - 1) {
        line_buffer[line_len++] = c;
      }
    }
  }

  rpi_network_client_lock();
  gint fd = rpi_network_client_get_fd();
  if (fd >= 0) {
    close(fd);
    rpi_network_client_set_fd(-1);
  }
  rpi_network_client_set_running(FALSE);
  rpi_network_client_unlock();

  return NULL;
}
