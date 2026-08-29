/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_user.c
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
#include "rpi_settings_user.h"
#include "../../rpi_config.h"

#include <stdio.h>
#include <unistd.h>

#if RPI_VERBOSE == 1
static const gchar *const USERNAME_SETTINGS_USER =
    "Successfully get username: %s.\n";
#endif

static const gchar *const FAILED_GET_USERNAME_SETTINGS_USER =
    "Unable to retrieve the username.\n";

gchar *rpi_get_username_settings_user(void) {
  gchar *username = g_strdup(g_get_user_name());

  if (!username) {
    g_critical("%s", FAILED_GET_USERNAME_SETTINGS_USER);
    return NULL;
  }

#if RPI_VERBOSE == 1
  g_debug(USERNAME_SETTINGS_USER, username);
#endif

  return username;
}
