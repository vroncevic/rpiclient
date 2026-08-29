/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_vertical_bar_gtk4.c
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
#include "rpi_vertical_bar.h"

G_DEFINE_TYPE(GtkVB, gtk_vb, GTK_TYPE_DRAWING_AREA)

static const gint PREFERRED_WIDTH_VB = 50;
static const gint PREFERRED_HEIGHT_VB = 200;
static const gint TOTAL_SEGMENTS_VB = 20;

static void gtk_vb_measure(GtkWidget *widget, GtkOrientation orientation,
                           gint for_size, gint *minimum, gint *natural,
                           gint *minimum_baseline, gint *natural_baseline);
static void gtk_vb_snapshot(GtkWidget *widget, GtkSnapshot *snapshot);

static void gtk_vb_class_init(GtkVBClass *klass) {
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  widget_class->measure = gtk_vb_measure;
  widget_class->snapshot = gtk_vb_snapshot;
}

static void gtk_vb_init(GtkVB *instance) {
  instance->sel = 0;
  gtk_widget_set_size_request(GTK_WIDGET(instance), PREFERRED_WIDTH_VB,
                              PREFERRED_HEIGHT_VB);
}

static void gtk_vb_measure(GtkWidget *widget, GtkOrientation orientation,
                           gint for_size, gint *minimum, gint *natural,
                           gint *minimum_baseline, gint *natural_baseline) {
  (void)widget;
  (void)for_size;
  (void)minimum_baseline;
  (void)natural_baseline;

  if (orientation == GTK_ORIENTATION_HORIZONTAL) {
    *minimum = PREFERRED_WIDTH_VB;
    *natural = PREFERRED_WIDTH_VB;
  } else {
    *minimum = PREFERRED_HEIGHT_VB;
    *natural = PREFERRED_HEIGHT_VB;
  }
}

GtkWidget *gtk_vb_new(void) { return g_object_new(gtk_vb_get_type(), NULL); }

void gtk_vb_set_state(GtkVB *instance, gint num) {
  g_return_if_fail(GTK_IS_VB(instance));
  if (num < 0) {
    num = 0;
  }
  if (num > 100) {
    num = 100;
  }
  instance->sel = num;
  gtk_widget_queue_draw(GTK_WIDGET(instance));
}

static void gtk_vb_snapshot(GtkWidget *widget, GtkSnapshot *snapshot) {
  GtkVB *instance = GTK_VB(widget);
  gint width = gtk_widget_get_width(widget);
  gint height = gtk_widget_get_height(widget);
  gint pos = instance->sel;
  gint rect_count = pos / 5;

  cairo_t *cr = gtk_snapshot_append_cairo(
      snapshot, &GRAPHENE_RECT_INIT(0, 0, (float)width, (float)height));

  // Dark sleek container background
  cairo_set_source_rgb(cr, 0.14, 0.14, 0.14);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);

  // Subtle border
  cairo_set_source_rgb(cr, 0.28, 0.28, 0.28);
  cairo_set_line_width(cr, 1.0);
  cairo_rectangle(cr, 0.5, 0.5, width - 1, height - 1);
  cairo_stroke(cr);

  for (gint i = 0; i < TOTAL_SEGMENTS_VB; i++) {
    if (i < rect_count) {
      // Active bright green LED
      cairo_set_source_rgb(cr, 0.35, 0.88, 0.20);
    } else {
      // Inactive dark green LED
      cairo_set_source_rgb(cr, 0.10, 0.24, 0.10);
    }

    cairo_rectangle(cr, 6, height - (i * 9) - 16, width - 12, 7);
    cairo_fill(cr);
  }

  cairo_destroy(cr);
}
