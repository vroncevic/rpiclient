/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_exit_dialog_gtk3.c
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
#include "rpi_exit_dialog.h"
#include "../rpi_config.h"

#if RPI_VERBOSE == 1
static const gchar *const EXIT_FROM_RPI_CLIENT_EXIT_DIALOG =
    "Exit action by exit dialog.\n";
static const gchar *const DO_NOT_EXIT_FROM_RPI_CLIENT_EXIT_DIALOG =
    "No exit action by exit dialog.\n";
#endif

static const gchar *const MISSING_PARENT_EXIT_DIALOG =
    "Missing parent widget parameter for exit dialog.\n";
static const gchar *const FAILED_MALLOC_EXIT_DIALOG =
    "Failed to allocate memory for exit dialog.\n";

static const gchar *const TITLE_EXIT_DIALOG = "Confirm Exit";
static const gchar *const TEXT_LABEL_EXIT_DIALOG = "Exit from RPIClient?";
static const gchar *const TEXT_OK_BUTTON_EXIT_DIALOG = "Ok";
static const gchar *const TEXT_CANCEL_BUTTON_EXIT_DIALOG = "Cancel";
static const gint LABEL_MARGIN_EXIT_DIALOG = 20;

//////////////////////////////////////////////////////////////////////////////
/// @brief Exit dialog complex widget
///   dialog - Gtk confirm dialog widget
///   content_area - Gtk dialog content area widget
///   label - Gtk label widget
struct _ExitDialog {
  GtkWidget *dialog;
  GtkWidget *content_area;
  GtkWidget *label;
};

ExitDialog *new_exit_dialog(GtkWidget *parent) {
  if (!parent) {
    g_critical("%s", MISSING_PARENT_EXIT_DIALOG);
    return NULL;
  }

  ExitDialog *instance = g_malloc(sizeof(ExitDialog));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_EXIT_DIALOG);
    return NULL;
  }

  instance->dialog = gtk_dialog_new_with_buttons(
      TITLE_EXIT_DIALOG, GTK_WINDOW(parent), GTK_DIALOG_DESTROY_WITH_PARENT,
      TEXT_OK_BUTTON_EXIT_DIALOG, GTK_RESPONSE_ACCEPT,
      TEXT_CANCEL_BUTTON_EXIT_DIALOG, GTK_RESPONSE_REJECT, NULL);

  if (!GTK_IS_DIALOG(instance->dialog)) {
    g_critical("%s", FAILED_MALLOC_EXIT_DIALOG);
    destroy_exit_dialog(instance);
    return NULL;
  }

  instance->content_area =
      gtk_dialog_get_content_area(GTK_DIALOG(instance->dialog));

  if (!GTK_WIDGET(instance->content_area)) {
    g_critical("%s", FAILED_MALLOC_EXIT_DIALOG);
    destroy_exit_dialog(instance);
    return NULL;
  }

  instance->label = gtk_label_new(TEXT_LABEL_EXIT_DIALOG);

  if (!GTK_IS_LABEL(instance->label)) {
    g_critical("%s", FAILED_MALLOC_EXIT_DIALOG);
    destroy_exit_dialog(instance);
    return NULL;
  }

  gtk_widget_set_margin_top(GTK_WIDGET(instance->label),
                            LABEL_MARGIN_EXIT_DIALOG);
  gtk_widget_set_margin_bottom(GTK_WIDGET(instance->label),
                               LABEL_MARGIN_EXIT_DIALOG);
  gtk_widget_set_margin_start(GTK_WIDGET(instance->label),
                              LABEL_MARGIN_EXIT_DIALOG);
  gtk_widget_set_margin_end(GTK_WIDGET(instance->label),
                            LABEL_MARGIN_EXIT_DIALOG);
  gtk_container_add(GTK_CONTAINER(instance->content_area),
                    GTK_WIDGET(instance->label));

  return instance;
}

gint show_exit_dialog(ExitDialog *instance) {
  if (instance && GTK_IS_DIALOG(instance->dialog)) {
    if (!gtk_widget_get_visible(GTK_WIDGET(instance->dialog))) {
      gtk_widget_show_all(GTK_WIDGET(instance->dialog));
      gint result = gtk_dialog_run(GTK_DIALOG(instance->dialog));

      if (result == GTK_RESPONSE_ACCEPT) {
        hide_exit_dialog(instance);
#if RPI_VERBOSE == 1
        g_debug("%s", EXIT_FROM_RPI_CLIENT_EXIT_DIALOG);
#endif
        return CLOSE_ON_EXIT_DIALOG;
      }

      hide_exit_dialog(instance);
    }
  }

#if RPI_VERBOSE == 1
  g_debug("%s", DO_NOT_EXIT_FROM_RPI_CLIENT_EXIT_DIALOG);
#endif

  return NOT_CLOSE_ON_EXIT_DIALOG;
}

void hide_exit_dialog(ExitDialog *instance) {
  if (instance && GTK_IS_DIALOG(instance->dialog)) {
    if (gtk_widget_get_visible(GTK_WIDGET(instance->dialog))) {
      gtk_widget_hide(GTK_WIDGET(instance->dialog));
    }
  }
}

void destroy_exit_dialog(ExitDialog *instance) {
  if (instance) {
    if (GTK_IS_DIALOG(instance->dialog)) {
      gtk_widget_destroy(GTK_WIDGET(instance->dialog));
      instance->dialog = NULL;
    }

    instance->content_area = NULL;
    instance->label = NULL;
    g_free(instance);
  }
}
