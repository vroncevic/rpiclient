/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_image_slider_gtk4.c
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
#include "rpi_image_slider.h"
#include "../rpi_config.h"

static const gchar *const FAILED_MALLOC_IMAGE_SLIDER =
    "Failed to allocate memory for help slider.\n";

#define NUM_HELP_STEPS 5

typedef struct {
  const gchar *title;
  const gchar *body;
} HelpStep;

static const HelpStep HELP_STEPS[NUM_HELP_STEPS] = {
    {"1. Overview &amp; Architecture",
     "<b>Welcome to RPIClient!</b>\n\n"
     "• RPIClient is an interactive desktop application for remote control "
     "and real-time telemetry monitoring over a TCP network connection.\n"
     "• Features dedicated channels for bilateral data transfer and live status "
     "readings."},
    {"2. Network Connection",
     "<b>Connecting to Server</b>\n\n"
     "• Select <b>Option ➔ Connect</b> to establish a connection with the server.\n"
     "• IP address and Port can be configured in <b>Settings ➔ Network</b>, "
     "or prompted on connection."},
    {"3. Channel Control",
     "<b>Controlling Channels (Top Panel)</b>\n\n"
     "• Check the <b>Channel checkbox</b> in the top panel to enable the channel.\n"
     "• Adjust channel level (0 - 100) using the vertical scale or the "
     "numerical spin button."},
    {"4. Channel Status",
     "<b>Telemetry Monitoring (Bottom Panel)</b>\n\n"
     "• The bottom panel displays real-time telemetry received from the server.\n"
     "• The vertical LED bar displays the level graphically, accompanied by "
     "numerical readout."},
    {"5. Settings &amp; Exit",
     "<b>Settings &amp; Session Management</b>\n\n"
     "• Enable or disable the quit confirmation dialog in <b>Settings ➔ General</b>.\n"
     "• Disconnect the session via <b>Option ➔ Disconnect</b> or quit via "
     "<b>File ➔ Exit</b>."}};

//////////////////////////////////////////////////////////////////////////////
/// @brief Help slider complex widget for GTK4
struct _ImageSlider {
  GtkWidget *vbox;
  GtkWidget *card_frame;
  GtkWidget *label_title;
  GtkWidget *label_body;
  GtkWidget *hbox_nav;
  GtkWidget *button_prev;
  GtkWidget *label_step;
  GtkWidget *button_next;
  gint current_step;
};

static void update_help_step(ImageSlider *instance);
static void on_button_prev_clicked(GtkWidget *widget, gpointer data);
static void on_button_next_clicked(GtkWidget *widget, gpointer data);

static void update_help_step(ImageSlider *instance) {
  if (!instance) {
    return;
  }

  if (instance->current_step < 0) {
    instance->current_step = 0;
  }
  if (instance->current_step >= NUM_HELP_STEPS) {
    instance->current_step = NUM_HELP_STEPS - 1;
  }

  const HelpStep *step = &HELP_STEPS[instance->current_step];

  gchar markup_title[128] = {0};
  g_snprintf(markup_title, sizeof(markup_title),
             "<span font_desc='11' weight='bold'>%s</span>", step->title);
  gtk_label_set_markup(GTK_LABEL(instance->label_title), markup_title);

  gtk_label_set_markup(GTK_LABEL(instance->label_body), step->body);

  gchar step_text[32] = {0};
  g_snprintf(step_text, sizeof(step_text), "%d / %d",
             instance->current_step + 1, NUM_HELP_STEPS);
  gtk_label_set_text(GTK_LABEL(instance->label_step), step_text);

  gtk_widget_set_sensitive(instance->button_prev, instance->current_step > 0);
  gtk_widget_set_sensitive(instance->button_next,
                           instance->current_step < (NUM_HELP_STEPS - 1));
}

static void on_button_prev_clicked(GtkWidget *widget, gpointer data) {
  (void)widget;
  ImageSlider *instance = (ImageSlider *)data;
  if (instance && instance->current_step > 0) {
    instance->current_step--;
    update_help_step(instance);
  }
}

static void on_button_next_clicked(GtkWidget *widget, gpointer data) {
  (void)widget;
  ImageSlider *instance = (ImageSlider *)data;
  if (instance && instance->current_step < (NUM_HELP_STEPS - 1)) {
    instance->current_step++;
    update_help_step(instance);
  }
}

ImageSlider *new_image_slider(void) {
  ImageSlider *instance = g_malloc0(sizeof(ImageSlider));

  if (!instance) {
    g_critical("%s", FAILED_MALLOC_IMAGE_SLIDER);
    return NULL;
  }

  instance->current_step = 0;

  instance->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_start(instance->vbox, 12);
  gtk_widget_set_margin_end(instance->vbox, 12);
  gtk_widget_set_margin_top(instance->vbox, 12);
  gtk_widget_set_margin_bottom(instance->vbox, 12);

  instance->card_frame = gtk_frame_new(NULL);

  GtkWidget *card_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  gtk_widget_set_margin_start(card_box, 12);
  gtk_widget_set_margin_end(card_box, 12);
  gtk_widget_set_margin_top(card_box, 12);
  gtk_widget_set_margin_bottom(card_box, 12);

  instance->label_title = gtk_label_new(NULL);
  gtk_label_set_xalign(GTK_LABEL(instance->label_title), 0.0);
  gtk_box_append(GTK_BOX(card_box), instance->label_title);

  instance->label_body = gtk_label_new(NULL);
  gtk_label_set_use_markup(GTK_LABEL(instance->label_body), TRUE);
  gtk_label_set_wrap(GTK_LABEL(instance->label_body), TRUE);
  gtk_label_set_xalign(GTK_LABEL(instance->label_body), 0.0);
  gtk_label_set_yalign(GTK_LABEL(instance->label_body), 0.0);
  gtk_widget_set_size_request(instance->label_body, 420, 110);
  gtk_box_append(GTK_BOX(card_box), instance->label_body);

  gtk_frame_set_child(GTK_FRAME(instance->card_frame), card_box);
  gtk_box_append(GTK_BOX(instance->vbox), instance->card_frame);

  instance->hbox_nav = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_widget_set_halign(instance->hbox_nav, GTK_ALIGN_CENTER);

  instance->button_prev = gtk_button_new_with_label("« Back");
  gtk_widget_set_size_request(instance->button_prev, 85, 32);

  instance->label_step = gtk_label_new("1 / 5");
  gtk_widget_set_size_request(instance->label_step, 60, 32);

  instance->button_next = gtk_button_new_with_label("Next »");
  gtk_widget_set_size_request(instance->button_next, 85, 32);

  g_signal_connect(instance->button_prev, "clicked",
                   G_CALLBACK(on_button_prev_clicked), instance);
  g_signal_connect(instance->button_next, "clicked",
                   G_CALLBACK(on_button_next_clicked), instance);

  gtk_box_append(GTK_BOX(instance->hbox_nav), instance->button_prev);
  gtk_box_append(GTK_BOX(instance->hbox_nav), instance->label_step);
  gtk_box_append(GTK_BOX(instance->hbox_nav), instance->button_next);

  gtk_box_append(GTK_BOX(instance->vbox), instance->hbox_nav);

  update_help_step(instance);

  return instance;
}

GtkWidget *get_fixed_image_slider(ImageSlider *instance) {
  if (instance) {
    return instance->vbox;
  }
  return NULL;
}

void destroy_image_slider(ImageSlider *instance) {
  if (instance) {
    instance->vbox = NULL;
    g_free(instance);
  }
}
