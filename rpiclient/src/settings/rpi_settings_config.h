/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_config.h
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

#include "rpi_settings_model.h"
#include <glib.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings configuration IO status codes
enum {
  SUCCESS_IO_SETTINGS_CONFIGURATION = 0,
  FAILED_IO_SETTINGS_CONFIGURATION = 1
};

//////////////////////////////////////////////////////////////////////////////
/// @brief Read configuration from backend storage
/// @return Settings configuration structure pointer | NULL
SettingsConfig *settings_read(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Write settings configuration to backend storage
/// @param instance is pointer to settings configuration to be written
/// @return Integer status 0 - success | 1 - failed
guint settings_write(const SettingsConfig *instance);

G_END_DECLS
