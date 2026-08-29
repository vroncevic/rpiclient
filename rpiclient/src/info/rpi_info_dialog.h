/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_info_dialog.h
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
/// @brief Info dialog complex widget
typedef struct _InfoDialog InfoDialog;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget info dialog
/// @param widget is parent widget (another window | dialog)
/// @param msg is info message to display
/// @return Info dialog complex widget pointer | NULL
InfoDialog *new_info_dialog(GtkWidget *widget, const gchar *msg);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show info dialog complex widget
/// @param instance is pointer to complex widget info dialog
void show_info_dialog(InfoDialog *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide info dialog complex widget
/// @param instance is pointer to complex widget info dialog
void hide_info_dialog(InfoDialog *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy info dialog complex widget
/// @param instance is pointer to complex widget info dialog
void destroy_info_dialog(InfoDialog *instance);

G_END_DECLS
