/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu_help.c
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
#include "rpi_menu_help.h"

static const gchar *const FAILED_MALLOC_RPI_MENU_HELP =
    "Failed to allocate memory for help menu.\n";

static const gchar *TEXT_MENU_HELP_RPI_MENU = "Help";
static const gchar *TEXT_MENU_HELP_SUBMENU_HELP_RPI_MENU = "Help";
static const gchar *TEXT_MENU_HELP_SUBMENU_ABOUT_RPI_MENU = "About";

GtkWidget *rpi_menu_help_new(GtkWidget **out_menu_item,
                             GtkWidget **out_help_item,
                             GtkWidget **out_about_item) {
  GtkWidget *menu_help = gtk_menu_new();
  if (!GTK_IS_MENU(menu_help)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_HELP);
    return NULL;
  }

  GtkWidget *menu_help_item =
      gtk_menu_item_new_with_label(TEXT_MENU_HELP_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_help_item)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_HELP);
    gtk_widget_destroy(menu_help);
    return NULL;
  }

  GtkWidget *menu_help_submenu_help =
      gtk_menu_item_new_with_label(TEXT_MENU_HELP_SUBMENU_HELP_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_help_submenu_help)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_HELP);
    gtk_widget_destroy(menu_help_item);
    gtk_widget_destroy(menu_help);
    return NULL;
  }

  GtkWidget *menu_help_submenu_about =
      gtk_menu_item_new_with_label(TEXT_MENU_HELP_SUBMENU_ABOUT_RPI_MENU);
  if (!GTK_IS_MENU_ITEM(menu_help_submenu_about)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU_HELP);
    gtk_widget_destroy(menu_help_submenu_help);
    gtk_widget_destroy(menu_help_item);
    gtk_widget_destroy(menu_help);
    return NULL;
  }

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_help), menu_help_submenu_help);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_help), menu_help_submenu_about);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_help_item), menu_help);

  if (out_menu_item) {
    *out_menu_item = menu_help_item;
  }
  if (out_help_item) {
    *out_help_item = menu_help_submenu_help;
  }
  if (out_about_item) {
    *out_about_item = menu_help_submenu_about;
  }

  return menu_help;
}
