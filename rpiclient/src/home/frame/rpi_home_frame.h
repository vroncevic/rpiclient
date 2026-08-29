/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame.h
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

#include "control/rpi_home_frame_control.h"
#include "status/rpi_home_frame_status.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Home frame complex widget
typedef struct _RPIHomeFrame RPIHomeFrame;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget home frame
/// @return RPI home frame complex widget pointer | NULL
RPIHomeFrame *new_rpi_home_frame(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show RPI home frame complex widget
/// @param instance is pointer to complex widget home frame
void show_rpi_home_frame(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide RPI home frame complex widget
/// @param instance is pointer to complex widget home frame
void hide_rpi_home_frame(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame container from RPI home frame complex widget
/// @param instance is pointer to complex widget home frame
/// @return Frame container from RPI home frame complex widget | NULL
GtkWidget *get_frame_from_rpi_home_frame(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets control sub-frame complex widget
/// @param instance is pointer to complex widget home frame
/// @return RPIHomeFrameControl pointer | NULL
RPIHomeFrameControl *rpi_home_frame_get_control(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets status sub-frame complex widget
/// @param instance is pointer to complex widget home frame
/// @return RPIHomeFrameStatus pointer | NULL
RPIHomeFrameStatus *rpi_home_frame_get_status(RPIHomeFrame *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Updates channel status value (channel_id 1-8, value 0-100)
/// @param instance is pointer to complex widget home frame
/// @param channel_id Channel identifier (1-8)
/// @param value Telemetry value (0-100)
void rpi_home_frame_update_channel_status(RPIHomeFrame *instance,
                                          gint channel_id, gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy RPI home frame complex widget
/// @param instance is pointer to complex widget home frame
void destroy_rpi_home_frame(RPIHomeFrame *instance);

G_END_DECLS
