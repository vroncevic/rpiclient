/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame_gtk4.c
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
#include "../../rpi_config.h"
#include "control/rpi_home_frame_control.h"
#include "rpi_home_frame.h"
#include "status/rpi_home_frame_status.h"

static const gchar *const FAILED_MALLOC_HOME_FRAME =
    "Failed to allocate memory for home frame.\n";

//////////////////////////////////////////////////////////////////////////////
/// @brief RPIHomeFrame frame complex widget
///   frame_home - Gtk widget for home frame
///   vpaned - Gtk widget for vertical paned
///   frame_control - Control subframe manager
///   frame_status - Status subframe manager
struct _RPIHomeFrame {
  GtkWidget *frame_home;
  GtkWidget *vpaned;
  RPIHomeFrameControl *frame_control;
  RPIHomeFrameStatus *frame_status;
};

RPIHomeFrame *new_rpi_home_frame(void) {
  RPIHomeFrame *instance = g_malloc(sizeof(RPIHomeFrame));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_HOME_FRAME);
    return NULL;
  }

  instance->frame_home = gtk_frame_new(NULL);
  if (!GTK_IS_FRAME(instance->frame_home)) {
    g_critical("%s", FAILED_MALLOC_HOME_FRAME);
    destroy_rpi_home_frame(instance);
    return NULL;
  }
  gtk_widget_set_vexpand(instance->frame_home, TRUE);
  gtk_widget_set_hexpand(instance->frame_home, TRUE);

  instance->vpaned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
  if (!GTK_IS_PANED(instance->vpaned)) {
    g_critical("%s", FAILED_MALLOC_HOME_FRAME);
    destroy_rpi_home_frame(instance);
    return NULL;
  }

  gtk_paned_set_shrink_start_child(GTK_PANED(instance->vpaned), FALSE);
  gtk_paned_set_shrink_end_child(GTK_PANED(instance->vpaned), FALSE);
  gtk_paned_set_resize_start_child(GTK_PANED(instance->vpaned), TRUE);
  gtk_paned_set_resize_end_child(GTK_PANED(instance->vpaned), TRUE);
  gtk_paned_set_position(GTK_PANED(instance->vpaned), 340);

  gtk_frame_set_child(GTK_FRAME(instance->frame_home), instance->vpaned);

  instance->frame_control = new_rpi_home_frame_control();
  if (!instance->frame_control) {
    g_critical("%s", FAILED_MALLOC_HOME_FRAME);
    destroy_rpi_home_frame(instance);
    return NULL;
  }

  instance->frame_status = new_rpi_home_frame_status();
  if (!instance->frame_status) {
    g_critical("%s", FAILED_MALLOC_HOME_FRAME);
    destroy_rpi_home_frame(instance);
    return NULL;
  }

  gtk_paned_set_start_child(
      GTK_PANED(instance->vpaned),
      get_frame_from_rpi_home_frame_control(instance->frame_control));
  gtk_paned_set_end_child(
      GTK_PANED(instance->vpaned),
      get_frame_from_rpi_home_frame_status(instance->frame_status));

  return instance;
}

void show_rpi_home_frame(RPIHomeFrame *instance) {
  if (instance && GTK_IS_FRAME(instance->frame_home)) {
    gtk_widget_set_visible(instance->frame_home, TRUE);
  }
}

void hide_rpi_home_frame(RPIHomeFrame *instance) {
  if (instance && GTK_IS_FRAME(instance->frame_home)) {
    gtk_widget_set_visible(instance->frame_home, FALSE);
  }
}

GtkWidget *get_frame_from_rpi_home_frame(RPIHomeFrame *instance) {
  if (instance && GTK_IS_FRAME(instance->frame_home)) {
    return instance->frame_home;
  }
  return NULL;
}

RPIHomeFrameControl *rpi_home_frame_get_control(RPIHomeFrame *instance) {
  if (instance) {
    return instance->frame_control;
  }
  return NULL;
}

RPIHomeFrameStatus *rpi_home_frame_get_status(RPIHomeFrame *instance) {
  if (instance) {
    return instance->frame_status;
  }
  return NULL;
}

void rpi_home_frame_update_channel_status(RPIHomeFrame *instance,
                                          gint channel_id, gint value) {
  if (instance && instance->frame_status) {
    rpi_home_frame_status_update_channel(instance->frame_status, channel_id,
                                         value);
  }
}

void destroy_rpi_home_frame(RPIHomeFrame *instance) {
  if (instance) {
    if (instance->frame_control) {
      destroy_rpi_home_frame_control(instance->frame_control);
      instance->frame_control = NULL;
    }

    if (instance->frame_status) {
      destroy_rpi_home_frame_status(instance->frame_status);
      instance->frame_status = NULL;
    }

    instance->frame_home = NULL;
    instance->vpaned = NULL;
    g_free(instance);
  }
}
