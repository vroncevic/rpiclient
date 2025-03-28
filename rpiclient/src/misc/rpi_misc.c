/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_misc.c
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
#include "rpi_misc.h"

gboolean rpi_is_widget_visible_misc(const GtkWidget* widget)
{
    gboolean is_widget_visible = FALSE;

    if (widget)
    {
#if GTK_MAJOR_VERSION == 4
        is_widget_visible = gtk_widget_is_visible(GTK_WIDGET(widget));
#elif GTK_MAJOR_VERSION == 3
        is_widget_visible = gtk_widget_get_visible(GTK_WIDGET(widget));
#else
    #error GTK_SUPPORTED_VERSION
#endif
    }

    return is_widget_visible;
}

void rpi_set_visible_widget_misc(GtkWidget* widget, gboolean visible)
{
    if (!widget)
    {
        return;
    }

#if GTK_MAJOR_VERSION == 4
    if (GTK_IS_WINDOW(widget))
    {
        gtk_window_present(GTK_WINDOW(widget));
    }
    else
    {
        gtk_widget_set_visible(GTK_WIDGET(widget), visible);
    }
#elif GTK_MAJOR_VERSION == 3
    if (visible)
    {
        gtk_widget_show(GTK_WIDGET(widget));
    }
    else
    {
        gtk_widget_hide(GTK_WIDGET(widget));
    }
#else
    #error GTK_SUPPORTED_VERSION
#endif
}

void rpi_destroy_widget_misc(GtkWidget* widget)
{
    if (!widget)
    {
        return;
    }

#if GTK_MAJOR_VERSION == 4
    if (GTK_IS_WINDOW(widget))
    {
        gtk_window_close(GTK_WINDOW(widget));
    }

    gtk_widget_unparent(GTK_WIDGET(widget));
    g_object_unref(widget);
#elif GTK_MAJOR_VERSION == 3
    gtk_widget_destroy(GTK_WIDGET(widget));
#else
    #error GTK_SUPPORTED_VERSION
#endif
}
