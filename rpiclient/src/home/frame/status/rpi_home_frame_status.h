/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame_status.h
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

#include "../../channels/rpi_channel_status.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Number of status channels
enum { NUMBER_OF_CHANNELS_STATUS = 8 };

//////////////////////////////////////////////////////////////////////////////
/// @brief Status frame complex widget
typedef struct _RPIHomeFrameStatus RPIHomeFrameStatus;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget status frame
/// @return RPIHomeFrameStatus pointer | NULL
RPIHomeFrameStatus *new_rpi_home_frame_status(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame container from status frame complex widget
/// @param instance Pointer to RPIHomeFrameStatus
/// @return GtkWidget frame pointer | NULL
GtkWidget *get_frame_from_rpi_home_frame_status(RPIHomeFrameStatus *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets channel status widget by channel index (0-7)
/// @param instance Pointer to RPIHomeFrameStatus
/// @param index Channel index (0-7)
/// @return RPIChannelStatus pointer | NULL
RPIChannelStatus *
rpi_home_frame_status_get_channel(RPIHomeFrameStatus *instance, guint index);

//////////////////////////////////////////////////////////////////////////////
/// @brief Updates status telemetry for specified channel (channel_id 1-8, value 0-100)
/// @param instance Pointer to RPIHomeFrameStatus
/// @param channel_id Channel identifier (1-8)
/// @param value Telemetry value (0-100)
void rpi_home_frame_status_update_channel(RPIHomeFrameStatus *instance,
                                          gint channel_id, gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy status frame complex widget
/// @param instance Pointer to RPIHomeFrameStatus
void destroy_rpi_home_frame_status(RPIHomeFrameStatus *instance);

G_END_DECLS
