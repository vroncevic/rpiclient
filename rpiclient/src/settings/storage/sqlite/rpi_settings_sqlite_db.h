/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_sqlite_db.h
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
/// @brief Status codes for SQLite settings operations
enum { SUCCESS_SETTINGS_SQLITE = 0, FAILED_SETTINGS_SQLITE = 1 };

//////////////////////////////////////////////////////////////////////////////
/// @brief Executes SQL query against settings SQLite database
/// @param query SQL statement string to execute
/// @param result Output pointer to receive query result string (if SELECT), or NULL
/// @return Status code (0 - success, 1 - failed)
gint rpi_settings_sqlite_exec(const gchar *query, gchar **result);

G_END_DECLS
