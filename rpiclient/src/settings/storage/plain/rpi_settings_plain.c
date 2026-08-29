/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_plain.c
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
#include "rpi_settings_plain.h"
#include "rpi_settings_plain_read.h"
#include "rpi_settings_plain_write.h"

static const SettingsStorageProvider plain_storage_provider = {
    .read_prompt = rpi_read_prompt_settings_plain_file,
    .read_address = rpi_read_address_settings_plain_file,
    .read_port = rpi_read_port_settings_plain_file,
    .read_exit = rpi_read_exit_settings_plain_file,
    .write_prompt = rpi_write_prompt_settings_plain_file,
    .write_address = rpi_write_address_settings_plain_file,
    .write_port = rpi_write_port_settings_plain_file,
    .write_exit = rpi_write_exit_settings_plain_file};

const SettingsStorageProvider *rpi_settings_plain_get_provider(void) {
  return &plain_storage_provider;
}
