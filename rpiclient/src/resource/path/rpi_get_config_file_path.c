/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_get_config_file_path.c
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
#include <gio/gio.h>

gchar *rpi_get_config_file_path(const gchar *file_name,
                                const gchar *default_content) {
  if (!file_name || !default_content) {
    return NULL;
  }

  gchar *cfg_dir = rpi_get_config_dir();
  if (!cfg_dir) {
    return NULL;
  }

  gchar *file_path = g_build_filename(cfg_dir, file_name, NULL);
  g_free(cfg_dir);

  if (!g_file_test(file_path, G_FILE_TEST_EXISTS)) {
    GError *error = NULL;
    GFile *file = g_file_new_for_path(file_path);
    GFileOutputStream *stream =
        g_file_replace(file, NULL, FALSE, G_FILE_CREATE_NONE, NULL, &error);

    if (error) {
      g_critical("Failed to create file: %s", error->message);
      g_error_free(error);
      g_free(file_path);
      g_object_unref(file);
      return NULL;
    }

    g_output_stream_write(G_OUTPUT_STREAM(stream), default_content,
                          strlen(default_content), NULL, &error);
    g_output_stream_close(G_OUTPUT_STREAM(stream), NULL, &error);

    if (error) {
      g_critical("Failed to write to file: %s", error->message);
      g_error_free(error);
    }

    g_object_unref(stream);
    g_object_unref(file);
  }

  return file_path;
}
