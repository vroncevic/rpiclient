/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame.c
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
#include "rpi_channel_status.h"
#include "rpi_channel_control.h"
#include "rpi_home_frame.h"

#if RPI_VERBOSE == 1
#define SETUP_STATUS_CHANNEL_HOME_FRAME "Setup status channel id: %d.\n"
#define SETUP_CONTROL_CHANNEL_HOME_FRAME "Setup control channel id: %d.\n"
#endif

#define FAILED_MALLOC_HOME_FRAME "Failed to allocate memory for home frame.\n"

static const gchar* CONTROL_FRAME_LABEL_HOME_FRAME = "RPI Control";
static const gchar* STATUS_FRAME_LABEL_HOME_FRAME = "RPI Status";

//////////////////////////////////////////////////////////////////////////////
/// @brief RPIHomeFrame frame complex widget
///   frame_home - Gtk widget for home frame
///   vpaned - Gtk widget for vertical paned
///   frame_status - Gtk widget for status frame
///   box_status - Gtk widget for status horizontal box container
///   channels_status - Complex custom widgets for channel status
///   frame_control - Gtk widget for control frame
///   box_control - Gtk widget for control horizontal box container
///   channels_control - Complex custom widgets for channel control
struct _RPIRPIHomeFrame
{
    GtkWidget *frame_home;
    GtkWidget *vpaned;
    GtkWidget *frame_status;
    GtkWidget *box_status;
    RPIChannelStatus *channels_status[MAX_CHANNELS_STATUS_HOME_FRAME];
    GtkWidget *frame_control;
    GtkWidget *box_control;
    RPIChannelControl *channels_control[MAX_CHANNELS_CONTROL_HOME_FRAME];
};

RPIHomeFrame *new_rpi_home_frame(void)
{
    RPIHomeFrame *instance = g_malloc(sizeof(RPIHomeFrame));

    if (!instance)
    {
        g_critical(FAILED_MALLOC_HOME_FRAME);
        return NULL;
    }

    instance->frame_home = gtk_frame_new("");

    if (!GTK_IS_FRAME(instance->frame_home))
    {
        g_critical(FAILED_MALLOC_HOME_FRAME);
        destroy_rpi_home_frame(instance);
        return NULL;
    }

    instance->vpaned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);

    if (!GTK_IS_PANED(instance->vpaned))
    {
        g_critical(FAILED_MALLOC_HOME_FRAME);
        destroy_rpi_home_frame(instance);
        return NULL;
    }

    gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_home), GTK_SHADOW_IN);
    gtk_container_add(GTK_CONTAINER(instance->frame_home), instance->vpaned);

    instance->frame_control = gtk_frame_new(CONTROL_FRAME_LABEL_HOME_FRAME);

    if (!GTK_IS_FRAME(instance->frame_control))
    {
        g_critical(FAILED_MALLOC_HOME_FRAME);
        destroy_rpi_home_frame(instance);
        return NULL;
    }

    instance->box_control = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);

    if (!GTK_IS_BOX(instance->box_control))
    {
        g_critical(FAILED_MALLOC_HOME_FRAME);
        destroy_rpi_home_frame(instance);
        return NULL;
    }

    gtk_box_set_homogeneous(GTK_BOX(instance->box_control), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(instance->box_control), 6);
    gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_control), GTK_SHADOW_IN);
    gtk_container_add(GTK_CONTAINER(instance->frame_control), instance->box_control);

    for (guint i = 0; i < MAX_CHANNELS_CONTROL_HOME_FRAME; i++)
    {
        instance->channels_control[i] = new_rpi_channel_control(i + 1);
        gtk_box_pack_start(
            GTK_BOX(instance->box_control),
            get_frame_from_rpi_channel_control(instance->channels_control[i]),
            TRUE, TRUE, 0
        );

#if RPI_VERBOSE == 1
        g_debug(SETUP_CONTROL_CHANNEL_HOME_FRAME, i);
#endif
    }

    instance->frame_status = gtk_frame_new(STATUS_FRAME_LABEL_HOME_FRAME);

    if (!GTK_IS_FRAME(instance->frame_status))
    {
        g_critical(FAILED_MALLOC_HOME_FRAME);
        destroy_rpi_home_frame(instance);
        return NULL;
    }

    instance->box_status = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);

    if (!GTK_IS_BOX(instance->box_status))
    {
        g_critical(FAILED_MALLOC_HOME_FRAME);
        destroy_rpi_home_frame(instance);
        return NULL;
    }

    gtk_box_set_homogeneous(GTK_BOX(instance->box_status), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(instance->box_status), 6);
    gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_status), GTK_SHADOW_IN);
    gtk_container_add(GTK_CONTAINER(instance->frame_status), instance->box_status);

    for (guint i = 0; i < MAX_CHANNELS_STATUS_HOME_FRAME; i++)
    {
        instance->channels_status[i] = new_rpi_channel_status(i + 1);
        gtk_box_pack_start(
            GTK_BOX(instance->box_status),
            get_frame_from_rpi_channel_status(instance->channels_status[i]),
            TRUE, TRUE, 0
        );

#if RPI_VERBOSE == 1
        g_debug(SETUP_STATUS_CHANNEL_HOME_FRAME, i);
#endif
    }

    gtk_paned_add1(GTK_PANED(instance->vpaned), GTK_WIDGET(instance->frame_control));
    gtk_paned_add2(GTK_PANED(instance->vpaned), GTK_WIDGET(instance->frame_status));

    return instance;
}

void show_rpi_home_frame(RPIHomeFrame *instance)
{
    if (instance)
    {
        gboolean is_frame = GTK_IS_FRAME(instance->frame_home);
        gboolean is_frame_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->frame_home));

        if (is_frame && !is_frame_visible)
        {
            rpi_set_visible_widget_misc(GTK_WIDGET(instance->frame_home), !is_frame_visible);
        }
    }
}

void hide_rpi_home_frame(RPIHomeFrame *instance)
{
    if (instance)
    {
        gboolean is_frame = GTK_IS_FRAME(instance->frame_home);
        gboolean is_frame_visible = rpi_is_widget_visible_misc(GTK_WIDGET(instance->frame_home));

        if (is_frame && is_frame_visible)
        {
            rpi_set_visible_widget_misc(GTK_WIDGET(instance->frame_home), !is_frame_visible);
        }
    }
}

GtkWidget* get_frame_from_rpi_home_frame(RPIHomeFrame *instance)
{
    if(instance)
    {
        gboolean is_frame = GTK_IS_FRAME(instance->frame_home);

        if (is_frame)
        {
            return instance->frame_home;
        }
    }

    return NULL;
}

void rpi_home_frame_update_channel_status(RPIHomeFrame *instance, gint channel_id, gint value)
{
    if (instance)
    {
        if (channel_id >= 1 && channel_id <= MAX_CHANNELS_STATUS_HOME_FRAME)
        {
            gint idx = channel_id - 1;
            if (instance->channels_status[idx])
            {
                set_value_to_rpi_channel_status(instance->channels_status[idx], value);
            }
        }
    }
}

void destroy_rpi_home_frame(RPIHomeFrame *instance)
{
    if (instance)
    {
        if (GTK_IS_FRAME(instance->frame_home))
        {
            rpi_destroy_widget_misc(GTK_WIDGET(instance->frame_home));
            instance->frame_home = NULL;
        }

        for (guint i = 0; i < MAX_CHANNELS_CONTROL_HOME_FRAME; i++)
        {
            if (instance->channels_control[i])
            {
                destroy_rpi_channel_control(instance->channels_control[i]);
                instance->channels_control[i] = NULL;
            }
        }

        for (guint i = 0; i < MAX_CHANNELS_STATUS_HOME_FRAME; i++)
        {
            if (instance->channels_status[i])
            {
                destroy_rpi_channel_status(instance->channels_status[i]);
                instance->channels_status[i] = NULL;
            }
        }

        instance->vpaned = NULL;
        instance->frame_status = NULL;
        instance->box_status = NULL;
        instance->frame_control = NULL;
        instance->box_control = NULL;
        g_free(instance);
    }
}
