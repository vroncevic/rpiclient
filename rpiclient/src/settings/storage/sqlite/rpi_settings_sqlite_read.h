/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_sqlite_read.h
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
/// @brief Reads prompt setting from SQLite database
/// @return Allocated string containing prompt setting | NULL
gchar *rpi_read_no_prompt_settings_sqlite(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Reads server IP address setting from SQLite database
/// @return Allocated string containing IP address | NULL
gchar *rpi_read_ip_address_settings_sqlite(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Reads server port number setting from SQLite database
/// @return Allocated string containing port number | NULL
gchar *rpi_read_port_number_settings_sqlite(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Reads exit confirmation setting from SQLite database
/// @return Allocated string containing exit setting | NULL
gchar *rpi_read_no_exit_settings_sqlite(void);

G_END_DECLS
