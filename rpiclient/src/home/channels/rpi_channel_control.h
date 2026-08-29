/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_channel_control.h
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
/// @brief Channel control complex widget
typedef struct _RPIChannelControl RPIChannelControl;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget channel control
/// @param channel_id Channel identifier number (1-8)
/// @return RPI channel control complex widget pointer | NULL
RPIChannelControl *new_rpi_channel_control(gint channel_id);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show RPI channel control complex widget
/// @param instance is pointer to complex widget channel control
void show_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide RPI channel control complex widget
/// @param instance is pointer to complex widget channel control
void hide_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame container from RPI channel control complex widget
/// @param instance is pointer to complex widget channel control
/// @return Frame container from RPI channel control complex widget | NULL
GtkWidget *get_frame_from_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy RPI channel control complex widget
/// @param instance is pointer to complex widget channel control
void destroy_rpi_channel_control(RPIChannelControl *instance);

G_END_DECLS
