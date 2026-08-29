/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_warning_dialog_gtk4.c
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
    "Setup connection settings.\n";

#if RPI_VERBOSE == 1
static const gchar *const CLOSE_WARNING_DIALOG = "Close warning dialog.\n";
#endif

static const gchar *const FAILED_MALLOC_WARNING_DIALOG =
    "Failed to allocate memory for warning dialog.\n";

//////////////////////////////////////////////////////////////////////////////
/// @brief Warning dialog complex widget
///   dialog - Gtk window widget
///   label - Gtk label widget
///   button - Gtk button widget
struct _WarningDialog {
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
};

WarningDialog *new_warning_dialog(GtkWidget *parent, const gchar *msg) {
  WarningDialog *instance = g_malloc(sizeof(WarningDialog));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_WARNING_DIALOG);
    return NULL;
  }

  instance->dialog = gtk_window_new();

  if (!GTK_IS_WINDOW(instance->dialog)) {
    g_critical("%s", FAILED_MALLOC_WARNING_DIALOG);
    g_free(instance);
    return NULL;
  }

  gtk_window_set_title(GTK_WINDOW(instance->dialog), "Warning");
  gtk_window_set_default_size(GTK_WINDOW(instance->dialog), 300, 120);

  if (parent && GTK_IS_WINDOW(parent)) {
    gtk_window_set_transient_for(GTK_WINDOW(instance->dialog),
                                 GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(instance->dialog), TRUE);
  }

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  gtk_widget_set_margin_start(box, 16);
  gtk_widget_set_margin_end(box, 16);
  gtk_widget_set_margin_top(box, 16);
  gtk_widget_set_margin_bottom(box, 16);

  instance->label = gtk_label_new(msg);
  gtk_box_append(GTK_BOX(box), instance->label);

  instance->button = gtk_button_new_with_label("Close");
  g_signal_connect_swapped(instance->button, "clicked",
                           G_CALLBACK(hide_warning_dialog), instance);
  gtk_box_append(GTK_BOX(box), instance->button);

  gtk_window_set_child(GTK_WINDOW(instance->dialog), box);

  return instance;
}

void show_warning_dialog(WarningDialog *instance) {
  if (instance && instance->dialog) {
    gtk_window_present(GTK_WINDOW(instance->dialog));
  }
}

void hide_warning_dialog(WarningDialog *instance) {
  if (instance && instance->dialog) {
    gtk_widget_set_visible(instance->dialog, FALSE);
#if RPI_VERBOSE == 1
    g_debug("%s", CLOSE_WARNING_DIALOG);
#endif
  }
}

void destroy_warning_dialog(WarningDialog *instance) {
  if (instance) {
    if (instance->dialog) {
      gtk_window_destroy(GTK_WINDOW(instance->dialog));
      instance->dialog = NULL;
    }

    instance->label = NULL;
    instance->button = NULL;
    g_free(instance);
  }
}
