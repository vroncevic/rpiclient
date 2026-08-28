/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home.h
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

#include "rpi_menu.h"

//////////////////////////////////////////////////////////////////////////////
/// @brief RPIHome complex widget
typedef struct _RPIHome RPIHome;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget home
/// @return RPIHome view complex widget pointer | NULL
RPIHome *new_rpi_home(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show home complex widget
/// @param instance is pointer to complex widget home
void show_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide home complex widget
/// @param instance is pointer to complex widget home
void hide_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets window from home complex widget
/// @param instance is pointer to complex widget home
/// @return Window from complex widget home | NULL
GtkWidget* get_window_from_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets menu bar from home complex widget
/// @param instance is pointer to complex widget home
/// @return Menu bar from complex widget home | NULL
RPIMenu* get_menu_bar_from_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Updates status for a channel on home window
/// @param instance is pointer to complex widget home
/// @param channel_id is channel number (1-8)
/// @param value is status percentage value (0-100)
void rpi_home_update_channel_status(RPIHome *instance, gint channel_id, gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy home complex widget
/// @param instance is pointer to complex widget home
void destroy_rpi_home(RPIHome *instance);
