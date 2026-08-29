/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_resource_path.h
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

#include <glib.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets resource directory path
/// @return Allocated directory path for resource | NULL
gchar *rpi_get_resource_dir(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets resource file path
/// @param file_name Resource file name
/// @return Allocated file path for resource | NULL
gchar *rpi_get_resource_file_path(const gchar *file_name);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets configuration directory path
/// @return Allocated directory path for configuration | NULL
gchar *rpi_get_config_dir(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets configuration file path
/// @param file_name Configuration file name
/// @param default_content Default content to write if file does not exist
/// @return Allocated file path for configuration | NULL
gchar *rpi_get_config_file_path(const gchar *file_name,
                                const gchar *default_content);

G_END_DECLS
