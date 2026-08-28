/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_channel_control.h
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
/// @brief Channel control complex widget
typedef struct _RPIChannelControl RPIChannelControl;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget channel control
/// @param channel_id is channel identification number
/// @return Channel control complex widget pointer | NULL
RPIChannelControl *new_rpi_channel_control(gint channel_id);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show channel control complex widget
/// @param instance is pointer to complex widget channel control
void show_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide channel control complex widget
/// @param instance is pointer to complex widget channel control
void hide_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets vertical bar from channel control complex widget
/// @param instance is pointer to complex widget channel control
/// @return Vertical bar from complex widget channel control | NULL
GtkVB* get_vertical_bar_from_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets scale from channel control complex widget
/// @param instance is pointer to complex widget channel control
/// @return Scale from complex widget channel control | NULL
GtkWidget* get_scale_from_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets spinner button from channel control complex widget
/// @param instance is pointer to complex widget channel control
/// @return Spinner button from complex widget channel control | NULL
GtkWidget* get_spinner_button_from_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets check box from channel control complex widget
/// @param instance is pointer to complex widget channel control
/// @return Check box from complex widget channel control | NULL
GtkWidget* get_check_box_from_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets frame from channel control complex widget
/// @param instance is pointer to complex widget channel control
/// @return Frame from complex widget channel control | NULL
GtkWidget* get_frame_from_rpi_channel_control(RPIChannelControl *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy channel control complex widget
/// @param instance is pointer to complex widget channel control
void destroy_rpi_channel_control(RPIChannelControl *instance);
