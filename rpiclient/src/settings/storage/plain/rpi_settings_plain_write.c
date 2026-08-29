/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_plain_write.c
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
#include "rpi_settings_plain_write.h"
#include "../../../resource/rpi_resource.h"
#include "../../../rpi_config.h"
#include "rpi_settings_plain.h"

#include <stdio.h>
#include <string.h>

#if RPI_VERBOSE == 1
static const gchar *const PROMPT_WRITE_SETTINGS_PLAIN =
    "Write to file no_prompt: %s.\n";
static const gchar *const IP_ADDRESS_WRITE_SETTINGS_PLAIN =
    "Write to file IP server address: %s.\n";
static const gchar *const PORT_NUMBER_WRITE_SETTINGS_PLAIN =
    "Write to file server port number: %s.\n";
static const gchar *const NO_EXIT_WRITE_SETTINGS_PLAIN =
    "Write to file no_exit: %s.\n";
#endif

static const gchar *const CONFIG_FILE_SETTINGS_PLAIN = "rpiclient.cfg";
static const gchar *const DEFAULT_CONTENT_SETTINGS_PLAIN =
    "no_prompt=false\nip_address=192.168.1.100\nport_number=8888\nno_exit=true\n";
static const gint BUFFER_SIZE_SETTINGS_PLAIN = 100;
static const gchar *const SEPARATOR_FILE_SETTINGS_PLAIN = "=";
static const gint NUMBER_OF_TOKENS_FILE_SETTINGS_PLAIN = 2;

static const gchar *const MISSING_NO_PROMPT_PARAMETER_SETTINGS_PLAIN =
    "Missing prompt parameter.\n";
static const gchar *const MISSING_IP_ADDRESS_PARAMETER_SETTINGS_PLAIN =
    "Missing server IP address parameter.\n";
static const gchar *const MISSING_PORT_NUMBER_PARAMETER_SETTINGS_PLAIN =
    "Missing server port number parameter.\n";
static const gchar *const MISSING_EXIT_PARAMETER_SETTINGS_PLAIN =
    "Missing exit parameter.\n";

static const gchar *PROMPT_CONFIG_FILE_SETTINGS_PLAIN = "no_prompt";
static const gchar *IP_ADDRESS_CONFIG_FILE_SETTINGS_PLAIN = "ip_address";
static const gchar *PORT_NUMBER_CONFIG_FILE_SETTINGS_PLAIN = "port_number";
static const gchar *NO_EXIT_CONFIG_FILE_SETTINGS_PLAIN = "no_exit";

static guint rpi_write_to_settings_plain_file(const gchar *item,
                                              const gchar *value);

guint rpi_write_prompt_settings_plain_file(const gchar *no_prompt) {
  if (!no_prompt) {
    g_critical("%s", MISSING_NO_PROMPT_PARAMETER_SETTINGS_PLAIN);
    return FAILED_SETTINGS_PLAIN;
  }

  guint status = rpi_write_to_settings_plain_file(
      PROMPT_CONFIG_FILE_SETTINGS_PLAIN, no_prompt);

#if RPI_VERBOSE == 1
  g_debug(PROMPT_WRITE_SETTINGS_PLAIN, no_prompt);
#endif

  return status;
}

guint rpi_write_address_settings_plain_file(const gchar *ip_address) {
  if (!ip_address) {
    g_critical("%s", MISSING_IP_ADDRESS_PARAMETER_SETTINGS_PLAIN);
    return FAILED_SETTINGS_PLAIN;
  }

  guint status = rpi_write_to_settings_plain_file(
      IP_ADDRESS_CONFIG_FILE_SETTINGS_PLAIN, ip_address);

#if RPI_VERBOSE == 1
  g_debug(IP_ADDRESS_WRITE_SETTINGS_PLAIN, ip_address);
#endif

  return status;
}

guint rpi_write_port_settings_plain_file(const gchar *port_number) {
  if (!port_number) {
    g_critical("%s", MISSING_PORT_NUMBER_PARAMETER_SETTINGS_PLAIN);
    return FAILED_SETTINGS_PLAIN;
  }

  guint status = rpi_write_to_settings_plain_file(
      PORT_NUMBER_CONFIG_FILE_SETTINGS_PLAIN, port_number);

#if RPI_VERBOSE == 1
  g_debug(PORT_NUMBER_WRITE_SETTINGS_PLAIN, port_number);
#endif

  return status;
}

guint rpi_write_exit_settings_plain_file(const gchar *no_exit) {
  if (!no_exit) {
    g_critical("%s", MISSING_EXIT_PARAMETER_SETTINGS_PLAIN);
    return FAILED_SETTINGS_PLAIN;
  }

  guint status = rpi_write_to_settings_plain_file(
      NO_EXIT_CONFIG_FILE_SETTINGS_PLAIN, no_exit);

#if RPI_VERBOSE == 1
  g_debug(NO_EXIT_WRITE_SETTINGS_PLAIN, no_exit);
#endif

  return status;
}

static guint rpi_write_to_settings_plain_file(const gchar *item,
                                              const gchar *value) {
  gchar *conf_file = rpi_get_config_file_path(CONFIG_FILE_SETTINGS_PLAIN,
                                              DEFAULT_CONTENT_SETTINGS_PLAIN);

  if (!conf_file) {
    return FAILED_SETTINGS_PLAIN;
  }

  FILE *fp = fopen(conf_file, "r");
  GString *new_content = g_string_new("");
  gboolean found = FALSE;

  if (fp) {
    gchar line[BUFFER_SIZE_SETTINGS_PLAIN];
    while (fgets(line, sizeof(line), fp)) {
      gchar **tokens =
          g_strsplit(g_strstrip(line), SEPARATOR_FILE_SETTINGS_PLAIN,
                     NUMBER_OF_TOKENS_FILE_SETTINGS_PLAIN);
      if (tokens && tokens[0] && tokens[1]) {
        if (g_strcmp0(g_strstrip(tokens[0]), item) == 0) {
          g_string_append_printf(new_content, "%s=%s\n", item, value);
          found = TRUE;
        } else {
          g_string_append_printf(new_content, "%s=%s\n", g_strstrip(tokens[0]),
                                 g_strstrip(tokens[1]));
        }
      }
      g_strfreev(tokens);
    }
    fclose(fp);
  }

  if (!found) {
    g_string_append_printf(new_content, "%s=%s\n", item, value);
  }

  fp = fopen(conf_file, "w");
  if (!fp) {
    g_string_free(new_content, TRUE);
    g_free(conf_file);
    return FAILED_SETTINGS_PLAIN;
  }

  fputs(new_content->str, fp);
  fclose(fp);
  g_string_free(new_content, TRUE);
  g_free(conf_file);

  return SUCCESS_SETTINGS_PLAIN;
}
