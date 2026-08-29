/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_network_receiver.h
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
/// @brief Callback prototype for incoming channel telemetry notifications
/// @param channel_id Channel identifier (1-8)
/// @param value Telemetry value (0-100)
/// @param user_data Application context pointer passed during registration
typedef void (*RPINetworkStatusCallback)(gint channel_id, gint value,
                                         gpointer user_data);

//////////////////////////////////////////////////////////////////////////////
/// @brief Spawns background worker thread to receive incoming JSON responses
void rpi_network_receiver_start(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Stops and joins background receiver worker thread
void rpi_network_receiver_stop(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Registers callback handler for incoming telemetry updates
/// @param callback Function pointer to invoke on telemetry events
/// @param user_data Application context pointer to deliver with callback
void rpi_network_set_status_callback(RPINetworkStatusCallback callback,
                                     gpointer user_data);

G_END_DECLS
