/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_sqlite.c
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
#include "rpi_settings_sqlite.h"
#include "rpi_settings_sqlite_read.h"
#include "rpi_settings_sqlite_write.h"

static guint sqlite_write_prompt_adapter(const gchar *no_prompt) {
  return (guint)rpi_write_no_prompt_settings_sqlite(no_prompt);
}

static guint sqlite_write_address_adapter(const gchar *ip_address) {
  return (guint)rpi_write_ip_address_settings_sqlite(ip_address);
}

static guint sqlite_write_port_adapter(const gchar *port_number) {
  return (guint)rpi_write_port_number_settings_sqlite(port_number);
}

static guint sqlite_write_exit_adapter(const gchar *no_exit) {
  return (guint)rpi_write_no_exit_settings_sqlite(no_exit);
}

static const SettingsStorageProvider sqlite_storage_provider = {
    .read_prompt = rpi_read_no_prompt_settings_sqlite,
    .read_address = rpi_read_ip_address_settings_sqlite,
    .read_port = rpi_read_port_number_settings_sqlite,
    .read_exit = rpi_read_no_exit_settings_sqlite,
    .write_prompt = sqlite_write_prompt_adapter,
    .write_address = sqlite_write_address_adapter,
    .write_port = sqlite_write_port_adapter,
    .write_exit = sqlite_write_exit_adapter};

const SettingsStorageProvider *rpi_settings_sqlite_get_provider(void) {
  return &sqlite_storage_provider;
}
