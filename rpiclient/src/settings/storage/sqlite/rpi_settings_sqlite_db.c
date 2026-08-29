/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_sqlite_db.c
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
#include "rpi_settings_sqlite_db.h"
#include "../../../rpi_config.h"
#include "../../rpi_settings.h"
#include "../rpi_settings_user.h"

#include <sqlite3.h>

#if RPI_VERBOSE == 1
static const gchar *const OPEN_DB_SETTINGS_SQLITE =
    "Successfully opened database: %s.\n";
static const gchar *const CLOSE_DB_SETTINGS_SQLITE =
    "Successfully closed database.\n";
static const gchar *const SQL_DB_SETTINGS_SQLITE = "SQL query: %s.\n";
#endif

static const gchar *const CONFIGURATION_FILE_SETTINGS_SQLITE =
    "/home/%s/.rpiclient/config/rpiclient.db";
static const gchar *const SQL_SELECT_SETTINGS_SQLITE = "SELECT";

static const gchar *const FAILED_OPEN_DB_MISSING_NAME_SETTINGS_SQLITE =
    "Can't open database, missing name.\n";
static const gchar *const FAILED_EXEC_DB_MISSING_QUERY_SETTINGS_SQLITE =
    "Missing query for sqlite exec.\n";
static const gchar *const FAILED_EXEC_DB_PATH_SETTINGS_SQLITE =
    "Can't prepare database path.\n";
static const gchar *const FAILED_CLOSE_DB_SETTINGS_SQLITE =
    "Database is already closed or was never opened.\n";
static const gchar *const FAILED_SQL_ERROR_SETTINGS_SQLITE = "SQL error: %s.\n";

static sqlite3 *db = NULL;
static void rpi_settings_sqlite_open(const gchar *db_name);
static void rpi_settings_sqlite_close(void);

gint rpi_settings_sqlite_exec(const gchar *query, gchar **result) {
  if (!query) {
    g_critical("%s", FAILED_EXEC_DB_MISSING_QUERY_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gchar *username = rpi_get_username_settings_user();

  if (!username) {
    return FAILED_SETTINGS_SQLITE;
  }

  gchar *db_name =
      g_strdup_printf(CONFIGURATION_FILE_SETTINGS_SQLITE, username);

  if (!db_name) {
    g_critical("%s", FAILED_EXEC_DB_PATH_SETTINGS_SQLITE);
    g_free(username);
    username = NULL;
    return FAILED_SETTINGS_SQLITE;
  }

  g_free(username);
  username = NULL;
  rpi_settings_sqlite_open(db_name);
  gint status = FAILED_SETTINGS_SQLITE;

  if (db) {
    //////////////////////////////////////////////////////////////////////
    /// Create table if not exists
    const char *sql_create_table = "CREATE TABLE IF NOT EXISTS settings ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "no_prompt TEXT, "
                                   "ip_address TEXT, "
                                   "port_number TEXT,"
                                   "no_exit TEXT);";
    gchar *error_message = NULL;
    status = sqlite3_exec(db, sql_create_table, 0, 0, &error_message);

    if (status != SQLITE_OK) {
      if (error_message) {
        g_critical(FAILED_SQL_ERROR_SETTINGS_SQLITE, error_message);
      }

      rpi_settings_sqlite_close();
      g_free(error_message);
      error_message = NULL;
      g_free(db_name);
      db_name = NULL;
      return status;
    }

    //////////////////////////////////////////////////////////////////////
    /// Insert default values if table is empty
    const char *sql_insert_default =
        "INSERT INTO settings (no_prompt, ip_address, port_number, no_exit) "
        "SELECT 'false', '192.168.1.100', '8888', 'true' "
        "WHERE NOT EXISTS (SELECT 1 FROM settings);";
    status = sqlite3_exec(db, sql_insert_default, 0, 0, &error_message);

    if (status != SQLITE_OK) {
      if (error_message) {
        g_critical(FAILED_SQL_ERROR_SETTINGS_SQLITE, error_message);
      }

      rpi_settings_sqlite_close();
      g_free(error_message);
      error_message = NULL;
      g_free(db_name);
      db_name = NULL;
      return status;
    }

    if (g_str_has_prefix(query, SQL_SELECT_SETTINGS_SQLITE)) {
      //////////////////////////////////////////////////////////////////
      /// Execute SELECT queries
      sqlite3_stmt *statement;
      status = sqlite3_prepare_v2(db, query, -1, &statement, NULL);

      if (status != SQLITE_OK) {
        g_critical(FAILED_SQL_ERROR_SETTINGS_SQLITE, sqlite3_errmsg(db));
        rpi_settings_sqlite_close();
        g_free(db_name);
        db_name = NULL;
        return status;
      }

#if RPI_VERBOSE == 1
      g_debug(SQL_DB_SETTINGS_SQLITE, query);
#endif

      status = sqlite3_step(statement);

      if (status == SQLITE_ROW) {
        const char *value = (const char *)sqlite3_column_text(statement, 0);

        if (result) {
          *result = g_strdup(value);
        }
      } else {
        g_critical(FAILED_SQL_ERROR_SETTINGS_SQLITE, sqlite3_errmsg(db));
      }

      sqlite3_finalize(statement);
    } else {
      //////////////////////////////////////////////////////////////////
      /// Execute non-SELECT queries
      status = sqlite3_exec(db, query, 0, 0, &error_message);

      if (status != SQLITE_OK) {
        if (error_message) {
          g_critical(FAILED_SQL_ERROR_SETTINGS_SQLITE, error_message);
        }

        rpi_settings_sqlite_close();
        g_free(error_message);
        error_message = NULL;
        g_free(db_name);
        db_name = NULL;
        return status;
      }

#if RPI_VERBOSE == 1
      g_debug(SQL_DB_SETTINGS_SQLITE, query);
#endif
    }

    status = SUCCESS_SETTINGS_SQLITE;
  }

  rpi_settings_sqlite_close();
  g_free(db_name);
  db_name = NULL;
  return status;
}

static void rpi_settings_sqlite_open(const gchar *db_name) {
  if (!db_name) {
    g_critical("%s", FAILED_OPEN_DB_MISSING_NAME_SETTINGS_SQLITE);
    return;
  }

  gint status = sqlite3_open(db_name, &db);

  if (status != SQLITE_OK) {
    g_critical(FAILED_SQL_ERROR_SETTINGS_SQLITE, sqlite3_errmsg(db));
    db = NULL;
    return;
  }

#if RPI_VERBOSE == 1
  g_debug(OPEN_DB_SETTINGS_SQLITE, db_name);
#endif
}

static void rpi_settings_sqlite_close(void) {
  if (!db) {
    g_critical("%s", FAILED_CLOSE_DB_SETTINGS_SQLITE);
    return;
  }

  gint status = sqlite3_close(db);

  if (status != SQLITE_OK) {
    g_critical(FAILED_SQL_ERROR_SETTINGS_SQLITE, sqlite3_errmsg(db));
    return;
  }

  db = NULL;

#if RPI_VERBOSE == 1
  g_debug(CLOSE_DB_SETTINGS_SQLITE);
#endif
}
