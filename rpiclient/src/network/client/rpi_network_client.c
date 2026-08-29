/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network_client.c
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
#include "rpi_network_client.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static const gchar *const DEFAULT_NET_HOST = "127.0.0.1";

static gint socket_fd = -1;
static GMutex net_mutex;
static volatile gboolean is_running = FALSE;
static gint request_id = 0;

void rpi_network_client_lock(void) { g_mutex_lock(&net_mutex); }

void rpi_network_client_unlock(void) { g_mutex_unlock(&net_mutex); }

gint rpi_network_client_get_fd(void) { return socket_fd; }

void rpi_network_client_set_fd(gint fd) { socket_fd = fd; }

gboolean rpi_network_client_is_running(void) { return is_running; }

void rpi_network_client_set_running(gboolean running) { is_running = running; }

gint rpi_network_client_next_id(void) {
  g_mutex_lock(&net_mutex);
  request_id++;
  gint id = request_id;
  g_mutex_unlock(&net_mutex);
  return id;
}

gboolean rpi_network_client_is_connected(void) {
  gboolean connected;
  g_mutex_lock(&net_mutex);
  connected = (socket_fd >= 0 && is_running);
  g_mutex_unlock(&net_mutex);
  return connected;
}

gboolean rpi_network_client_connect(const gchar *host, gint port) {
  g_mutex_lock(&net_mutex);

  const gchar *target_host = host ? host : DEFAULT_NET_HOST;
  struct hostent *server = gethostbyname(target_host);
  if (!server) {
    g_warning("Network: Unable to resolve hostname '%s'", target_host);
    g_mutex_unlock(&net_mutex);
    return FALSE;
  }

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    g_warning("Network: Failed to create socket.");
    g_mutex_unlock(&net_mutex);
    return FALSE;
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0],
         (size_t)server->h_length);
  serv_addr.sin_port = htons((uint16_t)port);

  if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    g_warning("Network: Failed to connect to %s:%d", target_host, port);
    close(socket_fd);
    socket_fd = -1;
    g_mutex_unlock(&net_mutex);
    return FALSE;
  }

  is_running = TRUE;
  g_message("Network: Successfully connected to %s:%d", target_host, port);
  g_mutex_unlock(&net_mutex);

  return TRUE;
}

void rpi_network_client_disconnect(void) {
  g_mutex_lock(&net_mutex);
  if (socket_fd >= 0) {
    is_running = FALSE;
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
    socket_fd = -1;
  }
  g_mutex_unlock(&net_mutex);
}

gboolean rpi_network_client_send_raw(const gchar *data) {
  g_mutex_lock(&net_mutex);
  if (socket_fd < 0 || !is_running) {
    g_mutex_unlock(&net_mutex);
    return FALSE;
  }

  size_t len = strlen(data);
  ssize_t sent = write(socket_fd, data, len);
  g_mutex_unlock(&net_mutex);

  return (sent == (ssize_t)len);
}
