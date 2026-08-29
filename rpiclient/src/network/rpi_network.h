/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network.h
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
#pragma once

#include "receiver/rpi_network_receiver.h"
#include "sender/rpi_network_sender.h"
#include <glib.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Connects to remote RPI server and starts background receiver thread
/// @param host IP address or hostname of the server
/// @param port Port number on the server
/// @return TRUE if connection and thread startup succeeded, FALSE otherwise
gboolean rpi_network_connect(const gchar *host, gint port);

//////////////////////////////////////////////////////////////////////////////
/// @brief Stops background receiver thread and disconnects TCP connection
void rpi_network_disconnect(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Checks if client is currently connected to server
/// @return TRUE if connected, FALSE otherwise
gboolean rpi_network_is_connected(void);

G_END_DECLS
