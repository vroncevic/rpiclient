/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
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
#include "about/rpi_about_dialog.h"
#include "exit/rpi_exit_dialog.h"
#include "help/rpi_help_window.h"
#include "home/rpi_home.h"
#include "home/menu/rpi_menu.h"
#include "network/rpi_network.h"
#include "resource/rpi_resource.h"
#include "rpi_config.h"
#include "settings/rpi_settings_config.h"
#include "settings/ui/rpi_settings_general_window.h"
#include "settings/ui/rpi_settings_network_window.h"

#if RPI_VERBOSE == 1
static const gchar *const ON_EXIT_RPI_CLIENT =
    "On exit RPIClient main thread.\n";
static const gchar *const ON_EXIT_CLOSE_RPI_CLIENT =
    "On exit RPIClient quit from main thread.\n";
static const gchar *const ON_CONNECT_RPI_CLIENT =
    "On connect RPIClient main thread.\n";
static const gchar *const ON_DISCONNECT_RPI_CLIENT =
    "On disconnect RPIClient main thread.\n";
static const gchar *const ON_SETTINGS_GENERAL_RPI_CLIENT =
    "On settings general RPIClient main thread.\n";
static const gchar *const ON_SETTINGS_NETWORK_RPI_CLIENT =
    "On settings network RPIClient main thread.\n";
static const gchar *const ON_HELP_RPI_CLIENT =
    "On help RPIClient main thread.\n";
static const gchar *const ON_ABOUT_RPI_CLIENT =
    "On about RPIClient main thread.\n";
#endif

static const gchar *const FAILED_RPI_CLIENT =
    "Runtime GTK version does not match compile-time version!\n";

static RPIHome *app = NULL;

static gboolean on_app_exit(GtkWidget *widget, GdkEvent *event, gpointer data);
static void on_menu_exit(GtkWidget *widget, gpointer data);
static void on_option_connect(GtkWidget *widget, gpointer data);
static void on_option_disconnect(GtkWidget *widget, gpointer data);
static void on_show_settings_general(GtkWidget *widget, gpointer data);
static void on_show_settings_network(GtkWidget *widget, gpointer data);
static void on_show_help(GtkWidget *widget, gpointer data);
static void on_show_about(GtkWidget *widget, gpointer data);
static void on_network_status_received(gint channel_id, gint value,
                                       gpointer user_data);
static void load_css(void);

static void on_network_status_received(gint channel_id, gint value,
                                       gpointer user_data) {
  (void)user_data;
  if (app) {
    rpi_home_update_channel_status(app, channel_id, value);
  }
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  if (gtk_get_major_version() != GTK_MAJOR_VERSION) {
    g_critical("%s", FAILED_RPI_CLIENT);
    return 1;
  }

  load_css();

  app = new_rpi_home();
  show_rpi_home(app);

  rpi_network_set_status_callback(on_network_status_received, NULL);

  g_signal_connect(G_OBJECT(get_window_from_rpi_home(app)), "delete_event",
                   G_CALLBACK(on_app_exit), NULL);
  rpi_menu_connect_signal(get_menu_bar_from_rpi_home(app), on_menu_exit,
                          "Exit");
  rpi_menu_connect_signal(get_menu_bar_from_rpi_home(app), on_option_connect,
                          "Connect");
  rpi_menu_connect_signal(get_menu_bar_from_rpi_home(app), on_option_disconnect,
                          "Disconnect");
  rpi_menu_connect_signal(get_menu_bar_from_rpi_home(app),
                          on_show_settings_general, "General");
  rpi_menu_connect_signal(get_menu_bar_from_rpi_home(app),
                          on_show_settings_network, "Network");
  rpi_menu_connect_signal(get_menu_bar_from_rpi_home(app), on_show_help,
                          "Help");
  rpi_menu_connect_signal(get_menu_bar_from_rpi_home(app), on_show_about,
                          "About");

  gtk_main();

  return 0;
}

static gboolean on_app_exit(GtkWidget *widget, GdkEvent *event, gpointer data) {
  (void)widget;
  (void)event;
  (void)data;
#if RPI_VERBOSE == 1
  g_debug(ON_EXIT_RPI_CLIENT);
#endif

  ExitDialog *exit_dialog = new_exit_dialog(get_window_from_rpi_home(app));
  gint exit_code = show_exit_dialog(exit_dialog);

  if (exit_code == CLOSE_ON_EXIT_DIALOG) {
    rpi_network_disconnect();
    destroy_exit_dialog(exit_dialog);
    exit_dialog = NULL;
    destroy_rpi_home(app);
    app = NULL;

#if RPI_VERBOSE == 1
    g_debug(ON_EXIT_CLOSE_RPI_CLIENT);
#endif

    gtk_main_quit();
    return FALSE;
  }

  destroy_exit_dialog(exit_dialog);
  exit_dialog = NULL;
  return TRUE;
}

static void on_menu_exit(GtkWidget *widget, gpointer data) {
  (void)widget;
  (void)data;
  gtk_window_close(GTK_WINDOW(get_window_from_rpi_home(app)));
}

static void on_option_connect(GtkWidget *widget, gpointer data) {
  (void)widget;
  (void)data;
#if RPI_VERBOSE == 1
  g_debug(ON_CONNECT_RPI_CLIENT);
#endif

  SettingsConfig *config = settings_read();
  const gchar *ip = "127.0.0.1";
  gint port = 8000;
  gchar *cfg_ip = NULL;
  gchar *cfg_port = NULL;

  if (config) {
    cfg_ip = get_server_ip_address_from_settings(config);
    cfg_port = get_server_port_number_from_settings(config);
    if (cfg_ip && strlen(cfg_ip) > 0) {
      ip = cfg_ip;
    }
    if (cfg_port && strlen(cfg_port) > 0) {
      port = atoi(cfg_port);
    }
  }

  gboolean res = rpi_network_connect(ip, port);
  if (!res) {
    g_warning("Network: Could not connect to server at %s:%d\n", ip, port);
  }

  if (config) {
    settings_free(config);
  }
}

static void on_option_disconnect(GtkWidget *widget, gpointer data) {
  (void)widget;
  (void)data;
#if RPI_VERBOSE == 1
  g_debug(ON_DISCONNECT_RPI_CLIENT);
#endif

  rpi_network_disconnect();
}

static void on_show_settings_general(GtkWidget *widget, gpointer data) {
#if RPI_VERBOSE == 1
  g_debug(ON_SETTINGS_GENERAL_RPI_CLIENT);
#endif

  SettingsGeneralWindow *settings_general_window =
      new_settings_general_window();

  if (settings_general_window) {
    show_settings_general_window(settings_general_window);
  }
}

static void on_show_settings_network(GtkWidget *widget, gpointer data) {
#if RPI_VERBOSE == 1
  g_debug(ON_SETTINGS_NETWORK_RPI_CLIENT);
#endif

  SettingsNetworkWindow *settings_network_window =
      new_settings_network_window();

  if (settings_network_window) {
    show_settings_network_window(settings_network_window);
  }
}

static void on_show_help(GtkWidget *widget, gpointer data) {
#if RPI_VERBOSE == 1
  g_debug(ON_HELP_RPI_CLIENT);
#endif

  HelpWindow *help_window = new_help_window();

  if (help_window) {
    show_help_window(help_window);
  }
}

static void on_show_about(GtkWidget *widget, gpointer data) {
#if RPI_VERBOSE == 1
  g_debug(ON_ABOUT_RPI_CLIENT);
#endif

  AboutDialog *about_dialog = new_about_dialog();

  if (about_dialog) {
    show_about_dialog(about_dialog);
  }
}

static void load_css(void) {
  GtkCssProvider *provider = gtk_css_provider_new();

  gtk_css_provider_load_from_resource(provider, "/org/rpiclient/gtk/style.css");
  gtk_style_context_add_provider_for_screen(
      gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  g_object_unref(provider);
}
