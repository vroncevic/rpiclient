/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_misc.h
 * Copyright (C) 2016 - 2025 Vladimir Roncevic <elektron.ronca@gmail.com>
 *
 * rpiclient-gtk is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rpiclient-gtk is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <gtk/gtk.h>

//////////////////////////////////////////////////////////////////////////////
/// @brief Error messqge with supported Gtk versions.
#define GTK_SUPPORTED_VERSION "Supported GTK+ version: gtk+-3.0 gtk+-4.0!"

//////////////////////////////////////////////////////////////////////////////
/// @brief Checks is gtk+ widget visible or not
/// @param widget is pointer to gtk+ widget (GtkWidget)
/// @return TRUE - visible | FALSE - not visible
gboolean rpi_is_widget_visible_misc(const GtkWidget* widget);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets gtk+ widget to be visible or not
/// @param widget is pointer to gtk+ widget (GtkWidget)
/// @param visible is flag for show/hide widget (TRUE - show, FALSE - hide)
void rpi_set_visible_widget_misc(GtkWidget* widget, gboolean visible);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy gtk+ widget (remove from UI and free resources)
/// @param widget is pointer to gtk+ widget (GtkWidget)
void rpi_destroy_widget_misc(GtkWidget* widget);
