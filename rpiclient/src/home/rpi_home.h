/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home.h
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

#include "frame/rpi_home_frame.h"
#include "menu/rpi_menu.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Home complex widget
typedef struct _RPIHome RPIHome;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget home
/// @return RPI complex widget pointer | NULL
RPIHome *new_rpi_home(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show RPI complex widget
/// @param instance is pointer to complex widget home
void show_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide RPI complex widget
/// @param instance is pointer to complex widget home
void hide_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets main window widget from RPI complex widget
/// @param instance is pointer to complex widget home
/// @return Main window widget from RPI complex widget | NULL
GtkWidget *get_window_from_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets menu bar from RPI complex widget
/// @param instance is pointer to complex widget home
/// @return Menu bar widget from RPI complex widget | NULL
RPIMenu *get_menu_bar_from_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets home frame from RPI complex widget
/// @param instance is pointer to complex widget home
/// @return Home frame widget from RPI complex widget | NULL
RPIHomeFrame *get_home_frame_from_rpi_home(RPIHome *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Updates status channel telemetry value
/// @param instance is pointer to complex widget home
/// @param channel_id Channel identifier (1-8)
/// @param value Telemetry value (0-100)
void rpi_home_update_channel_status(RPIHome *instance, gint channel_id,
                                    gint value);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy RPI complex widget
/// @param instance is pointer to complex widget home
void destroy_rpi_home(RPIHome *instance);

G_END_DECLS
