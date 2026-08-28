/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home.c
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
#include "rpi_menu.h"
#include "rpi_home_frame.h"
#include "rpi_home.h"

#if RPI_VERBOSE == 1
#define ICON_PATH_RPI_HOME "Icon file path: %s.\n"
#endif

#define FAILED_MALLOC_RPI_HOME "Failed to allocate memory for home.\n"
#define FAILED_MALLOC_WINDOW_RPI_HOME "Failed to allocate memory for home window.\n"
#define FAILED_PIXBUF_RPI_HOME "Failed to create pixbuf from home icon.\n"
#define FAILED_RESOURCE_RPI_HOME "Failed to get resource path for home icon.\n"
#define FAILED_MALLOC_VBOX_RPI_HOME "Failed to allocate memory for vertial box home.\n"
#define FAILED_MALLOC_RPI_MENU_RPI_HOME "Failed to allocate memory for menu bar home.\n"
#define FAILED_MALLOC_RPI_HOME_FRAME_RPI_HOME "Failed to allocate memory for frame home.\n"

static const gchar* TITLE_WINDOW_RPI_HOME = "RPIClient v1.0";
static const gchar* LOGO_RPI_HOME = "icon.png";
static const gint WIDTH_WINDOW_RPI_HOME = 1200;
static const gint HEIGHT_WINDOW_RPI_HOME = 640;
static const gint CONTAINER_BORDER_WIDTH_WINDOW_RPI_HOME = 2;
static const gint VERTICAL_BOX_SPACING_WINDOW_RPI_HOME = 0;

//////////////////////////////////////////////////////////////////////////////
/// @brief RPIHome complex widget
///   window - Gtk window widget for home
///   vbox - Gtk vertial box widget for home
///   menu_bar - Complex widget for menu bar
///   frame_home - Complex widget for home frame
struct _RPIHome
{
    GtkWidget *window;
    GtkWidget *vbox;
    RPIMenu *menu_bar;
    RPIHomeFrame *frame_home;
};

RPIHome *new_rpi_home(void)
{
    RPIHome *instance = g_malloc(sizeof(RPIHome));

    if(!instance)
    {
        g_critical(FAILED_MALLOC_RPI_HOME);
        return NULL;
    }

    instance->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    if (!GTK_IS_WINDOW(instance->window))
    {
        g_critical(FAILED_MALLOC_WINDOW_RPI_HOME);
        destroy_rpi_home(instance);
        return NULL;
    }

    gtk_window_set_position(GTK_WINDOW(instance->window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(instance->window), WIDTH_WINDOW_RPI_HOME, HEIGHT_WINDOW_RPI_HOME);
    gchar *icon_file_path = rpi_get_resource_file_path(LOGO_RPI_HOME);

    if (icon_file_path)
    {
        GdkPixbuf *pixbuf = rpi_cpixbuf(icon_file_path);

        if (GDK_IS_PIXBUF(pixbuf))
        {
            gtk_window_set_icon(GTK_WINDOW(instance->window), pixbuf);

#if RPI_VERBOSE == 1
            g_debug(ICON_PATH_RPI_HOME, icon_file_path);
#endif

            g_object_unref(pixbuf);
            pixbuf = NULL;
        }
        else
        {
            g_critical(FAILED_PIXBUF_RPI_HOME);
            pixbuf = NULL;
        }

        g_free(icon_file_path);
        icon_file_path = NULL;
    }
    else
    {
        g_critical(FAILED_RESOURCE_RPI_HOME);
        icon_file_path = NULL;
    }

    gtk_window_set_title(GTK_WINDOW(instance->window), TITLE_WINDOW_RPI_HOME);
    gtk_window_set_resizable(GTK_WINDOW(instance->window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(instance->window), CONTAINER_BORDER_WIDTH_WINDOW_RPI_HOME);
    instance->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, VERTICAL_BOX_SPACING_WINDOW_RPI_HOME);

    if (!GTK_IS_BOX(instance->vbox))
    {
        g_critical(FAILED_MALLOC_VBOX_RPI_HOME);
        destroy_rpi_home(instance);
        return NULL;
    }

    instance->menu_bar = new_rpi_menu();

    if (!instance->menu_bar)
    {
        g_critical(FAILED_MALLOC_RPI_MENU_RPI_HOME);
        destroy_rpi_home(instance);
        return NULL;
    }

    instance->frame_home = new_rpi_home_frame();

    if (!instance->frame_home)
    {
        g_critical(FAILED_MALLOC_RPI_HOME_FRAME_RPI_HOME);
        destroy_rpi_home(instance);
        return NULL;
    }

    gtk_box_pack_start(
        GTK_BOX(instance->vbox),
        GTK_WIDGET(get_menu_bar_from_rpi_menu(instance->menu_bar)),
        FALSE,
        FALSE,
        3
    );
    gtk_box_pack_start(
        GTK_BOX(instance->vbox),
        GTK_WIDGET(get_frame_from_rpi_home_frame(instance->frame_home)),
        FALSE,
        FALSE,
        0
    );
    gtk_container_add(GTK_CONTAINER(instance->window), GTK_WIDGET(instance->vbox));

    return instance;
}

void show_rpi_home(RPIHome *instance)
{
    if (instance)
    {
        gboolean is_window = GTK_IS_WINDOW(instance->window);
        gboolean is_window_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->window));

        if (is_window && !is_window_visible)
        {
            // TODO: check for gtk-4.0
            gtk_widget_show_all(GTK_WIDGET(instance->window));
        }
    }
}

void hide_rpi_home(RPIHome *instance)
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

GtkWidget* get_window_from_rpi_home(RPIHome *instance)
{
    if (instance)
    {
        gboolean is_window = GTK_IS_WINDOW(instance->window);

        if (is_window)
        {
            return instance->window;
        }
    }

    return NULL;
}

RPIMenu* get_menu_bar_from_rpi_home(RPIHome *instance)
{
    if (instance)
    {
        if (instance->menu_bar)
        {
            return instance->menu_bar;
        }
    }

    return NULL;
}

void rpi_home_update_channel_status(RPIHome *instance, gint channel_id, gint value)
{
    if (instance && instance->frame_home)
    {
        rpi_home_frame_update_channel_status(instance->frame_home, channel_id, value);
    }
}

void destroy_rpi_home(RPIHome *instance)
{
    if (instance)
    {
        if (instance->frame_home)
        {
            destroy_rpi_home_frame(instance->frame_home);
            instance->frame_home = NULL;
        }

        if (instance->menu_bar)
        {
            destroy_rpi_menu(instance->menu_bar);
            instance->menu_bar = NULL;
        }

        if (GTK_IS_WINDOW(instance->window))
        {
            rpi_destroy_widget_misc(GTK_WIDGET(instance->window));
            instance->window = NULL;
        }

        instance->vbox = NULL;
        g_free(instance);
    }
}
