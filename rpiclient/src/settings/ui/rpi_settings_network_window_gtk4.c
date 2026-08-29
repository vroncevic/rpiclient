/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_network_window_gtk4.c
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
#include "rpi_settings_network_window.h"
#include "../../resource/rpi_resource.h"
#include "../../rpi_config.h"
#include "../rpi_settings.h"
#include "../rpi_settings_config.h"

#if RPI_VERBOSE == 1
static const gchar *const ICON_FILE_PATH_NETWORK_WINDOW =
    "Icon file path: %s.\n";
#endif

static const gchar *const FAILED_MALLOC_SETTINGS_NETWORK_WINDOW =
    "Failed to allocate memory for settings network window.\n";
static const gchar *const FAILED_PIXBUF_SETTINGS_NETWORK_WINDOW =
    "Failed to create pixbuf from settings network icon.\n";
static const gchar *const FAILED_RESOURCE_SETTINGS_NETWORK_WINDOW =
    "Failed to get resource path for settings network icon.\n";

static const gchar *TITLE_SETTINGS_NETWORK_WINDOW = "Settings Network";
static const gchar *ICON_SETTINGS_NETWORK_WINDOW = "icon.png";
static const gint WIDTH_SETTINGS_NETWORK_WINDOW = 350;
static const gint HEIGHT_SETTINGS_NETWORK_WINDOW = 320;
static const gchar *TEXT_FRAME_CONTORL_PROMPT_SETTINGS_NETWORK_WINDOW =
    "Prompt on Connect";
static const gchar *TEXT_CHECK_BUTTON_CONTROL_PROMPT_SETTINGS_NETWORK_WINDOW =
    "Prompt for IP address and Port?";
static const gchar *TEXT_FRAME_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW =
    "IP Address";
static const gint MAX_LENGTH_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW = 15;
static const gchar *TEXT_FRAME_ENTRY_PORT_SETTINGS_NETWORK_WINDOW = "Port";
static const gint MAX_LENGTH_ENTRY_PORT_SETTINGS_NETWORK_WINDOW = 5;
static const gchar *TEXT_BUTTON_OK_HBOX_SETTINGS_NETWORK_WINDOW = "OK";
static const gchar *TEXT_BUTTON_CANCEL_HBOX_SETTINGS_NETWORK_WINDOW = "Cancel";

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings network window complex widget for GTK4
struct _SettingsNetworkWindow {
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *frame_control_prompt;
  GtkWidget *check_button_control_prompt;
  GtkWidget *frame_entry_address;
  GtkWidget *entry_address;
  GtkWidget *frame_entry_port;
  GtkWidget *entry_port;
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
  SettingsNetworkWindow *instance = (SettingsNetworkWindow *)data;
  if (instance) {
    gboolean is_prompt = gtk_check_button_get_active(
        GTK_CHECK_BUTTON(instance->check_button_control_prompt));
    const gchar *addr_str =
        gtk_editable_get_text(GTK_EDITABLE(instance->entry_address));
    const gchar *port_str =
        gtk_editable_get_text(GTK_EDITABLE(instance->entry_port));

    set_prompt_enabled_settings(instance->settings, is_prompt);
    set_server_ip_address_settings(instance->settings, addr_str);
    set_server_port_number_settings(instance->settings, port_str);

    guint status = settings_write(instance->settings);
    if (status == FAILED_IO_SETTINGS_CONFIGURATION) {
      return;
    }
    destroy_settings_network_window(instance);
  }
}

static void on_button_cancel_clicked(GtkWidget *widget, gpointer data) {
  (void)widget;
  SettingsNetworkWindow *instance = (SettingsNetworkWindow *)data;
  if (instance) {
    destroy_settings_network_window(instance);
  }
}

static gboolean on_window_close_request(GtkWindow *window, gpointer data) {
  (void)window;
  SettingsNetworkWindow *instance = (SettingsNetworkWindow *)data;
  if (instance) {
    destroy_settings_network_window(instance);
  }
  return TRUE;
}

SettingsNetworkWindow *new_settings_network_window(void) {
  SettingsNetworkWindow *instance = g_malloc0(sizeof(SettingsNetworkWindow));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
    return NULL;
  }

  instance->settings = settings_read();
  if (!instance->settings) {
    g_critical("%s", FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
    g_free(instance);
    return NULL;
  }

  instance->window = gtk_window_new();
  if (!GTK_IS_WINDOW(instance->window)) {
    g_critical("%s", FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
    destroy_settings_network_window(instance);
    return NULL;
  }

  gtk_window_set_default_size(GTK_WINDOW(instance->window),
                              WIDTH_SETTINGS_NETWORK_WINDOW,
                              HEIGHT_SETTINGS_NETWORK_WINDOW);
  gtk_window_set_title(GTK_WINDOW(instance->window),
                       TITLE_SETTINGS_NETWORK_WINDOW);
  gtk_window_set_resizable(GTK_WINDOW(instance->window), FALSE);

  gchar *icon_file_path =
      rpi_get_resource_file_path(ICON_SETTINGS_NETWORK_WINDOW);
  if (icon_file_path) {
    GdkPixbuf *pixbuf = rpi_cpixbuf(icon_file_path);
    if (GDK_IS_PIXBUF(pixbuf)) {
      gchar *themed_icon_name = g_path_get_basename(icon_file_path);
      gtk_window_set_icon_name(GTK_WINDOW(instance->window), themed_icon_name);
      g_free(themed_icon_name);
#if RPI_VERBOSE == 1
      g_debug(ICON_FILE_PATH_NETWORK_WINDOW, icon_file_path);
#endif
      g_object_unref(pixbuf);
    } else {
      g_critical("%s", FAILED_PIXBUF_SETTINGS_NETWORK_WINDOW);
    }
    g_free(icon_file_path);
  } else {
    g_critical("%s", FAILED_RESOURCE_SETTINGS_NETWORK_WINDOW);
  }

  instance->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  gtk_widget_set_margin_start(instance->vbox, 16);
  gtk_widget_set_margin_end(instance->vbox, 16);
  gtk_widget_set_margin_top(instance->vbox, 16);
  gtk_widget_set_margin_bottom(instance->vbox, 16);

  // Prompt frame
  instance->frame_control_prompt =
      gtk_frame_new(TEXT_FRAME_CONTORL_PROMPT_SETTINGS_NETWORK_WINDOW);
  instance->check_button_control_prompt = gtk_check_button_new_with_label(
      TEXT_CHECK_BUTTON_CONTROL_PROMPT_SETTINGS_NETWORK_WINDOW);
  gboolean is_prompt_enabled = is_prompt_enabled_settings(instance->settings);
  gtk_check_button_set_active(
      GTK_CHECK_BUTTON(instance->check_button_control_prompt),
      is_prompt_enabled);
  gtk_widget_set_margin_start(instance->check_button_control_prompt, 8);
  gtk_widget_set_margin_end(instance->check_button_control_prompt, 8);
  gtk_widget_set_margin_top(instance->check_button_control_prompt, 8);
  gtk_widget_set_margin_bottom(instance->check_button_control_prompt, 8);
  gtk_frame_set_child(GTK_FRAME(instance->frame_control_prompt),
                      instance->check_button_control_prompt);
  gtk_box_append(GTK_BOX(instance->vbox), instance->frame_control_prompt);

  // Address frame
  instance->frame_entry_address =
      gtk_frame_new(TEXT_FRAME_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW);
  instance->entry_address = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(instance->entry_address),
                           MAX_LENGTH_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW);
  gchar *ip_address = get_server_ip_address_from_settings(instance->settings);
  if (ip_address) {
    gtk_editable_set_text(GTK_EDITABLE(instance->entry_address), ip_address);
    g_free(ip_address);
  }
  gtk_widget_set_margin_start(instance->entry_address, 8);
  gtk_widget_set_margin_end(instance->entry_address, 8);
  gtk_widget_set_margin_top(instance->entry_address, 8);
  gtk_widget_set_margin_bottom(instance->entry_address, 8);
  gtk_frame_set_child(GTK_FRAME(instance->frame_entry_address),
                      instance->entry_address);
  gtk_box_append(GTK_BOX(instance->vbox), instance->frame_entry_address);

  // Port frame
  instance->frame_entry_port =
      gtk_frame_new(TEXT_FRAME_ENTRY_PORT_SETTINGS_NETWORK_WINDOW);
  instance->entry_port = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(instance->entry_port),
                           MAX_LENGTH_ENTRY_PORT_SETTINGS_NETWORK_WINDOW);
  gchar *port_number = get_server_port_number_from_settings(instance->settings);
  if (port_number) {
    gtk_editable_set_text(GTK_EDITABLE(instance->entry_port), port_number);
    g_free(port_number);
  }
  gtk_widget_set_margin_start(instance->entry_port, 8);
  gtk_widget_set_margin_end(instance->entry_port, 8);
  gtk_widget_set_margin_top(instance->entry_port, 8);
  gtk_widget_set_margin_bottom(instance->entry_port, 8);
  gtk_frame_set_child(GTK_FRAME(instance->frame_entry_port),
                      instance->entry_port);
  gtk_box_append(GTK_BOX(instance->vbox), instance->frame_entry_port);

  // Buttons
  instance->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_widget_set_halign(instance->hbox, GTK_ALIGN_END);

  instance->button_ok =
      gtk_button_new_with_label(TEXT_BUTTON_OK_HBOX_SETTINGS_NETWORK_WINDOW);
  instance->button_cancel = gtk_button_new_with_label(
      TEXT_BUTTON_CANCEL_HBOX_SETTINGS_NETWORK_WINDOW);

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

void show_settings_network_window(SettingsNetworkWindow *instance) {
  if (instance && instance->window) {
    gtk_window_present(GTK_WINDOW(instance->window));
  }
}

void hide_settings_network_window(SettingsNetworkWindow *instance) {
  if (instance && instance->window) {
    gtk_widget_set_visible(instance->window, FALSE);
  }
}

void destroy_settings_network_window(SettingsNetworkWindow *instance) {
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
