/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_general_window.h
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

#include "../rpi_settings_config.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings general window complex widget
typedef struct _SettingsGeneralWindow SettingsGeneralWindow;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget settings general window
/// @return Settings general window complex widget pointer | NULL
SettingsGeneralWindow *new_settings_general_window(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Show settings general window complex widget
/// @param instance is pointer to complex widget settings general window
void show_settings_general_window(SettingsGeneralWindow *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Hide settings general window complex widget
/// @param instance is pointer to complex widget settings general window
void hide_settings_general_window(SettingsGeneralWindow *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy settings general window complex widget
/// @param instance is pointer to complex widget settings general window
void destroy_settings_general_window(SettingsGeneralWindow *instance);

G_END_DECLS
