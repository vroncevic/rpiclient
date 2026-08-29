/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_vertical_bar.h
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
#pragma once

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_VB(obj)                                                            \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), gtk_vb_get_type(), GtkVB))
#define GTK_IS_VB(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), gtk_vb_get_type()))

//////////////////////////////////////////////////////////////////////////////
/// @brief Vertical bar custom widget
///   parent_class - Gtk drawing area
///   sel - Number of selected cells in vertical bar
typedef struct {
  GtkDrawingArea parent_instance;
  gint sel;
} GtkVB;

//////////////////////////////////////////////////////////////////////////////
/// @brief Vertical bar custom widget class
///   parent_class - Gtk drawing area class
typedef struct {
  GtkDrawingAreaClass parent_class;
} GtkVBClass;

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets type of custom widget vertical bar
/// @return Unique identifier for vertical bar type
GType gtk_vb_get_type(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct custom widget vertical bar
/// @return Vertical bar custom widget pointer | NULL
GtkWidget *gtk_vb_new(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets state for custom widget vertical bar
/// @param instance is pointer to complex widget vertical bar
/// @param num is number of selected cells in vertical bar
void gtk_vb_set_state(GtkVB *instance, gint num);

G_END_DECLS
