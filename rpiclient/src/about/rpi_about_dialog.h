/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_about_dialog.h
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
/// @brief About dialog complex widget
typedef struct _AboutDialog AboutDialog;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget about dialog
/// @return About dialog complex widget pointer | NULL
AboutDialog *new_about_dialog(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show about dialog complex widget
/// @param instance is pointer to complex widget about dialog
void show_about_dialog(AboutDialog *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide about dialog complex widget
/// @param instance is pointer to complex widget about dialog
void hide_about_dialog(AboutDialog *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy about dialog complex widget
/// @param instance is pointer to complex widget about dialog
void destroy_about_dialog(AboutDialog *instance);

G_END_DECLS
