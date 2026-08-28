/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame.h
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

#define MAX_CHANNELS_CONTROL_HOME_FRAME 8
#define MAX_CHANNELS_STATUS_HOME_FRAME 8

//////////////////////////////////////////////////////////////////////////////
/// @brief RPIHome frame complex widget
typedef struct _RPIRPIHomeFrame RPIHomeFrame;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget home frame
/// @return RPIHome frame complex widget pointer | NULL
RPIHomeFrame *new_rpi_home_frame(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show home frame complex widget
/// @param instance is pointer to complex widget home frame
void show_rpi_home_frame(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide home frame complex widget
/// @param instance is pointer to complex widget home frame
void hide_rpi_home_frame(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame from home frame complex widget
/// @param instance is pointer to complex widget home frame
/// @return Frame from complex widget home frame | NULL
GtkWidget* get_frame_from_rpi_home_frame(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Updates status for a channel
/// @param instance is pointer to complex widget home frame
/// @param channel_id is channel number (1-8 or 0-7)
/// @param value is status percentage value (0-100)
void rpi_home_frame_update_channel_status(RPIHomeFrame *instance, gint channel_id, gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy home frame complex widget
/// @param instance is pointer to complex widget home frame
void destroy_rpi_home_frame(RPIHomeFrame *instance);
