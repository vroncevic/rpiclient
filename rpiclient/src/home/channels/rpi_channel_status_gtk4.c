/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_channel_status_gtk4.c
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
#include "rpi_channel_status.h"

static const gchar *const FAILED_MALLOC_CHANNEL_STATUS =
    "Failed to allocate memory for channel status.\n";

//////////////////////////////////////////////////////////////////////////////
/// @brief RPI channel status complex widget
struct _RPIChannelStatus {
  GtkWidget *frame;
  GtkWidget *activate_channel_check_box;
  GtkWidget *status_channel_label;
  GtkVB *status_channel_vertical_bar;
  gint channel_id;
  gint current_value;
};

static void on_status_check_box_toggled(GtkCheckButton *check_button,
                                        gpointer user_data);

static void on_status_check_box_toggled(GtkCheckButton *check_button,
                                        gpointer user_data) {
  RPIChannelStatus *instance = (RPIChannelStatus *)user_data;
  if (instance) {
    gboolean is_active = gtk_check_button_get_active(check_button);
    gtk_widget_set_sensitive(GTK_WIDGET(instance->status_channel_vertical_bar),
                             is_active);
    gtk_widget_set_sensitive(instance->status_channel_label, is_active);
    if (!is_active) {
      gtk_vb_set_state(instance->status_channel_vertical_bar, 0);
    } else {
      gtk_vb_set_state(instance->status_channel_vertical_bar,
                       instance->current_value);
    }
  }
}

RPIChannelStatus *new_rpi_channel_status(gint channel_id) {
  RPIChannelStatus *instance = g_malloc(sizeof(RPIChannelStatus));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_CHANNEL_STATUS);
    return NULL;
  }

  instance->channel_id = channel_id;
  instance->current_value = 0;

  instance->frame = gtk_frame_new(NULL);
  if (!GTK_IS_FRAME(instance->frame)) {
    g_critical("%s", FAILED_MALLOC_CHANNEL_STATUS);
    g_free(instance);
    return NULL;
  }
  gtk_widget_add_css_class(instance->frame, "channel-box");

  gchar status_checkbox[24] = {0};
  g_snprintf(status_checkbox, sizeof(status_checkbox), "Channel %d",
             channel_id - 1);
  instance->activate_channel_check_box =
      gtk_check_button_new_with_label(status_checkbox);
  if (!GTK_IS_CHECK_BUTTON(instance->activate_channel_check_box)) {
    g_critical("%s", FAILED_MALLOC_CHANNEL_STATUS);
    destroy_rpi_channel_status(instance);
    return NULL;
  }

  gchar tooltip_text_checkbox[32] = {0};
  g_snprintf(tooltip_text_checkbox, sizeof(tooltip_text_checkbox),
             "Activate Channel %d", channel_id - 1);
  gtk_widget_set_tooltip_text(instance->activate_channel_check_box,
                              tooltip_text_checkbox);
  gtk_check_button_set_active(
      GTK_CHECK_BUTTON(instance->activate_channel_check_box), FALSE);

  instance->status_channel_vertical_bar = GTK_VB(gtk_vb_new());
  if (!GTK_IS_VB(instance->status_channel_vertical_bar)) {
    g_critical("%s", FAILED_MALLOC_CHANNEL_STATUS);
    destroy_rpi_channel_status(instance);
    return NULL;
  }

  gchar tooltip_text_vbar[24] = {0};
  g_snprintf(tooltip_text_vbar, sizeof(tooltip_text_vbar), "Channel %d",
             channel_id - 1);
  gtk_widget_set_tooltip_text(
      GTK_WIDGET(instance->status_channel_vertical_bar), tooltip_text_vbar);

  gchar status_label[32] = {0};
  g_snprintf(status_label, sizeof(status_label), "CH%d Status: 0",
             channel_id - 1);
  instance->status_channel_label = gtk_label_new(status_label);
  if (!GTK_IS_LABEL(instance->status_channel_label)) {
    g_critical("%s", FAILED_MALLOC_CHANNEL_STATUS);
    destroy_rpi_channel_status(instance);
    return NULL;
  }

  gtk_widget_set_sensitive(GTK_WIDGET(instance->status_channel_vertical_bar),
                           FALSE);
  gtk_widget_set_sensitive(instance->status_channel_label, FALSE);

  g_signal_connect(G_OBJECT(instance->activate_channel_check_box), "toggled",
                   G_CALLBACK(on_status_check_box_toggled), instance);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_frame_set_child(GTK_FRAME(instance->frame), vbox);

  gtk_widget_set_halign(GTK_WIDGET(instance->status_channel_vertical_bar),
                        GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(vbox),
                 GTK_WIDGET(instance->status_channel_vertical_bar));

  gtk_widget_set_halign(instance->activate_channel_check_box, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(vbox), instance->activate_channel_check_box);

  gtk_widget_set_halign(instance->status_channel_label, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(vbox), instance->status_channel_label);

  return instance;
}

void show_rpi_channel_status(RPIChannelStatus *instance) {
  if (instance && GTK_IS_FRAME(instance->frame)) {
    gtk_widget_set_visible(instance->frame, TRUE);
  }
}

void hide_rpi_channel_status(RPIChannelStatus *instance) {
  if (instance && GTK_IS_FRAME(instance->frame)) {
    gtk_widget_set_visible(instance->frame, FALSE);
  }
}

GtkWidget *get_frame_from_rpi_channel_status(RPIChannelStatus *instance) {
  if (instance && GTK_IS_FRAME(instance->frame)) {
    return instance->frame;
  }
  return NULL;
}

void rpi_channel_status_set_value(RPIChannelStatus *instance, gint value) {
  if (instance) {
    instance->current_value = value;
    if (gtk_check_button_get_active(
            GTK_CHECK_BUTTON(instance->activate_channel_check_box))) {
      gtk_vb_set_state(instance->status_channel_vertical_bar, value);
      gchar status_label[32] = {0};
      g_snprintf(status_label, sizeof(status_label), "CH%d Status: %d",
                 instance->channel_id - 1, value);
      gtk_label_set_text(GTK_LABEL(instance->status_channel_label),
                         status_label);
    }
  }
}

void destroy_rpi_channel_status(RPIChannelStatus *instance) {
  if (instance) {
    if (GTK_IS_FRAME(instance->frame)) {
      instance->frame = NULL;
    }

    instance->status_channel_label = NULL;
    instance->activate_channel_check_box = NULL;
    instance->status_channel_vertical_bar = NULL;

    g_free(instance);
  }
}
