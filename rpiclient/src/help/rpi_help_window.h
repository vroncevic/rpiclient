/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_help_window.h
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
/// @brief Help window complex widget
typedef struct _HelpWindow HelpWindow;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget help window
/// @return Help window complex widget pointer | NULL
HelpWindow *new_help_window(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show help window complex widget
/// @param instance is pointer to complex widget help window
void show_help_window(HelpWindow *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide help window complex widget
/// @param instance is pointer to complex widget help window
void hide_help_window(HelpWindow *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy help window complex widget
/// @param instance is pointer to complex widget help window
void destroy_help_window(HelpWindow *instance);

G_END_DECLS
