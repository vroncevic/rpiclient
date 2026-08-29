/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network_sender.c
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
#include "rpi_network_sender.h"
#include "../client/rpi_network_client.h"

#include <stdio.h>
#include <string.h>

static const gsize NET_CMD_BUFFER_SIZE = 256;
static const gsize NET_KEEPALIVE_BUFFER_SIZE = 128;

gboolean rpi_network_send_channel(gint channel_id, gint value) {
  if (!rpi_network_client_is_connected()) {
    return FALSE;
  }

  gchar buffer[NET_CMD_BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));

  gint cur_id = rpi_network_client_next_id();

  g_snprintf(buffer, sizeof(buffer),
             "{\"jsonrpc\": \"2.0\", \"method\": \"rpi_cmd\", \"params\": "
             "{\"action\": \"SET_CHANNEL\", \"channel\": %d, \"value\": %d}, "
             "\"id\": %d}\n",
             channel_id, value, cur_id);

  return rpi_network_client_send_raw(buffer);
}

gboolean rpi_network_send_keepalive(void) {
  if (!rpi_network_client_is_connected()) {
    return FALSE;
  }

  gchar buffer[NET_KEEPALIVE_BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));

  gint cur_id = rpi_network_client_next_id();

  g_snprintf(buffer, sizeof(buffer),
             "{\"jsonrpc\": \"2.0\", \"method\": \"rpi_cmd\", \"params\": "
             "{\"action\": \"REQ\"}, \"id\": %d}\n",
             cur_id);

  return rpi_network_client_send_raw(buffer);
}
