/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu_option.c
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
#include "rpi_menu_option.h"

static const gchar *const FAILED_MALLOC_RPI_MENU_OPTION =
    "Failed to allocate memory for option menu.\n";

static const gchar *TEXT_MENU_OPTION_RPI_MENU = "Option";
static const gchar *TEXT_MENU_OPTION_SUBMENU_CONNECT_RPI_MENU = "Connect";
static const gchar *TEXT_MENU_OPTION_SUBMENU_DISCONNECT_RPI_MENU = "Disconnect";

GtkWidget *rpi_menu_option_new(GtkWidget **out_menu_item,
                               GtkWidget **out_connect_item,
                               GtkWidget **out_disconnect_item) {
  GtkWidget *menu_option = gtk_menu_new();
  if (!GTK_IS_MENU(menu_option)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_OPTION);
    return NULL;
  }

  GtkWidget *menu_option_item =
      gtk_menu_item_new_with_label(TEXT_MENU_OPTION_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_option_item)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_OPTION);
    gtk_widget_destroy(menu_option);
    return NULL;
  }

  GtkWidget *menu_option_submenu_connect =
      gtk_menu_item_new_with_label(TEXT_MENU_OPTION_SUBMENU_CONNECT_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_option_submenu_connect)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_OPTION);
    gtk_widget_destroy(menu_option_item);
    gtk_widget_destroy(menu_option);
    return NULL;
  }

  GtkWidget *menu_option_submenu_disconnect = gtk_menu_item_new_with_label(
      TEXT_MENU_OPTION_SUBMENU_DISCONNECT_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_option_submenu_disconnect)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_OPTION);
    gtk_widget_destroy(menu_option_submenu_connect);
    gtk_widget_destroy(menu_option_item);
    gtk_widget_destroy(menu_option);
    return NULL;
  }

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_option),
                        menu_option_submenu_connect);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_option),
                        menu_option_submenu_disconnect);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_option_item), menu_option);

  if (out_menu_item) {
    *out_menu_item = menu_option_item;
  }
  if (out_connect_item) {
    *out_connect_item = menu_option_submenu_connect;
  }
  if (out_disconnect_item) {
    *out_disconnect_item = menu_option_submenu_disconnect;
  }

  return menu_option;
}
