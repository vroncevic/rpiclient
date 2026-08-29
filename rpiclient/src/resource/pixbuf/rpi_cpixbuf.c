/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_cpixbuf.c
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
#include "rpi_cpixbuf.h"

GdkPixbuf *rpi_cpixbuf(const gchar *file_path) {
  if (!file_path) {
    return NULL;
  }

  GError *error = NULL;
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(file_path, &error);

  if (!pixbuf) {
    if (error) {
      g_critical("%s\n", error->message);
      g_error_free(error);
      error = NULL;
    }
    return NULL;
  }

  return pixbuf;
}
