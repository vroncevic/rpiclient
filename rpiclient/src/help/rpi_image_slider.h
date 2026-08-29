/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_image_slider.h
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

#include <gtk/gtk.h>

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Image slider complex widget
typedef struct _ImageSlider ImageSlider;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct complex widget image slider
/// @return Image slider complex widget pointer | NULL
ImageSlider *new_image_slider(void);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets fixed container from image slider complex widget
/// @param instance is pointer to complex widget image slider
/// @return Fixed container widget | NULL
GtkWidget *get_fixed_image_slider(ImageSlider *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Destroy image slider complex widget
/// @param instance is pointer to complex widget image slider
void destroy_image_slider(ImageSlider *instance);

G_END_DECLS
