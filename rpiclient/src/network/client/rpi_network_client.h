/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network_client.h
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

#include <glib.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Establishes TCP connection to the RPI server
/// @param host IP address or hostname of the server
/// @param port Port number on the server
/// @return TRUE if connection succeeded, FALSE otherwise
gboolean rpi_network_client_connect(const gchar *host, gint port);

//////////////////////////////////////////////////////////////////////////////
/// @brief Disconnects and closes the TCP socket
void rpi_network_client_disconnect(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Checks if client is currently connected
/// @return TRUE if connected, FALSE otherwise
gboolean rpi_network_client_is_connected(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sends raw string data over the socket
/// @param data String payload to send
/// @return TRUE if sending succeeded, FALSE otherwise
gboolean rpi_network_client_send_raw(const gchar *data);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets underlying socket file descriptor
/// @return Socket file descriptor integer
gint rpi_network_client_get_fd(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets underlying socket file descriptor
/// @param fd Socket file descriptor integer
void rpi_network_client_set_fd(gint fd);

//////////////////////////////////////////////////////////////////////////////
/// @brief Checks if client receiver loop is currently running
/// @return TRUE if running, FALSE otherwise
gboolean rpi_network_client_is_running(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets running state flag for client receiver loop
/// @param running Boolean flag indicating running state
void rpi_network_client_set_running(gboolean running);

//////////////////////////////////////////////////////////////////////////////
/// @brief Locks client I/O mutex
void rpi_network_client_lock(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Unlocks client I/O mutex
void rpi_network_client_unlock(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Generates next sequential JSON-RPC request identifier
/// @return Next request ID integer
gint rpi_network_client_next_id(void);

G_END_DECLS
