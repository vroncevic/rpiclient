/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame_control.h
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

#include "../../channels/rpi_channel_control.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Number of control channels
enum { NUMBER_OF_CHANNELS_CONTROL = 8 };

//////////////////////////////////////////////////////////////////////////////
/// @brief Control frame complex widget
typedef struct _RPIHomeFrameControl RPIHomeFrameControl;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget control frame
/// @return RPIHomeFrameControl pointer | NULL
RPIHomeFrameControl *new_rpi_home_frame_control(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame container from control frame complex widget
/// @param instance Pointer to RPIHomeFrameControl
/// @return GtkWidget frame pointer | NULL
GtkWidget *
get_frame_from_rpi_home_frame_control(RPIHomeFrameControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets channel control widget by channel index (0-7)
/// @param instance Pointer to RPIHomeFrameControl
/// @param index Channel index (0-7)
/// @return RPIChannelControl pointer | NULL
RPIChannelControl *
rpi_home_frame_control_get_channel(RPIHomeFrameControl *instance, guint index);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy control frame complex widget
/// @param instance Pointer to RPIHomeFrameControl
void destroy_rpi_home_frame_control(RPIHomeFrameControl *instance);

G_END_DECLS
