/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu_file.c
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
#include "rpi_menu_file.h"

static const gchar *const FAILED_MALLOC_RPI_MENU_FILE =
    "Failed to allocate memory for file menu.\n";

static const gchar *TEXT_MENU_FILE_RPI_MENU = "File";
static const gchar *TEXT_MENU_FILE_SUBMENU_EXIT_RPI_MENU = "Exit";

GtkWidget *rpi_menu_file_new(GtkWidget **out_menu_item,
                             GtkWidget **out_exit_item) {
  GtkWidget *menu_file = gtk_menu_new();
  if (!GTK_IS_MENU(menu_file)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_FILE);
    return NULL;
  }

  GtkWidget *menu_file_item =
      gtk_menu_item_new_with_label(TEXT_MENU_FILE_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_file_item)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_FILE);
    gtk_widget_destroy(menu_file);
    return NULL;
  }

  GtkWidget *menu_file_submenu_exit =
      gtk_menu_item_new_with_label(TEXT_MENU_FILE_SUBMENU_EXIT_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_file_submenu_exit)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_FILE);
    gtk_widget_destroy(menu_file_item);
    gtk_widget_destroy(menu_file);
    return NULL;
  }

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), menu_file_submenu_exit);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_file_item), menu_file);

  if (out_menu_item) {
    *out_menu_item = menu_file_item;
  }
  if (out_exit_item) {
    *out_exit_item = menu_file_submenu_exit;
  }

  return menu_file;
}
