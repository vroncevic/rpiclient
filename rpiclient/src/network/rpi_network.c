/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network.c
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
#include "rpi_network.h"
#include "client/rpi_network_client.h"

gboolean rpi_network_is_connected(void) {
  return rpi_network_client_is_connected();
}

gboolean rpi_network_connect(const gchar *host, gint port) {
  if (rpi_network_is_connected()) {
    rpi_network_disconnect();
  }

  gboolean ok = rpi_network_client_connect(host, port);
  if (ok) {
    rpi_network_receiver_start();
    rpi_network_send_keepalive();
  }
  return ok;
}

void rpi_network_disconnect(void) {
  rpi_network_client_disconnect();
  rpi_network_receiver_stop();
  g_message("Network: Disconnected.");
}
