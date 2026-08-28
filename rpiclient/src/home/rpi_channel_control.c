/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * channel_control.c
 * Copyright (C) 2016 Vladimir Roncevic <elektron.ronca@gmail.com>
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
#include "../misc/rpi_misc.h"
#include "../network/rpi_network.h"
#include "rpi_channel_control.h"

#define FAILED_MALLOC_CHANNEL_CONTROL "Failed to allocate memory for channel control.\n"

static const gint WIDTH_SCALE_CHANNEL_CONTROL = 40;
static const gint HEIGHT_SCALE_CHANNEL_CONTROL = 200;
static const gdouble MIN_VALUE_SCALE_CHANNEL_CONTROL = 0.0;
static const gdouble MAX_VALUE_SCALE_CHANNEL_CONTROL = 100.0;
static const gdouble STEP_VALUE_SCALE_CHANNEL_CONTROL = 1.0;
static const gdouble VALUE_SPINNER_ADJUSTMENT_CHANNEL_CONTROL = 0.0;
static const gdouble LOWER_SPINNER_ADJUSTMENT_CHANNEL_CONTROL = 0.0;
static const gdouble UPPER_SPINNER_ADJUSTMENT_CHANNEL_CONTROL = 100.0;
static const gdouble STEP_INCREMENT_SPINNER_ADJUSTMENT_CHANNEL_CONTROL = 1.0;
static const gdouble PAGE_INCREMENT_SPINNER_ADJUSTMENT_CHANNEL_CONTROL = 0.0;
static const gdouble PAGE_SIZE_SPINNER_ADJUSTMENT_CHANNEL_CONTROL = 0.0;
static const gdouble CLIMB_RATE_SPINNER_BUTTON_CHANNEL_CONTROL = 1.0;
static const gint DIGITS_SPINNER_BUTTON_CHANNEL_CONTROL = 0;

//////////////////////////////////////////////////////////////////////////////
/// @brief RPI channel control complex widget
///   control_channel_vertical_bar - Custom vertical bar widget for channel control
///   control_channel_scale - Gtk scale widget for channel control
///   control_channel_spinner_adjustment - Gtk adjustment widget for channel control
///   control_channel_spinner_button - Gtk spinner widget for channel control
///   control_channel_check_box - Gtk check box widget for channel control
struct _RPIChannelControl
{
    GtkWidget *frame;
    GtkVB *control_channel_vertical_bar;
    GtkWidget *control_channel_scale;
    GtkAdjustment *control_channel_spinner_adjustment;
    GtkWidget *control_channel_spinner_button;
    GtkWidget *control_channel_check_box;
    gint channel_id;
    gboolean is_updating;
};

static void on_scale_value_changed(GtkRange *range, gpointer user_data);
static void on_spinner_value_changed(GtkSpinButton *spin_button, gpointer user_data);
static void on_channel_check_box_toggled(GtkToggleButton *toggle_button, gpointer user_data);

static void on_scale_value_changed(GtkRange *range, gpointer user_data)
{
    RPIChannelControl *instance = (RPIChannelControl *)user_data;
    if (instance && !instance->is_updating)
    {
        instance->is_updating = TRUE;
        gdouble val = gtk_range_get_value(range);
        gtk_vb_set_state(instance->control_channel_vertical_bar, (gint)val);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(instance->control_channel_spinner_button), val);
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(instance->control_channel_check_box)))
        {
            rpi_network_send_channel(instance->channel_id, (gint)val);
        }
        instance->is_updating = FALSE;
    }
}

static void on_spinner_value_changed(GtkSpinButton *spin_button, gpointer user_data)
{
    RPIChannelControl *instance = (RPIChannelControl *)user_data;
    if (instance && !instance->is_updating)
    {
        instance->is_updating = TRUE;
        gdouble val = gtk_spin_button_get_value(spin_button);
        gtk_vb_set_state(instance->control_channel_vertical_bar, (gint)val);
        gtk_range_set_value(GTK_RANGE(instance->control_channel_scale), val);
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(instance->control_channel_check_box)))
        {
            rpi_network_send_channel(instance->channel_id, (gint)val);
        }
        instance->is_updating = FALSE;
    }
}

static void on_channel_check_box_toggled(GtkToggleButton *toggle_button, gpointer user_data)
{
    RPIChannelControl *instance = (RPIChannelControl *)user_data;
    if (instance)
    {
        gboolean is_active = gtk_toggle_button_get_active(toggle_button);
        gtk_widget_set_sensitive(instance->control_channel_scale, is_active);
        gtk_widget_set_sensitive(instance->control_channel_spinner_button, is_active);
        if (!is_active)
        {
            gtk_vb_set_state(instance->control_channel_vertical_bar, 0);
            rpi_network_send_channel(instance->channel_id, 0);
        }
        else
        {
            gdouble val = gtk_range_get_value(GTK_RANGE(instance->control_channel_scale));
            gtk_vb_set_state(instance->control_channel_vertical_bar, (gint)val);
            rpi_network_send_channel(instance->channel_id, (gint)val);
        }
    }
}

RPIChannelControl *new_rpi_channel_control(gint channel_id)
{
    RPIChannelControl *instance = g_malloc(sizeof(RPIChannelControl));

    if (!instance)
    {
        g_critical(FAILED_MALLOC_CHANNEL_CONTROL);
        return NULL;
    }

    instance->channel_id = channel_id;
    instance->is_updating = FALSE;
    instance->frame = gtk_frame_new(NULL);
    if (!GTK_IS_FRAME(instance->frame))
    {
        g_critical(FAILED_MALLOC_CHANNEL_CONTROL);
        g_free(instance);
        return NULL;
    }
    gtk_style_context_add_class(gtk_widget_get_style_context(instance->frame), "channel-box");

    instance->control_channel_vertical_bar = GTK_VB(gtk_vb_new());

    if (!GTK_IS_VB(instance->control_channel_vertical_bar))
    {
        g_critical(FAILED_MALLOC_CHANNEL_CONTROL);
        destroy_rpi_channel_control(instance);
        return NULL;        
    }

    gchar tooltip_text_vbar[10] = {0};
    g_snprintf(tooltip_text_vbar, sizeof(tooltip_text_vbar), "Channel %d", channel_id);
    gtk_widget_set_tooltip_text(GTK_WIDGET(instance->control_channel_vertical_bar), tooltip_text_vbar);
    instance->control_channel_scale = gtk_scale_new_with_range(
        GTK_ORIENTATION_VERTICAL,
        MIN_VALUE_SCALE_CHANNEL_CONTROL,
        MAX_VALUE_SCALE_CHANNEL_CONTROL,
        STEP_VALUE_SCALE_CHANNEL_CONTROL
    );

    if (!GTK_IS_SCALE(instance->control_channel_scale))
    {
        g_critical(FAILED_MALLOC_CHANNEL_CONTROL);
        destroy_rpi_channel_control(instance);
        return NULL;
    }

    gchar tooltip_text_scale[14] = {0};
    g_snprintf(tooltip_text_scale, sizeof(tooltip_text_scale), "Set Channel %d", channel_id);
    gtk_widget_set_tooltip_text(GTK_WIDGET(instance->control_channel_scale), tooltip_text_scale);
    gtk_range_set_inverted(GTK_RANGE(instance->control_channel_scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(instance->control_channel_scale), GTK_POS_TOP);
    gtk_widget_set_size_request(
        instance->control_channel_scale,
        WIDTH_SCALE_CHANNEL_CONTROL,
        HEIGHT_SCALE_CHANNEL_CONTROL
    );
    instance->control_channel_spinner_adjustment = (GtkAdjustment *) gtk_adjustment_new(
        VALUE_SPINNER_ADJUSTMENT_CHANNEL_CONTROL,
        LOWER_SPINNER_ADJUSTMENT_CHANNEL_CONTROL,
        UPPER_SPINNER_ADJUSTMENT_CHANNEL_CONTROL,
        STEP_INCREMENT_SPINNER_ADJUSTMENT_CHANNEL_CONTROL,
        PAGE_INCREMENT_SPINNER_ADJUSTMENT_CHANNEL_CONTROL,
        PAGE_SIZE_SPINNER_ADJUSTMENT_CHANNEL_CONTROL
    );

    if (!GTK_IS_ADJUSTMENT(instance->control_channel_spinner_adjustment))
    {
        g_critical(FAILED_MALLOC_CHANNEL_CONTROL);
        destroy_rpi_channel_control(instance);
        return NULL;
    }

    instance->control_channel_spinner_button = gtk_spin_button_new(
        GTK_ADJUSTMENT(instance->control_channel_spinner_adjustment),
        CLIMB_RATE_SPINNER_BUTTON_CHANNEL_CONTROL,
        DIGITS_SPINNER_BUTTON_CHANNEL_CONTROL
    );

    if (!GTK_IS_SPIN_BUTTON(instance->control_channel_spinner_button))
    {
        g_critical(FAILED_MALLOC_CHANNEL_CONTROL);
        destroy_rpi_channel_control(instance);
        return NULL;
    }

    gtk_widget_set_size_request(
        GTK_WIDGET(instance->control_channel_spinner_button),
        90,
        30
    );

    gchar tooltip_text_spinner[23] = {0};
    g_snprintf(tooltip_text_spinner, sizeof(tooltip_text_spinner), "Set level at channel %d", channel_id);
    gtk_widget_set_tooltip_text(GTK_WIDGET(instance->control_channel_spinner_button), tooltip_text_spinner);
    gchar text_check_box[16] = {0};
    g_snprintf(text_check_box, sizeof(text_check_box), "Channel %d", channel_id);
    instance->control_channel_check_box = gtk_check_button_new_with_label(text_check_box);

    if (!GTK_IS_CHECK_BUTTON(instance->control_channel_check_box))
    {
        g_critical(FAILED_MALLOC_CHANNEL_CONTROL);
        destroy_rpi_channel_control(instance);
        return NULL;
    }

    gchar tooltip_text_check_box[24] = {0};
    g_snprintf(tooltip_text_check_box, sizeof(tooltip_text_check_box), "Activate Channel %d", channel_id);
    gtk_widget_set_tooltip_text(GTK_WIDGET(instance->control_channel_check_box), tooltip_text_check_box);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(instance->control_channel_check_box), FALSE);
    gtk_widget_set_sensitive(instance->control_channel_scale, FALSE);
    gtk_widget_set_sensitive(instance->control_channel_spinner_button, FALSE);

    g_signal_connect(
        G_OBJECT(instance->control_channel_check_box),
        "toggled",
        G_CALLBACK(on_channel_check_box_toggled),
        instance
    );
    g_signal_connect(
        G_OBJECT(instance->control_channel_scale),
        "value-changed",
        G_CALLBACK(on_scale_value_changed),
        instance
    );
    g_signal_connect(
        G_OBJECT(instance->control_channel_spinner_button),
        "value-changed",
        G_CALLBACK(on_spinner_value_changed),
        instance
    );

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 4);
    gtk_container_add(GTK_CONTAINER(instance->frame), vbox);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(instance->control_channel_vertical_bar), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(instance->control_channel_scale), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    gtk_widget_set_halign(instance->control_channel_check_box, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), instance->control_channel_check_box, FALSE, FALSE, 0);

    gtk_widget_set_halign(instance->control_channel_spinner_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), instance->control_channel_spinner_button, FALSE, FALSE, 0);

    return instance;
}

void show_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance && GTK_IS_FRAME(instance->frame))
    {
        gtk_widget_show_all(instance->frame);
    }
}

void hide_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance && GTK_IS_FRAME(instance->frame))
    {
        gtk_widget_hide(instance->frame);
    }
}

GtkVB* get_vertical_bar_from_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance)
    {
        gboolean is_vertical_bar = GTK_IS_VB(instance->control_channel_vertical_bar);

        if (is_vertical_bar)
        {
            return instance->control_channel_vertical_bar;
        }
    }

    return NULL;
}

GtkWidget* get_scale_from_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance)
    {
        gboolean is_scale = GTK_IS_SCALE(instance->control_channel_scale);

        if (is_scale)
        {
            return instance->control_channel_scale;
        }
    }

    return NULL;
}

GtkWidget* get_spinner_button_from_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance)
    {
        gboolean is_spinner = GTK_IS_SPIN_BUTTON(instance->control_channel_spinner_button);

        if (is_spinner)
        {
            return instance->control_channel_spinner_button;
        }
    }

    return NULL;
}

GtkWidget* get_check_box_from_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance)
    {
        gboolean is_check_box = GTK_IS_CHECK_BUTTON(instance->control_channel_check_box);

        if (is_check_box)
        {
            return instance->control_channel_check_box;
        }
    }

    return NULL;
}

GtkWidget* get_frame_from_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance && GTK_IS_FRAME(instance->frame))
    {
        return instance->frame;
    }

    return NULL;
}

void destroy_rpi_channel_control(RPIChannelControl *instance)
{
    if (instance)
    {
        if (GTK_IS_FRAME(instance->frame))
        {
            rpi_destroy_widget_misc(instance->frame);
            instance->frame = NULL;
        }

        instance->control_channel_vertical_bar = NULL;
        instance->control_channel_scale = NULL;
        instance->control_channel_spinner_adjustment = NULL;
        instance->control_channel_spinner_button = NULL;
        instance->control_channel_check_box = NULL;

        g_free(instance);
    }
}
