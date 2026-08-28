/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_channel_status.h
 * Copyright (C) 2016 - 2025 Vladimir Roncevic <elektron.ronca@gmail.com>
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

#include <gtk/gtk.h>
#include "../widgets/rpi_vertical_bar.h"

//////////////////////////////////////////////////////////////////////////////
/// @brief Channel status complex widget
typedef struct _RPIChannelStatus RPIChannelStatus;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget channel status
/// @return Channel status complex widget pointer | NULL
RPIChannelStatus *new_rpi_channel_status(gint channel_id);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show channel status complex widget
/// @param instance is pointer to complex widget channel status
void show_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide channel status complex widget
/// @param instance is pointer to complex widget channel status
void hide_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets check box from channel status complex widget
/// @param instance is pointer to complex widget channel status
/// @return Check box from complex widget channel status | NULL
GtkWidget* get_check_box_from_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets label from channel status complex widget
/// @param instance is pointer to complex widget channel status
/// @return Label from complex widget channel status | NULL
GtkWidget* get_label_from_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets vertical bar from channel status complex widget
/// @param instance is pointer to complex widget channel status
/// @return Bar from complex widget channel status | NULL
GtkVB* get_vertical_bar_from_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame from channel status complex widget
/// @param instance is pointer to complex widget channel status
/// @return Frame from complex widget channel status | NULL
GtkWidget* get_frame_from_rpi_channel_status(RPIChannelStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets value for channel status complex widget
/// @param instance is pointer to complex widget channel status
/// @param value is status percentage value (0-100)
void set_value_to_rpi_channel_status(RPIChannelStatus *instance, gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy channel status complex widget
/// @param instance is pointer to complex widget channel status
void destroy_rpi_channel_status(RPIChannelStatus *instance);
