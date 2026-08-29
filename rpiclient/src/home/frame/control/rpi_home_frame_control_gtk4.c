/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame_control_gtk4.c
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
#include "../../../rpi_config.h"
#include "rpi_home_frame_control.h"

#if RPI_VERBOSE == 1
static const gchar *const SETUP_CONTROL_CHANNEL =
    "Setup control channel id: %d.\n";
#endif

static const gchar *const FAILED_MALLOC_CONTROL_FRAME =
    "Failed to allocate memory for control frame.\n";
static const gchar *CONTROL_FRAME_LABEL = "RPI Control";

//////////////////////////////////////////////////////////////////////////////
/// @brief Control frame complex widget
///   frame_control - Gtk frame container widget
///   box_control - Gtk horizontal box container widget
///   channels_control - Array of channel control widgets
struct _RPIHomeFrameControl {
  GtkWidget *frame_control;
  GtkWidget *box_control;
  RPIChannelControl *channels_control[NUMBER_OF_CHANNELS_CONTROL];
};

RPIHomeFrameControl *new_rpi_home_frame_control(void) {
  RPIHomeFrameControl *instance = g_malloc(sizeof(RPIHomeFrameControl));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_CONTROL_FRAME);
    return NULL;
  }

  instance->frame_control = gtk_frame_new(CONTROL_FRAME_LABEL);
  if (!GTK_IS_FRAME(instance->frame_control)) {
    g_critical("%s", FAILED_MALLOC_CONTROL_FRAME);
    g_free(instance);
    return NULL;
  }

  instance->box_control = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  if (!GTK_IS_BOX(instance->box_control)) {
    g_critical("%s", FAILED_MALLOC_CONTROL_FRAME);
    destroy_rpi_home_frame_control(instance);
    return NULL;
  }

  gtk_box_set_homogeneous(GTK_BOX(instance->box_control), TRUE);
  gtk_frame_set_child(GTK_FRAME(instance->frame_control),
                      instance->box_control);

  for (guint i = 0; i < NUMBER_OF_CHANNELS_CONTROL; i++) {
    instance->channels_control[i] = new_rpi_channel_control((gint)(i + 1));
    gtk_box_append(
        GTK_BOX(instance->box_control),
        get_frame_from_rpi_channel_control(instance->channels_control[i]));

#if RPI_VERBOSE == 1
    g_debug(SETUP_CONTROL_CHANNEL, i);
#endif
  }

  return instance;
}

GtkWidget *
get_frame_from_rpi_home_frame_control(RPIHomeFrameControl *instance) {
  if (instance && GTK_IS_FRAME(instance->frame_control)) {
    return instance->frame_control;
  }
  return NULL;
}

RPIChannelControl *
rpi_home_frame_control_get_channel(RPIHomeFrameControl *instance, guint index) {
  if (instance && index < NUMBER_OF_CHANNELS_CONTROL) {
    return instance->channels_control[index];
  }
  return NULL;
}

void destroy_rpi_home_frame_control(RPIHomeFrameControl *instance) {
  if (instance) {
    for (guint i = 0; i < NUMBER_OF_CHANNELS_CONTROL; i++) {
      if (instance->channels_control[i]) {
        destroy_rpi_channel_control(instance->channels_control[i]);
        instance->channels_control[i] = NULL;
      }
    }

    instance->frame_control = NULL;
    instance->box_control = NULL;
    g_free(instance);
  }
}
