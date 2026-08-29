/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_sqlite_write.h
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
/// @brief Writes prompt setting to SQLite database
/// @param no_prompt Prompt setting string value
/// @return Status code (0 - success, 1 - failed)
gint rpi_write_no_prompt_settings_sqlite(const gchar *no_prompt);

//////////////////////////////////////////////////////////////////////////////
/// @brief Writes server IP address setting to SQLite database
/// @param ip_address IP address string value
/// @return Status code (0 - success, 1 - failed)
gint rpi_write_ip_address_settings_sqlite(const gchar *ip_address);

//////////////////////////////////////////////////////////////////////////////
/// @brief Writes server port number setting to SQLite database
/// @param port_number Port number string value
/// @return Status code (0 - success, 1 - failed)
gint rpi_write_port_number_settings_sqlite(const gchar *port_number);

//////////////////////////////////////////////////////////////////////////////
/// @brief Writes exit confirmation setting to SQLite database
/// @param no_exit Exit setting string value
/// @return Status code (0 - success, 1 - failed)
gint rpi_write_no_exit_settings_sqlite(const gchar *no_exit);

G_END_DECLS
