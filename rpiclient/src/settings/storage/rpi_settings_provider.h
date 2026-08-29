/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_provider.h
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
/// @brief Storage provider virtual function table interface for settings I/O
///   read_prompt - Function pointer to read prompt setting
///   read_address - Function pointer to read server address setting
///   read_port - Function pointer to read server port setting
///   read_exit - Function pointer to read exit confirmation setting
///   write_prompt - Function pointer to write prompt setting
///   write_address - Function pointer to write server address setting
///   write_port - Function pointer to write server port setting
///   write_exit - Function pointer to write exit confirmation setting
typedef struct _SettingsStorageProvider {
  gchar *(*read_prompt)(void);
  gchar *(*read_address)(void);
  gchar *(*read_port)(void);
  gchar *(*read_exit)(void);
  guint (*write_prompt)(const gchar *no_prompt);
  guint (*write_address)(const gchar *ip_address);
  guint (*write_port)(const gchar *port_number);
  guint (*write_exit)(const gchar *no_exit);
} SettingsStorageProvider;

G_END_DECLS
