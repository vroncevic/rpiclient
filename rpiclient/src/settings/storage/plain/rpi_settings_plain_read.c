/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_plain_read.c
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
#include "rpi_settings_plain_read.h"
#include "../../../resource/rpi_resource.h"
#include "../../../rpi_config.h"

#include <stdio.h>
#include <string.h>

#if RPI_VERBOSE == 1
static const gchar *const PROMPT_READ_SETTINGS_PLAIN =
    "Read from file no_prompt: %s.\n";
static const gchar *const IP_ADDRESS_READ_SETTINGS_PLAIN =
    "Read from file IP server address: %s.\n";
static const gchar *const PORT_NUMBER_READ_SETTINGS_PLAIN =
    "Read from file server port number: %s.\n";
static const gchar *const NO_EXIT_READ_SETTINGS_PLAIN =
    "Read from file no_exit: %s.\n";
#endif

static const gchar *const CONFIG_FILE_SETTINGS_PLAIN = "rpiclient.cfg";
static const gchar *const DEFAULT_CONTENT_SETTINGS_PLAIN =
    "no_prompt=false\nip_address=192.168.1.100\nport_number=8888\nno_exit=true\n";
static const gchar *const OPEN_FILE_FLAGS_SETTINGS_PLAIN = "r";
static const gint BUFFER_SIZE_SETTINGS_PLAIN = 100;
static const gchar *const SEPARATOR_FILE_SETTINGS_PLAIN = "=";
static const gint NUMBER_OF_TOKENS_FILE_SETTINGS_PLAIN = 2;

static const gchar *const FAILED_READ_SETTINGS_PLAIN =
    "Unable to read file: %s.\n";
static const gchar *const FAILED_OPEN_FILE_SETTINGS_PLAIN =
    "Unable to open file: %s.\n";

static const gchar *PROMPT_CONFIG_FILE_SETTINGS_PLAIN = "no_prompt";
static const gchar *IP_ADDRESS_CONFIG_FILE_SETTINGS_PLAIN = "ip_address";
static const gchar *PORT_NUMBER_CONFIG_FILE_SETTINGS_PLAIN = "port_number";
static const gchar *NO_EXIT_CONFIG_FILE_SETTINGS_PLAIN = "no_exit";

static gchar *rpi_read_from_settings_plain_file(const gchar *item);

gchar *rpi_read_prompt_settings_plain_file(void) {
  gchar *no_prompt =
      rpi_read_from_settings_plain_file(PROMPT_CONFIG_FILE_SETTINGS_PLAIN);

#if RPI_VERBOSE == 1
  g_debug(PROMPT_READ_SETTINGS_PLAIN, no_prompt);
#endif

  return no_prompt;
}

gchar *rpi_read_address_settings_plain_file(void) {
  gchar *ip_address =
      rpi_read_from_settings_plain_file(IP_ADDRESS_CONFIG_FILE_SETTINGS_PLAIN);

#if RPI_VERBOSE == 1
  g_debug(IP_ADDRESS_READ_SETTINGS_PLAIN, ip_address);
#endif

  return ip_address;
}

gchar *rpi_read_port_settings_plain_file(void) {
  gchar *port_number =
      rpi_read_from_settings_plain_file(PORT_NUMBER_CONFIG_FILE_SETTINGS_PLAIN);

#if RPI_VERBOSE == 1
  g_debug(PORT_NUMBER_READ_SETTINGS_PLAIN, port_number);
#endif

  return port_number;
}

gchar *rpi_read_exit_settings_plain_file(void) {
  gchar *no_exit =
      rpi_read_from_settings_plain_file(NO_EXIT_CONFIG_FILE_SETTINGS_PLAIN);

#if RPI_VERBOSE == 1
  g_debug(NO_EXIT_READ_SETTINGS_PLAIN, no_exit);
#endif

  return no_exit;
}

static gchar *rpi_read_from_settings_plain_file(const gchar *item) {
  gchar *conf_file = rpi_get_config_file_path(CONFIG_FILE_SETTINGS_PLAIN,
                                              DEFAULT_CONTENT_SETTINGS_PLAIN);

  if (!conf_file) {
    g_critical(FAILED_READ_SETTINGS_PLAIN, CONFIG_FILE_SETTINGS_PLAIN);
    return NULL;
  }

  FILE *fp = fopen(conf_file, OPEN_FILE_FLAGS_SETTINGS_PLAIN);

  if (!fp) {
    g_critical(FAILED_OPEN_FILE_SETTINGS_PLAIN, conf_file);
    g_free(conf_file);
    conf_file = NULL;
    return NULL;
  }

  gchar line[BUFFER_SIZE_SETTINGS_PLAIN];
  gchar *val = NULL;

  while (fgets(line, sizeof(line), fp)) {
    gchar **tokens = g_strsplit(g_strstrip(line), SEPARATOR_FILE_SETTINGS_PLAIN,
                                NUMBER_OF_TOKENS_FILE_SETTINGS_PLAIN);
    if (tokens && tokens[0] && tokens[1]) {
      if (g_strcmp0(g_strstrip(tokens[0]), item) == 0) {
        val = g_strdup(g_strstrip(tokens[1]));
        g_strfreev(tokens);
        break;
      }
    }
    g_strfreev(tokens);
  }

  fclose(fp);
  g_free(conf_file);
  conf_file = NULL;

  return val;
}
