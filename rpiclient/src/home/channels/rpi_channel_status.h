/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_channel_status.h
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

#include "../../widgets/rpi_vertical_bar.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Channel status complex widget
typedef struct _RPIChannelStatus RPIChannelStatus;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget channel status
/// @param channel_id Channel identifier number (1-8)
/// @return RPI channel status complex widget pointer | NULL
RPIChannelStatus *new_rpi_channel_status(gint channel_id);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show RPI channel status complex widget
/// @param instance is pointer to complex widget channel status
void show_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide RPI channel status complex widget
/// @param instance is pointer to complex widget channel status
void hide_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame container from RPI channel status complex widget
/// @param instance is pointer to complex widget channel status
/// @return Frame container from RPI channel status complex widget | NULL
GtkWidget *get_frame_from_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets status channel telemetry value (updates vertical bar & label)
/// @param instance is pointer to complex widget channel status
/// @param value Value between 0 and 100
void rpi_channel_status_set_value(RPIChannelStatus *instance, gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets current status channel telemetry value
/// @param instance is pointer to complex widget channel status
/// @return Value between 0 and 100
gint rpi_channel_status_get_value(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy RPI channel status complex widget
/// @param instance is pointer to complex widget channel status
void destroy_rpi_channel_status(RPIChannelStatus *instance);

G_END_DECLS
