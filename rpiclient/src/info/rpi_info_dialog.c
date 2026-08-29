/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_info_dialog.c
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
#include "../rpi_config.h"
#include "../misc/rpi_misc.h"
#include "rpi_info_dialog.h"

#if RPI_VERBOSE == 1
static const gchar* const CLOSE_INFO_DIALOG = "Close info dialog.\n";
#endif

static const gchar* const MISSING_PARENT_INFO_DIALOG = "Missing parent widget parameter for info dialog.\n";
static const gchar* const MISSING_MESSAGE_INFO_DIALOG = "Missing message parameter for info dialog.\n";
static const gchar* const FAILED_MALLOC_INFO_DIALOG = "Failed to allocate memory for info dialog.\n";

//////////////////////////////////////////////////////////////////////////////
/// @brief Info dialog complex widget
///   dialog - Gtk dialog widget
struct _InfoDialog
{
    GtkWidget *dialog;
};

InfoDialog *new_info_dialog(GtkWidget *parent, const gchar *msg)
{
    if (!parent)
    {
        g_critical("%s", MISSING_PARENT_INFO_DIALOG);
        return NULL;
    }

    if (!msg)
    {
        g_critical("%s", MISSING_MESSAGE_INFO_DIALOG);
        return NULL;
    }

    InfoDialog *instance = g_malloc(sizeof(InfoDialog));

    if (!instance)
    {
        g_critical("%s", FAILED_MALLOC_INFO_DIALOG);
        return NULL;
    }

#if GTK_MAJOR_VERSION == 4
    // TODO: prepare info dialog for gtk+-4.0
#elif GTK_MAJOR_VERSION == 3
    instance->dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_CLOSE,
        "%s",
        msg
    );
#else
    #error "Supported GTK+ version: gtk+-3.0 gtk+-4.0!"
#endif

    if (!GTK_IS_MESSAGE_DIALOG(instance->dialog))
    {
        g_critical("%s", FAILED_MALLOC_INFO_DIALOG);
        destroy_info_dialog(instance);
        return NULL;
    }

    return instance;
}

void show_info_dialog(InfoDialog *instance)
{
    if (instance)
    {
        gboolean is_message_dialog = GTK_IS_MESSAGE_DIALOG(instance->dialog);
        gboolean is_message_dialog_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->dialog));

        if (is_message_dialog && !is_message_dialog_visible)
        {
            rpi_set_visible_widget_misc(GTK_WIDGET(instance->dialog), !is_message_dialog_visible);
            gint result = GTK_RESPONSE_NONE;
#if GTK_MAJOR_VERSION == 4
            // TODO: prepare run info dialog for gtk+-4.0
#elif GTK_MAJOR_VERSION == 3
            result = gtk_dialog_run(GTK_DIALOG(instance->dialog));
#else
            #error "Supported GTK+ version: gtk+-3.0 gtk+-4.0!"
#endif

            if (result == GTK_RESPONSE_CLOSE || result == GTK_RESPONSE_DELETE_EVENT)
            {
                hide_info_dialog(instance);
#if RPI_VERBOSE == 1
                g_debug("%s", CLOSE_INFO_DIALOG);
#endif
            }
        }
    }
}

void hide_info_dialog(InfoDialog *instance)
{
    if (instance)
    {
        gboolean is_message_dialog = GTK_IS_MESSAGE_DIALOG(instance->dialog);
        gboolean is_message_dialog_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->dialog));

        if (is_message_dialog && is_message_dialog_visible)
        {
            rpi_set_visible_widget_misc(GTK_WIDGET(instance->dialog), !is_message_dialog_visible);
        }
    }
}

void destroy_info_dialog(InfoDialog *instance)
{
    if (instance)
    {
        if (GTK_IS_MESSAGE_DIALOG(instance->dialog))
        {
            rpi_destroy_widget_misc(GTK_WIDGET(instance->dialog));
            instance->dialog = NULL;
        }

        g_free(instance);
    }
}
