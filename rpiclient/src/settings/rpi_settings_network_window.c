/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_network_window.c
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
#include "../rpi_config.h"
#include "../resource/rpi_resource.h"
#include "rpi_settings.h"
#include "rpi_settings_config.h"
#include "rpi_settings_network_window.h"

#if RPI_VERBOSE == 1
#define ICON_FILE_PATH_NETWORK_WINDOW "Icon file path: %s.\n"
#endif

#define FAILED_MALLOC_SETTINGS_NETWORK_WINDOW "Failed to allocate memory for settings network window.\n"
#define FAILED_PIXBUF_SETTINGS_NETWORK_WINDOW "Failed to create pixbuf from settings network icon.\n"
#define FAILED_RESOURCE_SETTINGS_NETWORK_WINDOW "Failed to get resource path for settings network icon.\n"

static const gchar* TITLE_SETTINGS_NETWORK_WINDOW = "Settings Network";
static const gchar* ICON_SETTINGS_NETWORK_WINDOW = "icon.png";
static const gint WIDTH_SETTINGS_NETWORK_WINDOW = 270;
static const gint HEIGHT_SETTINGS_NETWORK_WINDOW = 250;
static const gint CONTAINER_BORDER_WIDTH_SETTINGS_NETWORK_WINDOW = 10;
static const gint SPACING_VBOX_SETTINGS_NETWORK_WINDOW = 20;
static const gint TABLE_ROW_SPACINGS_TABLE_SETTINGS_NETWORK_WINDOW = 10;
static const gint TABLE_COL_SPACINGS_TABLE_SETTINGS_NETWORK_WINDOW = 10;
static const gchar* TEXT_FRAME_CONTORL_PROMPT_SETTINGS_NETWORK_WINDOW = "Prompt settings";
static const gchar* TEXT_CHECK_BUTTON_CONTROL_PROMPT_SETTINGS_NETWORK_WINDOW = "Enable prompt to be visible?";
static const gchar* TEXT_FRAME_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW = "Server IP Address";
static const gint MAX_LENGTH_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW = 50;
static const gint NEW_TEXT_LENGTH_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW = (-1);
static const gint START_POSITION_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW = 0;
static const gchar* TEXT_FRAME_ENTRY_PORT_SETTINGS_NETWORK_WINDOW = "Server Port Number";
static const gint MAX_LENGTH_ENTRY_PORT_SETTINGS_NETWORK_WINDOW = 50;
static const gint NEW_TEXT_LENGTH_ENTRY_PORT_SETTINGS_NETWORK_WINDOW = (-1);
static const gint START_POSITION_ENTRY_PORT_SETTINGS_NETWORK_WINDOW = 0;
static const gchar* TEXT_BUTTON_OK_HBOX_SETTINGS_NETWORK_WINDOW = "OK";
static const gchar* TEXT_BUTTON_CANCEL_HBOX_SETTINGS_NETWORK_WINDOW = "Cancel";
static const gint WIDTH_BUTTON_HBOX_SETTINGS_NETWORK_WINDOW = 70;
static const gint HEIGHT_BUTTON_HBOX_SETTINGS_NETWORK_WINDOW = 30;
static const gint SPACING_HBOX_SETTINGS_NETWORK_WINDOW = 3;

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings network window complex widget
///   window - Gtk window widget
///   vbox - Gtk widget for vertical box
///   table - Gtk widget for table
///   frame_control_prompt - Gtk widget for frame
///   check_button_control_prompt - Gtk widget for check box
///   frame_entry_address - Gtk widget for frame
///   entry_address - Gtk widget for entry
///   cur_pos_address - Cursor position
///   frame_entry_port - Gtk widget for frame
///   entry_port - Gtk widget for entry
///   cur_pos_port - Cursor position for entry
///   hbox - Gtk widget for horizontal alignment
///   button_ok - Gtk widget for ok action
///   button_cancel - Gtk widget for cancel action
///   settings - Custom structure for keeping settings
struct _SettingsNetworkWindow
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *table;
    GtkWidget *frame_control_prompt;
    GtkWidget *check_button_control_prompt;
    GtkWidget *frame_entry_address;
    GtkWidget *entry_address;
    gint cur_pos_address;
    GtkWidget *frame_entry_port;
    GtkWidget *entry_port;
    gint cur_pos_port;
    GtkWidget *hbox;
    GtkWidget *button_ok;
    GtkWidget *button_cancel;
    SettingsConfig* settings;
};

static void on_button_ok_clicked(GtkWidget *widget, SettingsNetworkWindow *instance);
static void on_button_cancel_clicked(GtkWidget *widget, SettingsNetworkWindow *instance);
static void on_window_destroy(GtkWidget *widget, gpointer data);

SettingsNetworkWindow *new_settings_network_window(void)
{
    SettingsNetworkWindow *instance = g_malloc(sizeof(SettingsNetworkWindow));

    if (!instance)
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        return NULL;
    }

    instance->settings = settings_read();

    if (!instance->settings)
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    instance->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    if (!GTK_IS_WINDOW(instance->window))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_window_set_position(GTK_WINDOW(instance->window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(
        GTK_WINDOW(instance->window), WIDTH_SETTINGS_NETWORK_WINDOW, HEIGHT_SETTINGS_NETWORK_WINDOW
    );
    gtk_window_set_title(GTK_WINDOW(instance->window), TITLE_SETTINGS_NETWORK_WINDOW);
    gchar *icon_file_path = rpi_get_resource_file_path(ICON_SETTINGS_NETWORK_WINDOW);

    if (icon_file_path)
    {
        GdkPixbuf *pixbuf = rpi_cpixbuf(icon_file_path);

        if (GDK_IS_PIXBUF(pixbuf))
        {
            gtk_window_set_icon(GTK_WINDOW(instance->window), pixbuf);

#if RPI_VERBOSE == 1
            g_debug(ICON_FILE_PATH_NETWORK_WINDOW, icon_file_path);
#endif

            g_object_unref(pixbuf);
            pixbuf = NULL;
        }
        else
        {
            g_critical(FAILED_PIXBUF_SETTINGS_NETWORK_WINDOW);
            pixbuf = NULL;
        }

        g_free(icon_file_path);
        icon_file_path = NULL;
    }
    else
    {
        g_critical(FAILED_RESOURCE_SETTINGS_NETWORK_WINDOW);
        icon_file_path = NULL;
    }

    gtk_window_set_resizable(GTK_WINDOW(instance->window), FALSE);
    gtk_container_set_border_width(
        GTK_CONTAINER(instance->window), CONTAINER_BORDER_WIDTH_SETTINGS_NETWORK_WINDOW
    );
    instance->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, SPACING_VBOX_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_BOX(instance->vbox))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_container_add(GTK_CONTAINER(instance->window), GTK_WIDGET(instance->vbox));
    instance->table = gtk_grid_new();

    if (!GTK_IS_GRID(instance->table))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_grid_set_row_spacing(
        GTK_GRID(instance->table),
        TABLE_ROW_SPACINGS_TABLE_SETTINGS_NETWORK_WINDOW
    );
    gtk_grid_set_column_spacing(
        GTK_GRID(instance->table),
        TABLE_COL_SPACINGS_TABLE_SETTINGS_NETWORK_WINDOW
    );
    instance->frame_control_prompt = gtk_frame_new(TEXT_FRAME_CONTORL_PROMPT_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_FRAME(instance->frame_control_prompt))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_control_prompt), GTK_SHADOW_IN);
    instance->check_button_control_prompt = gtk_check_button_new_with_label(TEXT_CHECK_BUTTON_CONTROL_PROMPT_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_CHECK_BUTTON(instance->check_button_control_prompt))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gboolean is_prompt_enabled = is_prompt_enabled_settings(instance->settings);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(instance->check_button_control_prompt), is_prompt_enabled);
    gtk_container_add(
        GTK_CONTAINER(instance->frame_control_prompt), GTK_WIDGET(instance->check_button_control_prompt)
    );
    instance->frame_entry_address = gtk_frame_new(TEXT_FRAME_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_FRAME(instance->frame_entry_address))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_entry_address), GTK_SHADOW_IN);
    instance->entry_address = gtk_entry_new();

    if (!GTK_IS_ENTRY(instance->entry_address))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_entry_set_max_length(
        GTK_ENTRY(instance->entry_address), MAX_LENGTH_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW
    );
    gint len_address = g_utf8_strlen(gtk_entry_get_text(GTK_ENTRY(instance->entry_address)), -1);
    instance->cur_pos_address = len_address;
    gchar* ip_address_server = get_server_ip_address_from_settings(instance->settings);

    if (!ip_address_server)
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_editable_insert_text(
        GTK_EDITABLE(instance->entry_address),
        ip_address_server,
        NEW_TEXT_LENGTH_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW,
        &(instance->cur_pos_address)
    );
    g_free(ip_address_server);
    gtk_editable_select_region(
        GTK_EDITABLE(instance->entry_address),
        START_POSITION_ENTRY_ADDRESS_SETTINGS_NETWORK_WINDOW,
        len_address
    );
    gtk_container_add(GTK_CONTAINER(instance->frame_entry_address), GTK_WIDGET(instance->entry_address));
    instance->frame_entry_port = gtk_frame_new(TEXT_FRAME_ENTRY_PORT_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_FRAME(instance->frame_entry_port))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_entry_port), GTK_SHADOW_IN);
    instance->entry_port = gtk_entry_new();

    if (!GTK_IS_ENTRY(instance->entry_port))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_entry_set_max_length(
        GTK_ENTRY(instance->entry_port), MAX_LENGTH_ENTRY_PORT_SETTINGS_NETWORK_WINDOW
    );
    gint len_port = g_utf8_strlen(gtk_entry_get_text(GTK_ENTRY(instance->entry_port)), -1);
    instance->cur_pos_port = len_port;
    gchar* port_number_server = get_server_port_number_from_settings(instance->settings);

    if (!port_number_server)
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_editable_insert_text(
        GTK_EDITABLE(instance->entry_port),
        port_number_server,
        NEW_TEXT_LENGTH_ENTRY_PORT_SETTINGS_NETWORK_WINDOW,
        &(instance->cur_pos_port)
    );
    g_free(port_number_server);
    gtk_editable_select_region(
        GTK_EDITABLE(instance->entry_port),
        START_POSITION_ENTRY_PORT_SETTINGS_NETWORK_WINDOW,
        len_port
    );
    gtk_container_add(GTK_CONTAINER(instance->frame_entry_port), GTK_WIDGET(instance->entry_port));
    gtk_grid_attach(GTK_GRID(instance->table), GTK_WIDGET(instance->frame_control_prompt), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(instance->table), GTK_WIDGET(instance->frame_entry_address), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(instance->table), GTK_WIDGET(instance->frame_entry_port), 0, 2, 1, 1);
    gtk_box_pack_start(GTK_BOX(instance->vbox), GTK_WIDGET(instance->table), TRUE, TRUE, 0);
    instance->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, SPACING_HBOX_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_BOX(instance->hbox))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    instance->button_ok = gtk_button_new_with_label(TEXT_BUTTON_OK_HBOX_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_BUTTON(instance->button_ok))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    instance->button_cancel = gtk_button_new_with_label(TEXT_BUTTON_CANCEL_HBOX_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_BUTTON(instance->button_cancel))
    {
        g_critical(FAILED_MALLOC_SETTINGS_NETWORK_WINDOW);
        destroy_settings_network_window(instance);
        return NULL;
    }

    gtk_widget_set_size_request(
        GTK_WIDGET(instance->button_ok),
        WIDTH_BUTTON_HBOX_SETTINGS_NETWORK_WINDOW,
        HEIGHT_BUTTON_HBOX_SETTINGS_NETWORK_WINDOW
    );
    gtk_widget_set_size_request(
        GTK_WIDGET(instance->button_cancel),
        WIDTH_BUTTON_HBOX_SETTINGS_NETWORK_WINDOW,
        HEIGHT_BUTTON_HBOX_SETTINGS_NETWORK_WINDOW
    );
    gtk_container_add(GTK_CONTAINER(instance->hbox), GTK_WIDGET(instance->button_ok));
    gtk_container_add(GTK_CONTAINER(instance->hbox), GTK_WIDGET(instance->button_cancel));
    gtk_box_pack_start(GTK_BOX(instance->vbox), GTK_WIDGET(instance->hbox), FALSE, FALSE, 0);
    g_signal_connect(
        G_OBJECT(instance->window), "destroy", G_CALLBACK(on_window_destroy), instance
    );
    g_signal_connect(
        G_OBJECT(instance->button_ok), "clicked", G_CALLBACK(on_button_ok_clicked), instance
    );
    g_signal_connect(
        G_OBJECT(instance->button_cancel), "clicked", G_CALLBACK(on_button_cancel_clicked), instance
    );

    return instance;
}

void show_settings_network_window(SettingsNetworkWindow *instance)
{
    if (instance)
    {
        gboolean is_window = GTK_IS_WINDOW(instance->window);
        gboolean is_window_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->window));

        if (is_window && is_window_hidden)
        {
            gtk_widget_show_all(GTK_WIDGET(instance->window));
        }
    }
}

void hide_settings_network_window(SettingsNetworkWindow *instance)
{
    if (instance)
    {
        gboolean is_window = GTK_IS_WINDOW(instance->window);
        gboolean is_window_visible = gtk_widget_get_visible(GTK_WIDGET(instance->window));

        if (is_window && is_window_visible)
        {
            gtk_widget_hide(GTK_WIDGET(instance->window));
        }
    }
}

static void on_button_ok_clicked(GtkWidget *widget, SettingsNetworkWindow *instance)
{
    const gboolean no_prompt_state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(instance->check_button_control_prompt));
    set_prompt_enabled_settings(instance->settings, no_prompt_state);
    const gchar* ip_address = gtk_entry_get_text(GTK_ENTRY(instance->entry_address));
    set_server_ip_address_settings(instance->settings, ip_address);
    const gchar* port_number = gtk_entry_get_text(GTK_ENTRY(instance->entry_port));
    set_server_port_number_settings(instance->settings, port_number);
    guint status = settings_write(instance->settings);

    if (status == FAILED_IO_SETTINGS_CONFIGURATION)
    {
        // TODO: warning dialog
        return;
    }

    destroy_settings_network_window(instance);
}

static void on_button_cancel_clicked(GtkWidget *widget, SettingsNetworkWindow *instance)
{
    destroy_settings_network_window(instance);
}

static void on_window_destroy(GtkWidget *widget, gpointer data)
{
    SettingsNetworkWindow *instance = (SettingsNetworkWindow *)data;
    if (instance)
    {
        if (instance->settings)
        {
            settings_free(instance->settings);
            instance->settings = NULL;
        }
        g_free(instance);
    }
}

void destroy_settings_network_window(SettingsNetworkWindow *instance)
{
    if (instance)
    {
        if (GTK_IS_WINDOW(instance->window))
        {
            gtk_widget_destroy(GTK_WIDGET(instance->window));
        }
    }
}
