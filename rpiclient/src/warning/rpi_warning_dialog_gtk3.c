/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_warning_dialog_gtk3.c
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
#include "rpi_warning_dialog.h"
#include "../rpi_config.h"

const gchar *const TEXT_SETUP_CONNECTION_SETTINGS_WARNING_DIALOG =
    "Please set ip address, port of server !";
const gchar *const TEXT_SETUP_CONNECTION_WARNING_DIALOG =
    "Please make connection\nGo to Option > Connection";
const gchar *const TEXT_ERROR_WARNING_DIALOG = "There was an error !";

#if RPI_VERBOSE == 1
static const gchar *const CLOSE_WARNING_DIALOG = "Close warning dialog.\n";
#endif

static const gchar *const MISSING_PARENT_WARNING_DIALOG =
    "Missing parent widget parameter for warning dialog.\n";
static const gchar *const MISSING_MESSAGE_WARNING_DIALOG =
    "Missing message parameter for warning dialog.\n";
static const gchar *const FAILED_MALLOC_WARNING_DIALOG =
    "Failed to allocate memory for warning dialog.\n";

//////////////////////////////////////////////////////////////////////////////
/// @brief Warning dialog complex widget
///   dialog - Gtk warning dialog widget
struct _WarningDialog {
  GtkWidget *dialog;
};

WarningDialog *new_warning_dialog(GtkWidget *parent, const gchar *msg) {
  if (!parent) {
    g_critical("%s", MISSING_PARENT_WARNING_DIALOG);
    return NULL;
  }

  if (!msg) {
    g_critical("%s", MISSING_MESSAGE_WARNING_DIALOG);
    return NULL;
  }

  WarningDialog *instance = g_malloc(sizeof(WarningDialog));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_WARNING_DIALOG);
    return NULL;
  }

  instance->dialog =
      gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_DESTROY_WITH_PARENT,
                             GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", msg);

  if (!GTK_IS_MESSAGE_DIALOG(instance->dialog)) {
    g_critical("%s", FAILED_MALLOC_WARNING_DIALOG);
    destroy_warning_dialog(instance);
    return NULL;
  }

  return instance;
}

void show_warning_dialog(WarningDialog *instance) {
  if (instance && GTK_IS_MESSAGE_DIALOG(instance->dialog)) {
    if (!gtk_widget_get_visible(GTK_WIDGET(instance->dialog))) {
      gtk_widget_show_all(GTK_WIDGET(instance->dialog));
      gint result = gtk_dialog_run(GTK_DIALOG(instance->dialog));

      if (result == GTK_RESPONSE_CLOSE || result == GTK_RESPONSE_DELETE_EVENT) {
        hide_warning_dialog(instance);
#if RPI_VERBOSE == 1
        g_debug("%s", CLOSE_WARNING_DIALOG);
#endif
      }
    }
  }
}

void hide_warning_dialog(WarningDialog *instance) {
  if (instance && GTK_IS_MESSAGE_DIALOG(instance->dialog)) {
    if (gtk_widget_get_visible(GTK_WIDGET(instance->dialog))) {
      gtk_widget_hide(GTK_WIDGET(instance->dialog));
    }
  }
}

void destroy_warning_dialog(WarningDialog *instance) {
  if (instance) {
    if (GTK_IS_MESSAGE_DIALOG(instance->dialog)) {
      gtk_widget_destroy(GTK_WIDGET(instance->dialog));
      instance->dialog = NULL;
    }

    g_free(instance);
  }
}
