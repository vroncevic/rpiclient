/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_sqlite_write.c
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
#include "rpi_settings_sqlite_write.h"
#include "../../../rpi_config.h"
#include "rpi_settings_sqlite_db.h"
#include <sqlite3.h>

#if RPI_VERBOSE == 1
static const gchar *const NO_PROMPT_WRITE_SETTINGS_SQLITE =
    "Write to database no_prompt: %s.\n";
static const gchar *const IP_ADDRESS_WRITE_SETTINGS_SQLITE =
    "Write to database IP server address: %s.\n";
static const gchar *const PORT_NUMBER_WRITE_SETTINGS_SQLITE =
    "Write to database server port number: %s.\n";
static const gchar *const NO_EXIT_WRITE_SETTINGS_SQLITE =
    "Write to database no_exit: %s.\n";
#endif

static const gchar *const SQL_UPDATE_NO_PROMPT_SETTINGS_SQLITE =
    "UPDATE settings SET no_prompt = '%s' WHERE id = 1;";
static const gchar *const SQL_UPDATE_IP_ADDRESS_SETTINGS_SQLITE =
    "UPDATE settings SET ip_address = '%s' WHERE id = 1;";
static const gchar *const SQL_UPDATE_PORT_NUMBER_SETTINGS_SQLITE =
    "UPDATE settings SET port_number = '%s' WHERE id = 1;";
static const gchar *const SQL_UPDATE_NO_EXIT_SETTINGS_SQLITE =
    "UPDATE settings SET no_exit = '%s' WHERE id = 1;";

static const gchar *const MISSING_NO_PROMPT_PARAMETER_SETTINGS_SQLITE =
    "Missing prompt parameter.\n";
static const gchar *const MISSING_IP_ADDRESS_PARAMETER_SETTINGS_SQLITE =
    "Missing server IP address parameter.\n";
static const gchar *const MISSING_PORT_NUMBER_PARAMETER_SETTINGS_SQLITE =
    "Missing server port number parameter.\n";
static const gchar *const MISSING_EXIT_PARAMETER_SETTINGS_SQLITE =
    "Missing exit parameter.\n";

static const gchar *const FAILED_UPDATE_NO_PROMPT_SETTINGS_SQLITE =
    "Can't update no_prompt parameter.\n";
static const gchar *const FAILED_UPDATE_IP_ADDRESS_SETTINGS_SQLITE =
    "Can't update ip_address parameter.\n";
static const gchar *const FAILED_UPDATE_PORT_NUMBER_SETTINGS_SQLITE =
    "Can't update port_number parameter.\n";
static const gchar *const FAILED_UPDATE_NO_EXIT_SETTINGS_SQLITE =
    "Can't update no_exit parameter.\n";

gint rpi_write_no_prompt_settings_sqlite(const gchar *no_prompt) {
  if (!no_prompt) {
    g_critical("%s", MISSING_NO_PROMPT_PARAMETER_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gchar *sql_update_no_prompt =
      g_strdup_printf(SQL_UPDATE_NO_PROMPT_SETTINGS_SQLITE, no_prompt);

  if (!sql_update_no_prompt) {
    g_critical("%s", FAILED_UPDATE_NO_PROMPT_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gint status = rpi_settings_sqlite_exec(sql_update_no_prompt, NULL);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);
  g_free(sql_update_no_prompt);
  sql_update_no_prompt = NULL;

#if RPI_VERBOSE == 1
  g_debug(NO_PROMPT_WRITE_SETTINGS_SQLITE, no_prompt);
#endif

  return ok_status ? SUCCESS_SETTINGS_SQLITE : FAILED_SETTINGS_SQLITE;
}

gint rpi_write_ip_address_settings_sqlite(const gchar *ip_address) {
  if (!ip_address) {
    g_critical("%s", MISSING_IP_ADDRESS_PARAMETER_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gchar *sql_update_ip_address =
      g_strdup_printf(SQL_UPDATE_IP_ADDRESS_SETTINGS_SQLITE, ip_address);

  if (!sql_update_ip_address) {
    g_critical("%s", FAILED_UPDATE_IP_ADDRESS_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gint status = rpi_settings_sqlite_exec(sql_update_ip_address, NULL);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);
  g_free(sql_update_ip_address);
  sql_update_ip_address = NULL;

#if RPI_VERBOSE == 1
  g_debug(IP_ADDRESS_WRITE_SETTINGS_SQLITE, ip_address);
#endif

  return ok_status ? SUCCESS_SETTINGS_SQLITE : FAILED_SETTINGS_SQLITE;
}

gint rpi_write_port_number_settings_sqlite(const gchar *port_number) {
  if (!port_number) {
    g_critical("%s", MISSING_PORT_NUMBER_PARAMETER_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gchar *sql_update_port_number =
      g_strdup_printf(SQL_UPDATE_PORT_NUMBER_SETTINGS_SQLITE, port_number);

  if (!sql_update_port_number) {
    g_critical("%s", FAILED_UPDATE_PORT_NUMBER_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gint status = rpi_settings_sqlite_exec(sql_update_port_number, NULL);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);
  g_free(sql_update_port_number);
  sql_update_port_number = NULL;

#if RPI_VERBOSE == 1
  g_debug(PORT_NUMBER_WRITE_SETTINGS_SQLITE, port_number);
#endif

  return ok_status ? SUCCESS_SETTINGS_SQLITE : FAILED_SETTINGS_SQLITE;
}

gint rpi_write_no_exit_settings_sqlite(const gchar *no_exit) {
  if (!no_exit) {
    g_critical("%s", MISSING_EXIT_PARAMETER_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gchar *sql_update_no_exit =
      g_strdup_printf(SQL_UPDATE_NO_EXIT_SETTINGS_SQLITE, no_exit);

  if (!sql_update_no_exit) {
    g_critical("%s", FAILED_UPDATE_NO_EXIT_SETTINGS_SQLITE);
    return FAILED_SETTINGS_SQLITE;
  }

  gint status = rpi_settings_sqlite_exec(sql_update_no_exit, NULL);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);
  g_free(sql_update_no_exit);
  sql_update_no_exit = NULL;

#if RPI_VERBOSE == 1
  g_debug(NO_EXIT_WRITE_SETTINGS_SQLITE, no_exit);
#endif

  return ok_status ? SUCCESS_SETTINGS_SQLITE : FAILED_SETTINGS_SQLITE;
}
