/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_general_window_gtk4.c
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
#include "rpi_settings_general_window.h"
#include "../../resource/rpi_resource.h"
#include "../../rpi_config.h"
#include "../rpi_settings.h"
#include "../rpi_settings_config.h"

#if RPI_VERBOSE == 1
static const gchar *const ICON_FILE_PATH_GENERAL_WINDOW =
    "Icon file path: %s.\n";
#endif

static const gchar *const FAILED_MALLOC_SETTINGS_GENERAL_WINDOW =
    "Failed to allocate memory for settings general window.\n";
static const gchar *const FAILED_PIXBUF_SETTINGS_GENERAL_WINDOW =
    "Failed to create pixbuf from settings general icon.\n";
static const gchar *const FAILED_RESOURCE_SETTINGS_GENERAL_WINDOW =
    "Failed to get resource path for settings general icon.\n";

static const gchar *TITLE_SETTINGS_GENERAL_WINDOW = "Settings General";
static const gchar *ICON_SETTINGS_GENERAL_WINDOW = "icon.png";
static const gint WIDTH_SETTINGS_GENERAL_WINDOW = 350;
static const gint HEIGHT_SETTINGS_GENERAL_WINDOW = 200;
static const gchar *TEXT_FRAME_CONTORL_EXIT_SETTINGS_NETWORK_WINDOW =
    "Exit from rpiclient";
static const gchar *TEXT_CHECK_BUTTON_CONTROL_EXIT_SETTINGS_NETWORK_WINDOW =
    "Display Exit Dialog on Quit?";
static const gchar *TEXT_BUTTON_OK_HBOX_SETTINGS_GENERAL_WINDOW = "OK";
static const gchar *TEXT_BUTTON_CANCEL_HBOX_SETTINGS_GENERAL_WINDOW = "Cancel";

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings general window complex widget for GTK4
struct _SettingsGeneralWindow {
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *grid;
  GtkWidget *frame_control_exit;
  GtkWidget *check_button_control_exit;
  GtkWidget *hbox;
  GtkWidget *button_ok;
  GtkWidget *button_cancel;
  SettingsConfig *settings;
};

static void on_button_ok_clicked(GtkWidget *widget, gpointer data);
static void on_button_cancel_clicked(GtkWidget *widget, gpointer data);
static gboolean on_window_close_request(GtkWindow *window, gpointer data);

static void on_button_ok_clicked(GtkWidget *widget, gpointer data) {
  (void)widget;
  SettingsGeneralWindow *instance = (SettingsGeneralWindow *)data;
  if (instance) {
    gboolean is_active = gtk_check_button_get_active(
        GTK_CHECK_BUTTON(instance->check_button_control_exit));
    set_exit_enabled_settings(instance->settings, is_active);
    guint status = settings_write(instance->settings);
    if (status == FAILED_IO_SETTINGS_CONFIGURATION) {
      return;
    }
    destroy_settings_general_window(instance);
  }
}

static void on_button_cancel_clicked(GtkWidget *widget, gpointer data) {
  (void)widget;
  SettingsGeneralWindow *instance = (SettingsGeneralWindow *)data;
  if (instance) {
    destroy_settings_general_window(instance);
  }
}

static gboolean on_window_close_request(GtkWindow *window, gpointer data) {
  (void)window;
  SettingsGeneralWindow *instance = (SettingsGeneralWindow *)data;
  if (instance) {
    destroy_settings_general_window(instance);
  }
  return TRUE;
}

SettingsGeneralWindow *new_settings_general_window(void) {
  SettingsGeneralWindow *instance = g_malloc0(sizeof(SettingsGeneralWindow));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
    return NULL;
  }

  instance->settings = settings_read();
  if (!instance->settings) {
    g_critical("%s", FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
    g_free(instance);
    return NULL;
  }

  instance->window = gtk_window_new();
  if (!GTK_IS_WINDOW(instance->window)) {
    g_critical("%s", FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
    destroy_settings_general_window(instance);
    return NULL;
  }

  gtk_window_set_default_size(GTK_WINDOW(instance->window),
                              WIDTH_SETTINGS_GENERAL_WINDOW,
                              HEIGHT_SETTINGS_GENERAL_WINDOW);
  gtk_window_set_title(GTK_WINDOW(instance->window),
                       TITLE_SETTINGS_GENERAL_WINDOW);
  gtk_window_set_resizable(GTK_WINDOW(instance->window), FALSE);

  gchar *icon_file_path =
      rpi_get_resource_file_path(ICON_SETTINGS_GENERAL_WINDOW);
  if (icon_file_path) {
    GdkPixbuf *pixbuf = rpi_cpixbuf(icon_file_path);
    if (GDK_IS_PIXBUF(pixbuf)) {
      gchar *themed_icon_name = g_path_get_basename(icon_file_path);
      gtk_window_set_icon_name(GTK_WINDOW(instance->window), themed_icon_name);
      g_free(themed_icon_name);
#if RPI_VERBOSE == 1
      g_debug(ICON_FILE_PATH_GENERAL_WINDOW, icon_file_path);
#endif
      g_object_unref(pixbuf);
    } else {
      g_critical("%s", FAILED_PIXBUF_SETTINGS_GENERAL_WINDOW);
    }
    g_free(icon_file_path);
  } else {
    g_critical("%s", FAILED_RESOURCE_SETTINGS_GENERAL_WINDOW);
  }

  instance->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
  gtk_widget_set_margin_start(instance->vbox, 16);
  gtk_widget_set_margin_end(instance->vbox, 16);
  gtk_widget_set_margin_top(instance->vbox, 16);
  gtk_widget_set_margin_bottom(instance->vbox, 16);

  instance->frame_control_exit =
      gtk_frame_new(TEXT_FRAME_CONTORL_EXIT_SETTINGS_NETWORK_WINDOW);
  instance->check_button_control_exit = gtk_check_button_new_with_label(
      TEXT_CHECK_BUTTON_CONTROL_EXIT_SETTINGS_NETWORK_WINDOW);

  gboolean is_exit_enabled = is_exit_enabled_settings(instance->settings);
  gtk_check_button_set_active(
      GTK_CHECK_BUTTON(instance->check_button_control_exit), is_exit_enabled);

  gtk_widget_set_margin_start(instance->check_button_control_exit, 8);
  gtk_widget_set_margin_end(instance->check_button_control_exit, 8);
  gtk_widget_set_margin_top(instance->check_button_control_exit, 8);
  gtk_widget_set_margin_bottom(instance->check_button_control_exit, 8);
  gtk_frame_set_child(GTK_FRAME(instance->frame_control_exit),
                      instance->check_button_control_exit);

  gtk_box_append(GTK_BOX(instance->vbox), instance->frame_control_exit);

  instance->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_widget_set_halign(instance->hbox, GTK_ALIGN_END);

  instance->button_ok =
      gtk_button_new_with_label(TEXT_BUTTON_OK_HBOX_SETTINGS_GENERAL_WINDOW);
  instance->button_cancel = gtk_button_new_with_label(
      TEXT_BUTTON_CANCEL_HBOX_SETTINGS_GENERAL_WINDOW);

  gtk_widget_set_size_request(instance->button_ok, 80, 32);
  gtk_widget_set_size_request(instance->button_cancel, 80, 32);

  gtk_box_append(GTK_BOX(instance->hbox), instance->button_ok);
  gtk_box_append(GTK_BOX(instance->hbox), instance->button_cancel);
  gtk_box_append(GTK_BOX(instance->vbox), instance->hbox);

  gtk_window_set_child(GTK_WINDOW(instance->window), instance->vbox);

  g_signal_connect(instance->button_ok, "clicked",
                   G_CALLBACK(on_button_ok_clicked), instance);
  g_signal_connect(instance->button_cancel, "clicked",
                   G_CALLBACK(on_button_cancel_clicked), instance);
  g_signal_connect(instance->window, "close-request",
                   G_CALLBACK(on_window_close_request), instance);

  return instance;
}

void show_settings_general_window(SettingsGeneralWindow *instance) {
  if (instance && instance->window) {
    gtk_window_present(GTK_WINDOW(instance->window));
  }
}

void hide_settings_general_window(SettingsGeneralWindow *instance) {
  if (instance && instance->window) {
    gtk_widget_set_visible(instance->window, FALSE);
  }
}

void destroy_settings_general_window(SettingsGeneralWindow *instance) {
  if (instance) {
    if (instance->window) {
      gtk_window_destroy(GTK_WINDOW(instance->window));
      instance->window = NULL;
    }

    if (instance->settings) {
      settings_free(instance->settings);
      instance->settings = NULL;
    }

    g_free(instance);
  }
}
