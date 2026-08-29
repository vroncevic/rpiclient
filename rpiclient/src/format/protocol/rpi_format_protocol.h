/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_format_protocol.h
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

G_BEGIN_DECLS

//////////////////////////////////////////////////////////////////////////////
/// @brief Number of RPI channels supported in protocol
enum { USED_CHANNELS_FORMAT = 8 };

//////////////////////////////////////////////////////////////////////////////
/// @brief RPI channels protocol complex structure
typedef struct _RPIChannels RPIChannels;

//////////////////////////////////////////////////////////////////////////////
/// @brief Initializes RPI channels structure with formatted commands
/// @param instance Pointer to RPIChannels structure
void rpi_format_init(RPIChannels *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Frees dynamic memory allocated inside RPI channels structure
/// @param instance Pointer to RPIChannels structure
void rpi_format_free(RPIChannels *instance);

G_END_DECLS
