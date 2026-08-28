/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_get_resource_dir.c
 * Copyright (C) 2016 - 2025 Vladimir Roncevic <elektron.ronca@gmail.com>
 *
 * rpiclient-gtk is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rpiclient-gtk is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "rpi_resource.h"

static const gchar* RPI_RESOURCE_PATH = "/usr/local/share/rpiclient/images/";
static const gchar* RPI_RESOURCE_PATH_WITH_PREFIX = "../share/rpiclient/images/";

gchar *rpi_get_resource_dir(void)
{
    gchar* image_dir = g_strdup(RPI_RESOURCE_PATH);

    if (g_file_test(image_dir, G_FILE_TEST_IS_DIR))
    {
        return image_dir;
    }

    g_free(image_dir);
    image_dir = NULL;
    gchar *image_dir_prefix = g_strdup(RPI_RESOURCE_PATH_WITH_PREFIX);

    if (g_file_test(image_dir_prefix, G_FILE_TEST_IS_DIR))
    {
        return image_dir_prefix;
    }

    g_free(image_dir_prefix);
    image_dir_prefix = NULL;

    const gchar* dev_path1 = "rc/images/";
    if (g_file_test(dev_path1, G_FILE_TEST_IS_DIR))
    {
        return g_strdup(dev_path1);
    }

    const gchar* dev_path2 = "../rc/images/";
    if (g_file_test(dev_path2, G_FILE_TEST_IS_DIR))
    {
        return g_strdup(dev_path2);
    }

    return NULL;    
}
