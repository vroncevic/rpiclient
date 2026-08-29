/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_exit_dialog_gtk4.c
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

//////////////////////////////////////////////////////////////////////////////
/// @brief Exit dialog complex widget for GTK4
///   dialog - Gtk confirm window widget
///   label - Gtk label widget
///   loop - Nested main loop for synchronous modal behavior
///   response - Response code (CLOSE_ON_EXIT_DIALOG or NOT_CLOSE_ON_EXIT_DIALOG)
struct _ExitDialog {
  GtkWidget *dialog;
  GtkWidget *label;
  GMainLoop *loop;
  gint response;
};

static void on_exit_response_ok(GtkWidget *btn, gpointer data);
static void on_exit_response_cancel(GtkWidget *btn, gpointer data);
static gboolean on_exit_window_close(GtkWindow *win, gpointer data);

static void on_exit_response_ok(GtkWidget *btn, gpointer data) {
  (void)btn;
  ExitDialog *instance = (ExitDialog *)data;
  if (instance) {
    instance->response = CLOSE_ON_EXIT_DIALOG;
    if (instance->loop && g_main_loop_is_running(instance->loop)) {
      g_main_loop_quit(instance->loop);
    }
  }
}

static void on_exit_response_cancel(GtkWidget *btn, gpointer data) {
  (void)btn;
  ExitDialog *instance = (ExitDialog *)data;
  if (instance) {
    instance->response = NOT_CLOSE_ON_EXIT_DIALOG;
    if (instance->loop && g_main_loop_is_running(instance->loop)) {
      g_main_loop_quit(instance->loop);
    }
  }
}

static gboolean on_exit_window_close(GtkWindow *win, gpointer data) {
  (void)win;
  ExitDialog *instance = (ExitDialog *)data;
  if (instance) {
    instance->response = NOT_CLOSE_ON_EXIT_DIALOG;
    if (instance->loop && g_main_loop_is_running(instance->loop)) {
      g_main_loop_quit(instance->loop);
    }
  }
  return TRUE;
}

ExitDialog *new_exit_dialog(GtkWidget *parent) {
  if (!parent) {
    g_critical("%s", MISSING_PARENT_EXIT_DIALOG);
    return NULL;
  }

  ExitDialog *instance = g_malloc0(sizeof(ExitDialog));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_EXIT_DIALOG);
    return NULL;
  }

  instance->dialog = gtk_window_new();

  if (!GTK_IS_WINDOW(instance->dialog)) {
    g_critical("%s", FAILED_MALLOC_EXIT_DIALOG);
    g_free(instance);
    return NULL;
  }

  gtk_window_set_title(GTK_WINDOW(instance->dialog), TITLE_EXIT_DIALOG);
  gtk_window_set_transient_for(GTK_WINDOW(instance->dialog),
                               GTK_WINDOW(parent));
  gtk_window_set_modal(GTK_WINDOW(instance->dialog), TRUE);
  gtk_window_set_default_size(GTK_WINDOW(instance->dialog), 300, 140);
  gtk_window_set_resizable(GTK_WINDOW(instance->dialog), FALSE);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
  gtk_widget_set_margin_start(vbox, 20);
  gtk_widget_set_margin_end(vbox, 20);
  gtk_widget_set_margin_top(vbox, 20);
  gtk_widget_set_margin_bottom(vbox, 20);

  instance->label = gtk_label_new(TEXT_LABEL_EXIT_DIALOG);
  gtk_widget_set_halign(instance->label, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(vbox), instance->label);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);

  GtkWidget *btn_ok = gtk_button_new_with_label(TEXT_OK_BUTTON_EXIT_DIALOG);
  GtkWidget *btn_cancel =
      gtk_button_new_with_label(TEXT_CANCEL_BUTTON_EXIT_DIALOG);
  gtk_widget_set_size_request(btn_ok, 80, 32);
  gtk_widget_set_size_request(btn_cancel, 80, 32);

  g_signal_connect(btn_ok, "clicked", G_CALLBACK(on_exit_response_ok),
                   instance);
  g_signal_connect(btn_cancel, "clicked", G_CALLBACK(on_exit_response_cancel),
                   instance);
  g_signal_connect(instance->dialog, "close-request",
                   G_CALLBACK(on_exit_window_close), instance);

  gtk_box_append(GTK_BOX(hbox), btn_ok);
  gtk_box_append(GTK_BOX(hbox), btn_cancel);
  gtk_box_append(GTK_BOX(vbox), hbox);

  gtk_window_set_child(GTK_WINDOW(instance->dialog), vbox);

  return instance;
}

gint show_exit_dialog(ExitDialog *instance) {
  if (instance && instance->dialog) {
    instance->response = NOT_CLOSE_ON_EXIT_DIALOG;
    gtk_window_present(GTK_WINDOW(instance->dialog));
    instance->loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(instance->loop);
    g_main_loop_unref(instance->loop);
    instance->loop = NULL;
    gtk_widget_set_visible(instance->dialog, FALSE);

#if RPI_VERBOSE == 1
    if (instance->response == CLOSE_ON_EXIT_DIALOG) {
      g_debug("%s", EXIT_FROM_RPI_CLIENT_EXIT_DIALOG);
    } else {
      g_debug("%s", DO_NOT_EXIT_FROM_RPI_CLIENT_EXIT_DIALOG);
    }
#endif
    return instance->response;
  }

  return NOT_CLOSE_ON_EXIT_DIALOG;
}

void hide_exit_dialog(ExitDialog *instance) {
  if (instance && instance->dialog) {
    gtk_widget_set_visible(instance->dialog, FALSE);
    if (instance->loop && g_main_loop_is_running(instance->loop)) {
      g_main_loop_quit(instance->loop);
    }
  }
}

void destroy_exit_dialog(ExitDialog *instance) {
  if (instance) {
    if (instance->dialog) {
      gtk_window_destroy(GTK_WINDOW(instance->dialog));
      instance->dialog = NULL;
    }

    if (instance->loop) {
      if (g_main_loop_is_running(instance->loop)) {
        g_main_loop_quit(instance->loop);
      }
      g_main_loop_unref(instance->loop);
      instance->loop = NULL;
    }

    instance->label = NULL;
    g_free(instance);
  }
}
