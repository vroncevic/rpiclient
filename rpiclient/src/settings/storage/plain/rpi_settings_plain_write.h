/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_plain_write.h
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
/// @brief Writes prompt setting to plain text file
/// @param no_prompt Value string to write
/// @return Status code (0 - success, 1 - failed)
guint rpi_write_prompt_settings_plain_file(const gchar *no_prompt);

//////////////////////////////////////////////////////////////////////////////
/// @brief Writes IP address setting to plain text file
/// @param ip_address IP address string to write
/// @return Status code (0 - success, 1 - failed)
guint rpi_write_address_settings_plain_file(const gchar *ip_address);

//////////////////////////////////////////////////////////////////////////////
/// @brief Writes port number setting to plain text file
/// @param port_number Port number string to write
/// @return Status code (0 - success, 1 - failed)
guint rpi_write_port_settings_plain_file(const gchar *port_number);

//////////////////////////////////////////////////////////////////////////////
/// @brief Writes exit confirmation setting to plain text file
/// @param no_exit Exit setting string to write
/// @return Status code (0 - success, 1 - failed)
guint rpi_write_exit_settings_plain_file(const gchar *no_exit);

G_END_DECLS
