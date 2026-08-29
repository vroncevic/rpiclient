/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_get_config_dir.c
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
#include "rpi_resource_path.h"
#include <stdlib.h>

static const gchar *home = "HOME";
static const gchar *RPI_RESOURCE_CONFIGURATION = "/.rpiclient/config/";

gchar *rpi_get_config_dir(void) {
  const char *home_directory = getenv(home);

  if (home_directory) {
    gchar *config_dir =
        g_strjoin(NULL, home_directory, RPI_RESOURCE_CONFIGURATION, NULL);

    if (!g_file_test(config_dir, G_FILE_TEST_IS_DIR)) {
      if (g_mkdir_with_parents(config_dir, 0700) != 0) {
        g_free(config_dir);
        config_dir = NULL;
        return NULL;
      }
    }

    return config_dir;
  }

  return NULL;
}
