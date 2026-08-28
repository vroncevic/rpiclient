/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network.h
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
#pragma once

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Callback prototype for status update notifications
/// @param channel_id Channel identifier number (1-8)
/// @param value Status value (0-100)
/// @param user_data User data passed when registering callback
typedef void (*RPINetworkStatusCallback)(gint channel_id, gint value, gpointer user_data);

//////////////////////////////////////////////////////////////////////////////
/// @brief Connects to RPI TCP Server
/// @param host Server hostname or IP address string
/// @param port Server port number
/// @return TRUE on success, FALSE on failure
gboolean rpi_network_connect(const gchar *host, gint port);

//////////////////////////////////////////////////////////////////////////////
/// @brief Disconnects from RPI TCP Server
void rpi_network_disconnect(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Checks if network client is currently connected
/// @return TRUE if connected, FALSE otherwise
gboolean rpi_network_is_connected(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sends channel value control command (SET_CHANNEL) via JSON-RPC 2.0
/// @param channel_id Target channel ID (1-8)
/// @param value Channel set value (0-100)
/// @return TRUE if sent successfully, FALSE otherwise
gboolean rpi_network_send_channel(gint channel_id, gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sends keep-alive request (REQ) via JSON-RPC 2.0
/// @return TRUE if sent successfully, FALSE otherwise
gboolean rpi_network_send_keepalive(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets callback to receive status updates on main GTK loop
/// @param callback Function pointer
/// @param user_data Pointer to pass to callback
void rpi_network_set_status_callback(RPINetworkStatusCallback callback, gpointer user_data);

G_END_DECLS
