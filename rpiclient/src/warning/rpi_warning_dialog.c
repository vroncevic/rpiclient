/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_warning_dialog.c
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
#include "rpi_warning_dialog.h"

#if RPI_VERBOSE == 1
    #define CLOSE_WARNING_DIALOG "Close warning dialog.\n"
#endif

#define MISSING_PARENT_WARNING_DIALOG "Missing parent widget parameter for warning dialog.\n"
#define MISSING_MESSAGE_WARNING_DIALOG "Missing message parameter for warning dialog.\n"

#define FAILED_MALLOC_WARNING_DIALOG "Failed to allocate memory for warning dialog.\n"

//////////////////////////////////////////////////////////////////////////////
/// @brief Warning dialog complex widget
///   dialog - Gtk warning dialog widget
struct _WarningDialog
{
    GtkWidget *dialog;
};

WarningDialog *new_warning_dialog(GtkWidget *parent, const gchar *msg)
{
    if (!parent)
    {
        g_critical(MISSING_PARENT_WARNING_DIALOG);
        return NULL;
    }

    if (!msg)
    {
        g_critical(MISSING_MESSAGE_WARNING_DIALOG);
        return NULL;
    }

    WarningDialog *instance = g_malloc(sizeof(WarningDialog));

    if (!instance)
    {
        g_critical(FAILED_MALLOC_WARNING_DIALOG);
        return NULL;
    }

#if GTK_MAJOR_VERSION == 4
    // TODO: prepare warning dialog for gtk+-4.0
#elif GTK_MAJOR_VERSION == 3
    instance->dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_CLOSE,
        "%s",
        msg
    );
#else
    #error GTK_SUPPORTED_VERSION
#endif

    if (!GTK_IS_MESSAGE_DIALOG(instance->dialog))
    {
        g_critical(FAILED_MALLOC_WARNING_DIALOG);
        destroy_warning_dialog(instance);
        return NULL;
    }

    return instance;
}

void show_warning_dialog(WarningDialog *instance)
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
            // TODO: prepare run warning dialog for gtk+-4.0
#elif GTK_MAJOR_VERSION == 3
            result = gtk_dialog_run(GTK_DIALOG(instance->dialog));
#else
    #error GTK_SUPPORTED_VERSION
#endif
            if (result == GTK_RESPONSE_CLOSE || result == GTK_RESPONSE_DELETE_EVENT)
            {
                hide_warning_dialog(instance);
#if RPI_VERBOSE == 1
                g_debug(CLOSE_WARNING_DIALOG);
#endif
            }
        }
    }
}

void hide_warning_dialog(WarningDialog *instance)
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

void destroy_warning_dialog(WarningDialog *instance)
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
