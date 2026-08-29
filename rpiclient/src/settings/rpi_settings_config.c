/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_config.c
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
#include "rpi_settings_config.h"
#include "../rpi_config.h"
#include "rpi_settings.h"
#include "storage/plain/rpi_settings_plain.h"
#include "storage/rpi_settings_provider.h"
#include "storage/sqlite/rpi_settings_sqlite.h"

#if (RPI_USE_SETTINGS_PLAIN_CONFIG == 1) && (RPI_USE_SETTINGS_SQLITE3 == 1)
#error                                                                         \
    "You cannot use both RPI_USE_SETTINGS_PLAIN_CONFIG and RPI_USE_SETTINGS_SQLITE3 at the same time!"
#endif

#if RPI_VERBOSE == 1
static const gchar *const READ_SUCCESS_SETTINGS_CONFIG =
    "Successfully read settings.\n";
static const gchar *const WRITE_SUCCESS_SETTINGS_CONFIG =
    "Successfully write settings.\n";
#endif

static const gchar *const MISSING_PARAMETER_SETTINGS_CONFIG =
    "Missing settings parameter for write settings configuration.\n";
static const gchar *const FAILED_READ_SETTINGS_CONFIG =
    "Failed to read settings configuration.\n";
static const gchar *const FAILED_WRITE_SETTINGS_CONFIG =
    "Failed to write settings configuration.\n";

static const SettingsStorageProvider *get_current_provider(void) {
#if (RPI_USE_SETTINGS_PLAIN_CONFIG == 1) && (RPI_USE_SETTINGS_SQLITE3 == 0)
  return rpi_settings_plain_get_provider();
#elif (RPI_USE_SETTINGS_PLAIN_CONFIG == 0) && (RPI_USE_SETTINGS_SQLITE3 == 1)
  return rpi_settings_sqlite_get_provider();
#else
#error "Invalid configuration settings backend choice."
#endif
}

SettingsConfig *settings_read(void) {
  const SettingsStorageProvider *provider = get_current_provider();
  gchar *prompt_config = provider->read_prompt();
  gchar *address_config = provider->read_address();
  gchar *port_config = provider->read_port();
  gchar *exit_config = provider->read_exit();
  gboolean nok_settings =
      (!prompt_config || !address_config || !port_config || !exit_config);

  if (nok_settings) {
    g_critical("%s", FAILED_READ_SETTINGS_CONFIG);

    if (prompt_config)
      g_free(prompt_config);
    if (address_config)
      g_free(address_config);
    if (port_config)
      g_free(port_config);
    if (exit_config)
      g_free(exit_config);

    return NULL;
  }

  SettingsConfig *instance = settings_model_new(
      prompt_config, address_config, port_config, exit_config);

  g_free(prompt_config);
  g_free(address_config);
  g_free(port_config);
  g_free(exit_config);

#if RPI_VERBOSE == 1
  g_debug(READ_SUCCESS_SETTINGS_CONFIG);
#endif

  return instance;
}

guint settings_write(const SettingsConfig *instance) {
  if (!instance) {
    g_critical("%s", MISSING_PARAMETER_SETTINGS_CONFIG);
    return FAILED_IO_SETTINGS_CONFIGURATION;
  }

  const SettingsStorageProvider *provider = get_current_provider();

#if (RPI_USE_SETTINGS_PLAIN_CONFIG == 1)
  guint err_flag = FAILED_SETTINGS_PLAIN;
#else
  guint err_flag = FAILED_SETTINGS_SQLITE;
#endif

  const gchar *prompt = settings_model_get_prompt_raw(instance);
  const gchar *address = settings_model_get_address_raw(instance);
  const gchar *port = settings_model_get_port_raw(instance);
  const gchar *exit_val = settings_model_get_exit_raw(instance);

  guint prompt_status = provider->write_prompt(prompt);
  guint address_status = provider->write_address(address);
  guint port_status = provider->write_port(port);
  guint exit_status = provider->write_exit(exit_val);
  gboolean nok_write_status =
      (prompt_status == err_flag || address_status == err_flag ||
       port_status == err_flag || exit_status == err_flag);

  if (nok_write_status) {
    g_critical("%s", FAILED_WRITE_SETTINGS_CONFIG);
    return FAILED_IO_SETTINGS_CONFIGURATION;
  }

#if RPI_VERBOSE == 1
  g_debug(WRITE_SUCCESS_SETTINGS_CONFIG);
#endif

  return SUCCESS_IO_SETTINGS_CONFIGURATION;
}
