/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_home_frame.c
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
#include "rpi_home_frame.h"
#include "../../rpi_config.h"
#include "control/rpi_home_frame_control.h"
#include "status/rpi_home_frame_status.h"

static const gchar *const FAILED_MALLOC_RPI_HOME_FRAME =
    "Failed to allocate memory for home frame.\n";

//////////////////////////////////////////////////////////////////////////////
/// @brief Home frame complex widget
///   frame_home - Gtk frame widget for home container
///   frame_control - Control subframe instance
///   frame_status - Status subframe instance
///   vpaned - Gtk vpaned widget splitting control and status
struct _RPIHomeFrame {
  GtkWidget *frame_home;
  RPIHomeFrameControl *frame_control;
  RPIHomeFrameStatus *frame_status;
  GtkWidget *vpaned;
};

RPIHomeFrame *new_rpi_home_frame(void) {
  RPIHomeFrame *instance = g_malloc(sizeof(RPIHomeFrame));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_RPI_HOME_FRAME);
    return NULL;
  }

  instance->frame_home = gtk_frame_new(NULL);

  if (!GTK_IS_FRAME(instance->frame_home)) {
    g_critical("%s", FAILED_MALLOC_RPI_HOME_FRAME);
    g_free(instance);
    return NULL;
  }

  instance->vpaned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);

  if (!GTK_IS_PANED(instance->vpaned)) {
    g_critical("%s", FAILED_MALLOC_RPI_HOME_FRAME);
    gtk_widget_destroy(instance->frame_home);
    g_free(instance);
    return NULL;
  }

  gtk_frame_set_shadow_type(GTK_FRAME(instance->frame_home), GTK_SHADOW_IN);
  gtk_container_add(GTK_CONTAINER(instance->frame_home), instance->vpaned);

  instance->frame_control = new_rpi_home_frame_control();
  if (!instance->frame_control) {
    g_critical("%s", FAILED_MALLOC_RPI_HOME_FRAME);
    destroy_rpi_home_frame(instance);
    return NULL;
  }

  instance->frame_status = new_rpi_home_frame_status();
  if (!instance->frame_status) {
    g_critical("%s", FAILED_MALLOC_RPI_HOME_FRAME);
    destroy_rpi_home_frame(instance);
    return NULL;
  }

  gtk_paned_add1(GTK_PANED(instance->vpaned),
                 get_frame_from_rpi_home_frame_control(instance->frame_control));
  gtk_paned_add2(GTK_PANED(instance->vpaned),
                 get_frame_from_rpi_home_frame_status(instance->frame_status));

  return instance;
}

void show_rpi_home_frame(RPIHomeFrame *instance) {
  if (instance && GTK_IS_FRAME(instance->frame_home)) {
    if (!gtk_widget_get_visible(instance->frame_home)) {
      gtk_widget_show_all(instance->frame_home);
    }
  }
}

void hide_rpi_home_frame(RPIHomeFrame *instance) {
  if (instance && GTK_IS_FRAME(instance->frame_home)) {
    if (gtk_widget_get_visible(instance->frame_home)) {
      gtk_widget_hide(instance->frame_home);
    }
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
    if (GTK_IS_FRAME(instance->frame_home)) {
      gtk_widget_destroy(instance->frame_home);
      instance->frame_home = NULL;
    }

    if (instance->frame_control) {
      destroy_rpi_home_frame_control(instance->frame_control);
      instance->frame_control = NULL;
    }

    if (instance->frame_status) {
      destroy_rpi_home_frame_status(instance->frame_status);
      instance->frame_status = NULL;
    }

    instance->vpaned = NULL;
    g_free(instance);
  }
}
