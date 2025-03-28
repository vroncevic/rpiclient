/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_general_window.c
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
#include "../misc/rpi_misc.h"
#include "rpi_settings.h"
#include "rpi_settings_config.h"
#include "rpi_settings_general_window.h"

#if RPI_VERBOSE == 1
#define ICON_FILE_PATH_GENERAL_WINDOW "Icon file path: %s.\n"
#endif

#define FAILED_MALLOC_SETTINGS_GENERAL_WINDOW "Failed to allocate memory for settings general window.\n"
#define FAILED_PIXBUF_SETTINGS_GENERAL_WINDOW "Failed to create pixbuf from settings general icon.\n"
#define FAILED_RESOURCE_SETTINGS_GENERAL_WINDOW "Failed to get resource path for settings general icon.\n"

static const gchar* TITLE_SETTINGS_GENERAL_WINDOW = "Settings General";
static const gchar* ICON_SETTINGS_GENERAL_WINDOW = "icon.png";
static const gint WIDTH_SETTINGS_GENERAL_WINDOW = 350;
static const gint HEIGHT_SETTINGS_GENERAL_WINDOW = 350;
static const gint CONTAINER_BORDER_WIDTH_SETTINGS_GENERAL_WINDOW = 10;
static const gint SPACING_VBOX_SETTINGS_GENERAL_WINDOW = 20;
static const gint TABLE_ROW_SPACINGS_TABLE_SETTINGS_GENERAL_WINDOW = 10;
static const gint TABLE_COL_SPACINGS_TABLE_SETTINGS_GENERAL_WINDOW = 10;
static const gchar* TEXT_FRAME_CONTORL_EXIT_SETTINGS_NETWORK_WINDOW = "Exit from rpiclient";
static const gchar* TEXT_CHECK_BUTTON_CONTROL_EXIT_SETTINGS_NETWORK_WINDOW = "Display Exit Dialog on Quit?";
static const gchar* TEXT_BUTTON_OK_HBOX_SETTINGS_GENERAL_WINDOW = "OK";
static const gchar* TEXT_BUTTON_CANCEL_HBOX_SETTINGS_GENERAL_WINDOW = "Cancel";
static const gint WIDTH_BUTTON_HBOX_SETTINGS_GENERAL_WINDOW = 70;
static const gint HEIGHT_BUTTON_HBOX_SETTINGS_GENERAL_WINDOW = 30;
static const gint SPACING_HBOX_SETTINGS_GENERAL_WINDOW = 3;

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings general window complex widget
///   window - Gtk window widget
///   vbox - Gtk widget for vertical box
///   grid - Gtk widget for grid
///   frame_control_exit - Gtk widget for frame
///   check_button_control_exit - Gtk widget for check box
///   hbox - Gtk widget for horizontal alignment
///   button_ok - Gtk widget for ok action
///   button_cancel - Gtk widget for cancel action
///   settings - Custom structure for keeping settings
struct _SettingsGeneralWindow
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *grid;
    GtkWidget *frame_control_exit;
    GtkWidget *check_button_control_exit;
    GtkWidget *hbox;
    GtkWidget *button_ok;
    GtkWidget *button_cancel;
    SettingsConfig* settings;
};

static void on_button_ok_clicked(GtkWidget *widget, SettingsGeneralWindow *instance);
static void on_button_cancel_clicked(GtkWidget *widget, SettingsGeneralWindow *instance);

SettingsGeneralWindow *new_settings_general_window(void)
{
    SettingsGeneralWindow *instance = g_malloc(sizeof(SettingsGeneralWindow));

    if (!instance)
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        return NULL;
    }

    instance->settings = settings_read();

    if (!instance->settings)
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    instance->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    if (!GTK_IS_WINDOW(instance->window))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    gtk_window_set_position(GTK_WINDOW(instance->window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(
        GTK_WINDOW(instance->window), WIDTH_SETTINGS_GENERAL_WINDOW, HEIGHT_SETTINGS_GENERAL_WINDOW
    );
    gtk_window_set_title(GTK_WINDOW(instance->window), TITLE_SETTINGS_GENERAL_WINDOW);
    gchar *icon_file_path = rpi_get_resource_file_path(ICON_SETTINGS_GENERAL_WINDOW);

    if (icon_file_path)
    {
        GdkPixbuf *pixbuf = rpi_cpixbuf(icon_file_path);

        if (GDK_IS_PIXBUF(pixbuf))
        {
            gtk_window_set_icon(GTK_WINDOW(instance->window), pixbuf);

#if RPI_VERBOSE == 1
            g_debug(ICON_FILE_PATH_GENERAL_WINDOW, icon_file_path);
#endif

            g_object_unref(pixbuf);
            pixbuf = NULL;
        }
        else
        {
            g_critical(FAILED_PIXBUF_SETTINGS_GENERAL_WINDOW);
            pixbuf = NULL;
        }

        g_free(icon_file_path);
        icon_file_path = NULL;
    }
    else
    {
        g_critical(FAILED_RESOURCE_SETTINGS_GENERAL_WINDOW);
        icon_file_path = NULL;
    }

    gtk_window_set_resizable(GTK_WINDOW(instance->window), FALSE);
    gtk_container_set_border_width(
        GTK_CONTAINER(instance->window), CONTAINER_BORDER_WIDTH_SETTINGS_GENERAL_WINDOW
    );
    instance->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, SPACING_VBOX_SETTINGS_GENERAL_WINDOW);

    if (!GTK_IS_BOX(instance->vbox))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    gtk_container_add(GTK_CONTAINER(instance->window), GTK_WIDGET(instance->vbox));
    instance->grid = gtk_grid_new();

    if (!GTK_IS_GRID(instance->grid))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    gtk_grid_set_row_spacing(GTK_GRID(instance->grid), TABLE_ROW_SPACINGS_TABLE_SETTINGS_GENERAL_WINDOW);
    gtk_grid_set_column_spacing(GTK_GRID(instance->grid), TABLE_COL_SPACINGS_TABLE_SETTINGS_GENERAL_WINDOW);
    gtk_box_pack_start(GTK_BOX(instance->vbox), instance->grid, TRUE, TRUE, 0);
    instance->frame_control_exit = gtk_frame_new(TEXT_FRAME_CONTORL_EXIT_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_FRAME(instance->frame_control_exit))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_control_exit), GTK_SHADOW_IN);
    instance->check_button_control_exit = gtk_check_button_new_with_label(TEXT_CHECK_BUTTON_CONTROL_EXIT_SETTINGS_NETWORK_WINDOW);

    if (!GTK_IS_CHECK_BUTTON(instance->check_button_control_exit))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    gboolean is_exit_enabled = is_exit_enabled_settings(instance->settings);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(instance->check_button_control_exit), is_exit_enabled);
    gtk_container_add(
        GTK_CONTAINER(instance->frame_control_exit), GTK_WIDGET(instance->check_button_control_exit)
    );

    gtk_grid_attach(GTK_GRID(instance->grid), GTK_WIDGET(instance->frame_control_exit), 0, 0, 1, 1);
    instance->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, SPACING_HBOX_SETTINGS_GENERAL_WINDOW);

    if (!GTK_IS_BOX(instance->hbox))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    instance->button_ok = gtk_button_new_with_label(TEXT_BUTTON_OK_HBOX_SETTINGS_GENERAL_WINDOW);

    if (!GTK_IS_BUTTON(instance->button_ok))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    instance->button_cancel = gtk_button_new_with_label(TEXT_BUTTON_CANCEL_HBOX_SETTINGS_GENERAL_WINDOW);

    if (!GTK_IS_BUTTON(instance->button_cancel))
    {
        g_critical(FAILED_MALLOC_SETTINGS_GENERAL_WINDOW);
        destroy_settings_general_window(instance);
        return NULL;
    }

    gtk_widget_set_size_request(
        GTK_WIDGET(instance->button_ok),
        WIDTH_BUTTON_HBOX_SETTINGS_GENERAL_WINDOW,
        HEIGHT_BUTTON_HBOX_SETTINGS_GENERAL_WINDOW
    );
    gtk_widget_set_size_request(
        GTK_WIDGET(instance->button_cancel),
        WIDTH_BUTTON_HBOX_SETTINGS_GENERAL_WINDOW,
        HEIGHT_BUTTON_HBOX_SETTINGS_GENERAL_WINDOW
    );
    gtk_container_add(GTK_CONTAINER(instance->hbox), GTK_WIDGET(instance->button_ok));
    gtk_container_add(GTK_CONTAINER(instance->hbox), GTK_WIDGET(instance->button_cancel));
    gtk_box_pack_start(GTK_BOX(instance->vbox), GTK_WIDGET(instance->hbox), FALSE, FALSE, 0);
    g_signal_connect_swapped(
        G_OBJECT(instance->window), "delete-event", G_CALLBACK(destroy_settings_general_window), instance
    );
    g_signal_connect(
        G_OBJECT(instance->button_ok), "clicked", G_CALLBACK(on_button_ok_clicked), instance
    );
    g_signal_connect(
        G_OBJECT(instance->button_cancel), "clicked", G_CALLBACK(on_button_cancel_clicked), instance
    );

    return instance;
}

void show_settings_general_window(SettingsGeneralWindow *instance)
{
    if (instance)
    {
#if GTK_MAJOR_VERSION == 4
        gboolean is_window = GTK_IS_WINDOW(instance->window);
        gboolean is_window_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->window));

        if (is_window && is_window_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->window));
        }

        gboolean is_vbox = GTK_IS_BOX(instance->window);
        gboolean is_vbox_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->window));

        if (is_vbox && is_vbox_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->vbox));
        }

        gboolean is_grid = GTK_IS_GRID(instance->grid);
        gboolean is_grid_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->grid));

        if (is_grid && is_grid_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->grid));
        }

        gboolean is_frame_control_exit = GTK_IS_FRAME(instance->frame_control_exit);
        gboolean is_frame_control_exit_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->frame_control_exit));

        if (is_frame_control_exit && is_frame_control_exit_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->frame_control_exit));
        }

        gboolean is_check_button_control_exit = GTK_IS_CHECK_BUTTON(instance->check_button_control_exit);
        gboolean is_check_button_control_exit_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->check_button_control_exit));

        if (is_check_button_control_exit && is_check_button_control_exit_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->check_button_control_exit));
        }

        gboolean is_hbox = GTK_IS_BOX(instance->hbox);
        gboolean is_hbox_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->hbox));

        if (is_hbox && is_hbox_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->hbox));
        }

        gboolean is_button_ok = GTK_IS_BUTTON(instance->button_ok);
        gboolean is_button_ok_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->button_ok));

        if (is_button_ok && is_button_ok_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->button_ok));
        }

        gboolean is_button_cancel = GTK_IS_BUTTON(instance->button_cancel);
        gboolean is_button_cancel_hidden = !gtk_widget_get_visible(GTK_WIDGET(instance->button_cancel));

        if (is_button_cancel && is_button_cancel_hidden)
        {
            gtk_widget_show(GTK_WIDGET(instance->button_cancel));
        }
#elif GTK_MAJOR_VERSION == 3
        gboolean is_window = GTK_IS_WINDOW(instance->window);
        gboolean is_window_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->window));

        if (is_window && !is_window_visible)
        {
            // TODO: prepare for misc
            gtk_widget_show_all(GTK_WIDGET(instance->window));
        }
#else
    #error GTK_SUPPORTED_VERSION
#endif
    }
}

void hide_settings_general_window(SettingsGeneralWindow *instance)
{
    if (instance)
    {
        gboolean is_window = GTK_IS_WINDOW(instance->window);
        gboolean is_window_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->window));

        if (is_window && is_window_visible)
        {
            rpi_set_visible_widget_misc(GTK_WIDGET(instance->window), !is_window_visible);
        }
    }
}

static void on_button_ok_clicked(GtkWidget *widget, SettingsGeneralWindow *instance)
{
    const gboolean no_exit_state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(instance->check_button_control_exit));
    set_exit_enabled_settings(instance->settings, no_exit_state);
    guint status = settings_write(instance->settings);

    if (status == FAILED_IO_SETTINGS_CONFIGURATION)
    {
        // TODO: warning dialog
        return;
    }

    destroy_settings_general_window(instance);
}

static void on_button_cancel_clicked(GtkWidget *widget, SettingsGeneralWindow *instance)
{
    destroy_settings_general_window(instance);
}

void destroy_settings_general_window(SettingsGeneralWindow *instance)
{
    if (instance)
    {
        if (instance->settings)
        {
            settings_free(instance->settings);
            instance->settings = NULL;
        }

        if (GTK_IS_WINDOW(instance->window))
        {
            rpi_destroy_widget_misc(GTK_WIDGET(instance->window));
            instance->window = NULL;
        }

        instance->button_cancel = NULL;
        instance->button_ok = NULL;
        instance->hbox = NULL;
        instance->check_button_control_exit = NULL;
        instance->frame_control_exit = NULL;
        instance->grid = NULL;
        g_free(instance);
    }
}
