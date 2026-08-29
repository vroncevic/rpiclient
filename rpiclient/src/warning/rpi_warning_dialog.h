/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_warning_dialog.h
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
/// @brief Warning dialog complex widget
typedef struct _WarningDialog WarningDialog;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget warning dialog
/// @param widget is parent widget (another window | dialog)
/// @param msg is warning message to display
/// @return Warning dialog complex widget pointer | NULL
WarningDialog *new_warning_dialog(GtkWidget *widget, const gchar *msg);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show warning dialog complex widget
/// @param instance is pointer to complex widget warning dialog
void show_warning_dialog(WarningDialog *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide warning dialog complex widget
/// @param instance is pointer to complex widget warning dialog
void hide_warning_dialog(WarningDialog *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy warning dialog complex widget
/// @param instance is pointer to complex widget warning dialog
void destroy_warning_dialog(WarningDialog *instance);

G_END_DECLS
