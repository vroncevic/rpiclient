/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame_status_gtk4.c
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
#include "rpi_home_frame_status.h"

#if RPI_VERBOSE == 1
static const gchar *const SETUP_STATUS_CHANNEL =
    "Setup status channel id: %d.\n";
#endif

static const gchar *const FAILED_MALLOC_STATUS_FRAME =
    "Failed to allocate memory for status frame.\n";
static const gchar *STATUS_FRAME_LABEL = "RPI Status";

//////////////////////////////////////////////////////////////////////////////
/// @brief Status frame complex widget
///   frame_status - Gtk frame container widget
///   box_status - Gtk horizontal box container widget
///   channels_status - Array of channel status widgets
struct _RPIHomeFrameStatus {
  GtkWidget *frame_status;
  GtkWidget *box_status;
  RPIChannelStatus *channels_status[NUMBER_OF_CHANNELS_STATUS];
};

RPIHomeFrameStatus *new_rpi_home_frame_status(void) {
  RPIHomeFrameStatus *instance = g_malloc(sizeof(RPIHomeFrameStatus));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_STATUS_FRAME);
    return NULL;
  }

  instance->frame_status = gtk_frame_new(STATUS_FRAME_LABEL);
  if (!GTK_IS_FRAME(instance->frame_status)) {
    g_critical("%s", FAILED_MALLOC_STATUS_FRAME);
    g_free(instance);
    return NULL;
  }

  instance->box_status = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  if (!GTK_IS_BOX(instance->box_status)) {
    g_critical("%s", FAILED_MALLOC_STATUS_FRAME);
    destroy_rpi_home_frame_status(instance);
    return NULL;
  }

  gtk_box_set_homogeneous(GTK_BOX(instance->box_status), TRUE);
  gtk_frame_set_child(GTK_FRAME(instance->frame_status), instance->box_status);

  for (guint i = 0; i < NUMBER_OF_CHANNELS_STATUS; i++) {
    instance->channels_status[i] = new_rpi_channel_status((gint)(i + 1));
    gtk_box_append(
        GTK_BOX(instance->box_status),
        get_frame_from_rpi_channel_status(instance->channels_status[i]));

#if RPI_VERBOSE == 1
    g_debug(SETUP_STATUS_CHANNEL, i);
#endif
  }

  return instance;
}

GtkWidget *get_frame_from_rpi_home_frame_status(RPIHomeFrameStatus *instance) {
  if (instance && GTK_IS_FRAME(instance->frame_status)) {
    return instance->frame_status;
  }
  return NULL;
}

RPIChannelStatus *
rpi_home_frame_status_get_channel(RPIHomeFrameStatus *instance, guint index) {
  if (instance && index < NUMBER_OF_CHANNELS_STATUS) {
    return instance->channels_status[index];
  }
  return NULL;
}

void rpi_home_frame_status_update_channel(RPIHomeFrameStatus *instance,
                                         gint channel_id, gint value) {
  if (instance && channel_id >= 1 &&
      channel_id <= (gint)NUMBER_OF_CHANNELS_STATUS) {
    gint idx = channel_id - 1;
    if (instance->channels_status[idx]) {
      rpi_channel_status_set_value(instance->channels_status[idx], value);
    }
  }
}

void destroy_rpi_home_frame_status(RPIHomeFrameStatus *instance) {
  if (instance) {
    for (guint i = 0; i < NUMBER_OF_CHANNELS_STATUS; i++) {
      if (instance->channels_status[i]) {
        destroy_rpi_channel_status(instance->channels_status[i]);
        instance->channels_status[i] = NULL;
      }
    }

    instance->frame_status = NULL;
    instance->box_status = NULL;
    g_free(instance);
  }
}
