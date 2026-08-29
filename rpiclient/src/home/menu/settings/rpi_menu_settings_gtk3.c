/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu_settings.c
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
#include "rpi_menu_settings.h"

static const gchar *const FAILED_MALLOC_RPI_MENU_SETTINGS =
    "Failed to allocate memory for settings menu.\n";

static const gchar *TEXT_MENU_SETTINGS_RPI_MENU = "Settings";
static const gchar *TEXT_MENU_SETTINGS_SUBMENU_GENERAL_RPI_MENU = "General";
static const gchar *TEXT_MENU_SETTINGS_SUBMENU_NETWORK_RPI_MENU = "Network";

GtkWidget *rpi_menu_settings_new(GtkWidget **out_menu_item,
                                 GtkWidget **out_general_item,
                                 GtkWidget **out_network_item) {
  GtkWidget *menu_settings = gtk_menu_new();
  if (!GTK_IS_MENU(menu_settings)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_SETTINGS);
    return NULL;
  }

  GtkWidget *menu_settings_item =
      gtk_menu_item_new_with_label(TEXT_MENU_SETTINGS_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_settings_item)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_SETTINGS);
    gtk_widget_destroy(menu_settings);
    return NULL;
  }

  GtkWidget *menu_settings_submenu_general =
      gtk_menu_item_new_with_label(TEXT_MENU_SETTINGS_SUBMENU_GENERAL_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_settings_submenu_general)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_SETTINGS);
    gtk_widget_destroy(menu_settings_item);
    gtk_widget_destroy(menu_settings);
    return NULL;
  }

  GtkWidget *menu_settings_submenu_network =
      gtk_menu_item_new_with_label(TEXT_MENU_SETTINGS_SUBMENU_NETWORK_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_settings_submenu_network)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_SETTINGS);
    gtk_widget_destroy(menu_settings_submenu_general);
    gtk_widget_destroy(menu_settings_item);
    gtk_widget_destroy(menu_settings);
    return NULL;
  }

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_settings),
                        menu_settings_submenu_general);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_settings),
                        menu_settings_submenu_network);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_settings_item), menu_settings);

  if (out_menu_item) {
    *out_menu_item = menu_settings_item;
  }
  if (out_general_item) {
    *out_general_item = menu_settings_submenu_general;
  }
  if (out_network_item) {
    *out_network_item = menu_settings_submenu_network;
  }

  return menu_settings;
}
