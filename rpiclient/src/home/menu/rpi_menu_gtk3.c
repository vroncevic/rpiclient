/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu.c
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
#include "rpi_menu.h"
#include "../../rpi_config.h"
#include "file/rpi_menu_file.h"
#include "help/rpi_menu_help.h"
#include "option/rpi_menu_option.h"
#include "settings/rpi_menu_settings.h"

#if RPI_VERBOSE == 1
static const gchar *const CONNECT_SIGNAL_RPI_MENU =
    "Connect signal to submenu: %s.\n";
#endif

static const gchar *const FAILED_MALLOC_RPI_MENU =
    "Failed to allocate memory for menu bar.\n";

static const gchar *TEXT_MENU_FILE_SUBMENU_EXIT_RPI_MENU = "Exit";
static const gchar *TEXT_MENU_OPTION_SUBMENU_CONNECT_RPI_MENU = "Connect";
static const gchar *TEXT_MENU_OPTION_SUBMENU_DISCONNECT_RPI_MENU = "Disconnect";
static const gchar *TEXT_MENU_SETTINGS_SUBMENU_GENERAL_RPI_MENU = "General";
static const gchar *TEXT_MENU_SETTINGS_SUBMENU_NETWORK_RPI_MENU = "Network";
static const gchar *TEXT_MENU_HELP_SUBMENU_HELP_RPI_MENU = "Help";
static const gchar *TEXT_MENU_HELP_SUBMENU_ABOUT_RPI_MENU = "About";

//////////////////////////////////////////////////////////////////////////////
/// @brief Menu bar RPI complex widget
///   menu_bar - Gtk menu bar container widget
///   menu_file - Gtk menu widget for File
///   menu_file_item - Gtk menu item for File top-level menu
///   menu_file_submenu_exit - Gtk menu item for Exit
///   menu_option - Gtk menu widget for Option
///   menu_option_item - Gtk menu item for Option top-level menu
///   menu_option_submenu_connect - Gtk menu item for Connect
///   menu_option_submenu_disconnect - Gtk menu item for Disconnect
///   menu_settings - Gtk menu widget for Settings
///   menu_settings_item - Gtk menu item for Settings top-level menu
///   menu_settings_submenu_general - Gtk menu item for General settings
///   menu_settings_submenu_network - Gtk menu item for Network settings
///   menu_help - Gtk menu widget for Help
///   menu_help_item - Gtk menu item for Help top-level menu
///   menu_help_submenu_help - Gtk menu item for Help
///   menu_help_submenu_about - Gtk menu item for About
struct _RPIMenu {
  GtkWidget *menu_bar;
  GtkWidget *menu_file;
  GtkWidget *menu_file_item;
  GtkWidget *menu_file_submenu_exit;
  GtkWidget *menu_option;
  GtkWidget *menu_option_item;
  GtkWidget *menu_option_submenu_connect;
  GtkWidget *menu_option_submenu_disconnect;
  GtkWidget *menu_settings;
  GtkWidget *menu_settings_item;
  GtkWidget *menu_settings_submenu_general;
  GtkWidget *menu_settings_submenu_network;
  GtkWidget *menu_help;
  GtkWidget *menu_help_item;
  GtkWidget *menu_help_submenu_help;
  GtkWidget *menu_help_submenu_about;
};

RPIMenu *new_rpi_menu(void) {
  RPIMenu *instance = g_malloc(sizeof(RPIMenu));
  if (!instance) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU);
    return NULL;
  }

  instance->menu_bar = gtk_menu_bar_new();
  if (!GTK_IS_MENU_BAR(instance->menu_bar)) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU);
    g_free(instance);
    return NULL;
  }

  instance->menu_file = rpi_menu_file_new(&instance->menu_file_item,
                                          &instance->menu_file_submenu_exit);
  if (!instance->menu_file) {
    destroy_rpi_menu(instance);
    return NULL;
  }
  gtk_menu_shell_append(GTK_MENU_SHELL(instance->menu_bar),
                        instance->menu_file_item);

  instance->menu_option =
      rpi_menu_option_new(&instance->menu_option_item,
                          &instance->menu_option_submenu_connect,
                          &instance->menu_option_submenu_disconnect);
  if (!instance->menu_option) {
    destroy_rpi_menu(instance);
    return NULL;
  }
  gtk_menu_shell_append(GTK_MENU_SHELL(instance->menu_bar),
                        instance->menu_option_item);

  instance->menu_settings =
      rpi_menu_settings_new(&instance->menu_settings_item,
                            &instance->menu_settings_submenu_general,
                            &instance->menu_settings_submenu_network);
  if (!instance->menu_settings) {
    destroy_rpi_menu(instance);
    return NULL;
  }
  gtk_menu_shell_append(GTK_MENU_SHELL(instance->menu_bar),
                        instance->menu_settings_item);

  instance->menu_help = rpi_menu_help_new(&instance->menu_help_item,
                                          &instance->menu_help_submenu_help,
                                          &instance->menu_help_submenu_about);
  if (!instance->menu_help) {
    destroy_rpi_menu(instance);
    return NULL;
  }
  gtk_menu_shell_append(GTK_MENU_SHELL(instance->menu_bar),
                        instance->menu_help_item);

  return instance;
}

void show_rpi_menu(RPIMenu *instance) {
  if (instance && GTK_IS_MENU_BAR(instance->menu_bar)) {
    gtk_widget_show_all(instance->menu_bar);
  }
}

void hide_rpi_menu(RPIMenu *instance) {
  if (instance && GTK_IS_MENU_BAR(instance->menu_bar)) {
    gtk_widget_hide(instance->menu_bar);
  }
}

GtkWidget *get_menu_bar_from_rpi_menu(RPIMenu *instance) {
  if (instance && GTK_IS_MENU_BAR(instance->menu_bar)) {
    return instance->menu_bar;
  }
  return NULL;
}

void rpi_menu_connect_signal(RPIMenu *instance,
                             void (*on_callback)(GtkWidget *, gpointer),
                             const gchar *submenu) {
  if (!instance || !submenu || !on_callback) {
    return;
  }

  if (g_strcmp0(TEXT_MENU_FILE_SUBMENU_EXIT_RPI_MENU, submenu) == 0) {
    g_signal_connect(G_OBJECT(instance->menu_file_submenu_exit), "activate",
                     G_CALLBACK(on_callback), NULL);
  } else if (g_strcmp0(TEXT_MENU_OPTION_SUBMENU_CONNECT_RPI_MENU, submenu) ==
             0) {
    g_signal_connect(G_OBJECT(instance->menu_option_submenu_connect),
                     "activate", G_CALLBACK(on_callback), NULL);
  } else if (g_strcmp0(TEXT_MENU_OPTION_SUBMENU_DISCONNECT_RPI_MENU, submenu) ==
             0) {
    g_signal_connect(G_OBJECT(instance->menu_option_submenu_disconnect),
                     "activate", G_CALLBACK(on_callback), NULL);
  } else if (g_strcmp0(TEXT_MENU_SETTINGS_SUBMENU_GENERAL_RPI_MENU, submenu) ==
             0) {
    g_signal_connect(G_OBJECT(instance->menu_settings_submenu_general),
                     "activate", G_CALLBACK(on_callback), NULL);
  } else if (g_strcmp0(TEXT_MENU_SETTINGS_SUBMENU_NETWORK_RPI_MENU, submenu) ==
             0) {
    g_signal_connect(G_OBJECT(instance->menu_settings_submenu_network),
                     "activate", G_CALLBACK(on_callback), NULL);
  } else if (g_strcmp0(TEXT_MENU_HELP_SUBMENU_HELP_RPI_MENU, submenu) == 0) {
    g_signal_connect(G_OBJECT(instance->menu_help_submenu_help), "activate",
                     G_CALLBACK(on_callback), NULL);
  } else if (g_strcmp0(TEXT_MENU_HELP_SUBMENU_ABOUT_RPI_MENU, submenu) == 0) {
    g_signal_connect(G_OBJECT(instance->menu_help_submenu_about), "activate",
                     G_CALLBACK(on_callback), NULL);
  }

#if RPI_VERBOSE == 1
  g_debug(CONNECT_SIGNAL_RPI_MENU, submenu);
#endif
}

GActionGroup *get_action_group_from_rpi_menu(RPIMenu *instance) {
  (void)instance;
  return NULL;
}

void destroy_rpi_menu(RPIMenu *instance) {
  if (instance) {
    if (GTK_IS_MENU_BAR(instance->menu_bar)) {
      gtk_widget_destroy(instance->menu_bar);
      instance->menu_bar = NULL;
    }
    g_free(instance);
  }
}
