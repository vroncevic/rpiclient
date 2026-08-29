/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu_settings.h
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
/// @brief Creates Settings menu component and its menu items
/// @param out_menu_item Pointer to receive the top-level Settings menu item
/// @param out_general_item Pointer to receive the General settings menu item
/// @param out_network_item Pointer to receive the Network settings menu item
/// @return GtkWidget pointer to the created GtkMenu | NULL
GtkWidget *rpi_menu_settings_new(GtkWidget **out_menu_item,
                                 GtkWidget **out_general_item,
                                 GtkWidget **out_network_item);

G_END_DECLS
