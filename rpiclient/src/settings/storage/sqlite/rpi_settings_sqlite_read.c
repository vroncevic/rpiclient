/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_sqlite_read.c
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
#include "rpi_settings_sqlite_read.h"
#include "../../../rpi_config.h"
#include "rpi_settings_sqlite_db.h"
#include <sqlite3.h>

#if RPI_VERBOSE == 1
static const gchar *const NO_PROMPT_READ_SETTINGS_SQLITE =
    "Read from database no_prompt: %s.\n";
static const gchar *const IP_ADDRESS_READ_SETTINGS_SQLITE =
    "Read from database IP server address: %s.\n";
static const gchar *const PORT_NUMBER_READ_SETTINGS_SQLITE =
    "Read from database server port number: %s.\n";
static const gchar *const NO_EXIT_READ_SETTINGS_SQLITE =
    "Read from database no_exit: %s.\n";
#endif

static const gchar *const SQL_SELECT_NO_PROMPT_SETTINGS_SQLITE =
    "SELECT no_prompt FROM settings WHERE id = 1;";
static const gchar *const SQL_SELECT_IP_ADDRESS_SETTINGS_SQLITE =
    "SELECT ip_address FROM settings WHERE id = 1;";
static const gchar *const SQL_SELECT_PORT_NUMBER_SETTINGS_SQLITE =
    "SELECT port_number FROM settings WHERE id = 1;";
static const gchar *const SQL_SELECT_NO_EXIT_SETTINGS_SQLITE =
    "SELECT no_exit FROM settings WHERE id = 1;";

gchar *rpi_read_no_prompt_settings_sqlite(void) {
  gchar *no_prompt = NULL;
  gint status = rpi_settings_sqlite_exec(SQL_SELECT_NO_PROMPT_SETTINGS_SQLITE,
                                         &no_prompt);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);

#if RPI_VERBOSE == 1
  g_debug(NO_PROMPT_READ_SETTINGS_SQLITE, no_prompt);
#endif

  return ok_status ? no_prompt : NULL;
}

gchar *rpi_read_ip_address_settings_sqlite(void) {
  gchar *ip_address = NULL;
  gint status = rpi_settings_sqlite_exec(SQL_SELECT_IP_ADDRESS_SETTINGS_SQLITE,
                                         &ip_address);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);

#if RPI_VERBOSE == 1
  g_debug(IP_ADDRESS_READ_SETTINGS_SQLITE, ip_address);
#endif

  return ok_status ? ip_address : NULL;
}

gchar *rpi_read_port_number_settings_sqlite(void) {
  gchar *port_number = NULL;
  gint status = rpi_settings_sqlite_exec(SQL_SELECT_PORT_NUMBER_SETTINGS_SQLITE,
                                         &port_number);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);

#if RPI_VERBOSE == 1
  g_debug(PORT_NUMBER_READ_SETTINGS_SQLITE, port_number);
#endif

  return ok_status ? port_number : NULL;
}

gchar *rpi_read_no_exit_settings_sqlite(void) {
  gchar *no_exit = NULL;
  gint status =
      rpi_settings_sqlite_exec(SQL_SELECT_NO_EXIT_SETTINGS_SQLITE, &no_exit);
  gboolean ok_status =
      (status == SQLITE_DONE || status == SUCCESS_SETTINGS_SQLITE);

#if RPI_VERBOSE == 1
  g_debug(NO_EXIT_READ_SETTINGS_SQLITE, no_exit);
#endif

  return ok_status ? no_exit : NULL;
}
