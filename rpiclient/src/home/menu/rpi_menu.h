/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu.h
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

#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Menu bar RPI complex widget
typedef struct _RPIMenu RPIMenu;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget RPI menu bar
/// @return RPI menu bar complex widget pointer | NULL
RPIMenu *new_rpi_menu(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show RPI menu bar complex widget
/// @param instance is pointer to complex widget RPI menu bar
void show_rpi_menu(RPIMenu *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide RPI menu bar complex widget
/// @param instance is pointer to complex widget RPI menu bar
void hide_rpi_menu(RPIMenu *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets menu bar from RPI menu bar complex widget
/// @param instance is pointer to complex widget RPI menu bar
/// @return Menu bar widget from RPI menu bar complex widget | NULL
GtkWidget *get_menu_bar_from_rpi_menu(RPIMenu *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Connect signal menu bar
/// @param instance is pointer to complex widget RPI menu bar
/// @param on_callback is callback which should be connected
/// @param submenu is submenu name which marks to which submenu to connect
/// callback
void rpi_menu_connect_signal(RPIMenu *instance,
                             void (*on_callback)(GtkWidget *, gpointer),
                             const gchar *submenu);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets action group from RPI menu bar complex widget
/// @param instance is pointer to complex widget RPI menu bar
/// @return Action group | NULL
GActionGroup *get_action_group_from_rpi_menu(RPIMenu *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy menu bar complex widget
/// @param instance is pointer to complex widget RPI menu bar
void destroy_rpi_menu(RPIMenu *instance);

G_END_DECLS
