/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_about_dialog.c
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
#include "../misc/rpi_misc.h"
#include "../resource/rpi_resource.h"
#include "rpi_about_dialog.h"

#if RPI_VERBOSE == 1
    #define LOGO_FILE_PATH_ABOUT_DIALOG "About logo file path: %s.\n"
#endif

#define FAILED_MALLOC_ABOUT_DIALOG "Failed to allocate memory for about dialog.\n"
#define FAILED_RESOURCE_ABOUT_DIALOG "Failed to get media resource path for about logo.\n"
#define FAILED_PIXBUF_ABOUT_DIALOG "Failed to create about pixbuf from media resource path.\n"

static const gchar* TEXT_NAME_ABOUT_DIALOG = "About RPIClient";
static const gchar* TEXT_VERSION_ABOUT_DIALOG = RPI_CLIENT_VERSION;
static const gchar* TEXT_COPYRIGHT_ABOUT_DIALOG = "2025 (c) elektron.ronca@gmail.com";
static const gchar* TEXT_LICENSE_ABOUT_DIALOG = "GPLv3 https://github.com/vroncevic/rpi_client_gtk?tab=GPL-3.0-1-ov-file";
static const gchar* TEXT_AUTHORS_ABOUT_DIALOG = "Vladimir Roncevic";
static const gchar* TEXT_COMMENTS_ABOUT_DIALOG = "Free Software you can redistribute it and/or modify it.";
static const gchar* TEXT_WEBSITE_ABOUT_DIALOG = "https://vroncevic.github.io/rpi_client_gtk/";
static const gchar* LOGO_FILE_NAME_ABOUT_DIALOG = "logo.png";

//////////////////////////////////////////////////////////////////////////////
/// @brief About dialog complex widget
///   dialog - Gtk about dialog widget
struct _AboutDialog
{
    GtkWidget *dialog;
};

AboutDialog *new_about_dialog(void)
{
    AboutDialog *instance = g_try_malloc(sizeof(AboutDialog));

    if(!instance)
    {
        g_critical(FAILED_MALLOC_ABOUT_DIALOG);
        return NULL;
    }

    instance->dialog = gtk_about_dialog_new();

    if (!GTK_IS_ABOUT_DIALOG(instance->dialog))
    {
        g_critical(FAILED_MALLOC_ABOUT_DIALOG);
        destroy_about_dialog(instance);
        return NULL;
    }

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(instance->dialog), TEXT_NAME_ABOUT_DIALOG);
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(instance->dialog), TEXT_VERSION_ABOUT_DIALOG);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(instance->dialog), TEXT_COPYRIGHT_ABOUT_DIALOG);
    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(instance->dialog), TEXT_LICENSE_ABOUT_DIALOG);
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(instance->dialog), (const char *[]){TEXT_AUTHORS_ABOUT_DIALOG, NULL});
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(instance->dialog), TEXT_COMMENTS_ABOUT_DIALOG);
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(instance->dialog), TEXT_WEBSITE_ABOUT_DIALOG);
    gchar *logo_file_path = rpi_get_resource_file_path(LOGO_FILE_NAME_ABOUT_DIALOG);

    if (logo_file_path)
    {
        GdkPixbuf *pixbuf = rpi_cpixbuf(logo_file_path);

        if (GDK_IS_PIXBUF(pixbuf))
        {
            gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(instance->dialog), pixbuf);
#if RPI_VERBOSE == 1
            g_debug(LOGO_FILE_PATH_ABOUT_DIALOG, logo_file_path);
#endif
            g_object_unref(pixbuf);
            pixbuf = NULL;
        }
        else
        {
            g_warning(FAILED_PIXBUF_ABOUT_DIALOG);
            pixbuf = NULL;
        }

        g_free(logo_file_path);
        logo_file_path = NULL;
    }
    else
    {
        g_warning(FAILED_RESOURCE_ABOUT_DIALOG);
        logo_file_path = NULL;
    }

    g_signal_connect_swapped(G_OBJECT(instance->dialog), "response", G_CALLBACK(destroy_about_dialog), instance);

    return instance;
}

void show_about_dialog(AboutDialog *instance)
{
    if (!instance) return;

    if (instance)
    {
        gboolean is_about_dialog = GTK_IS_ABOUT_DIALOG(instance->dialog);
        gboolean is_about_dialog_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->dialog));

        if (is_about_dialog && !is_about_dialog_visible)
        {
            rpi_set_visible_widget_misc(GTK_WIDGET(instance->dialog), !is_about_dialog_visible);
        }
    }
}

void hide_about_dialog(AboutDialog *instance)
{
    if (!instance) return;

    if (instance)
    {
        gboolean is_about_dialog = GTK_IS_ABOUT_DIALOG(instance->dialog);
        gboolean is_about_dialog_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->dialog));

        if (is_about_dialog && is_about_dialog_visible)
        {
            rpi_set_visible_widget_misc(GTK_WIDGET(instance->dialog), !is_about_dialog_visible);
        }
    }
}

void destroy_about_dialog(AboutDialog *instance)
{
    if (!instance) return;

    if (instance)
    {
        if (GTK_IS_ABOUT_DIALOG(instance->dialog))
        {
            rpi_destroy_widget_misc(GTK_WIDGET(instance->dialog));
            instance->dialog = NULL;
        }

        g_free(instance);
    }
}
