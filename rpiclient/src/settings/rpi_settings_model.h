/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_model.h
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
/// @brief Settings configuration structure
typedef struct _SettingsConfig SettingsConfig;

//////////////////////////////////////////////////////////////////////////////
/// @brief Construct SettingsConfig entity model
/// @param prompt Initial prompt state string
/// @param address Initial server address string
/// @param port Initial server port string
/// @param exit_val Initial exit confirmation string
/// @return Pointer to allocated SettingsConfig | NULL
SettingsConfig *settings_model_new(const gchar *prompt, const gchar *address,
                                   const gchar *port, const gchar *exit_val);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets prompt status
/// @param instance Pointer to SettingsConfig entity model
/// @return TRUE if prompt is enabled, FALSE otherwise
gboolean is_prompt_enabled_settings(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets prompt status
/// @param instance Pointer to SettingsConfig entity model
/// @param no_prompt_state Boolean state for prompt
void set_prompt_enabled_settings(SettingsConfig *instance,
                                 const gboolean no_prompt_state);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets server IP address
/// @param instance Pointer to SettingsConfig entity model
/// @return Allocated IP address string | NULL
gchar *get_server_ip_address_from_settings(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets server IP address
/// @param instance Pointer to SettingsConfig entity model
/// @param ip_address IP address string
void set_server_ip_address_settings(SettingsConfig *instance,
                                    const gchar *ip_address);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets server port number
/// @param instance Pointer to SettingsConfig entity model
/// @return Allocated port number string | NULL
gchar *get_server_port_number_from_settings(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets server port number
/// @param instance Pointer to SettingsConfig entity model
/// @param port_number Port number string
void set_server_port_number_settings(SettingsConfig *instance,
                                     const gchar *port_number);

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets exit dialog status
/// @param instance Pointer to SettingsConfig entity model
/// @return TRUE if exit dialog is enabled, FALSE otherwise
gboolean is_exit_enabled_settings(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Sets exit dialog status
/// @param instance Pointer to SettingsConfig entity model
/// @param no_exit_state Boolean state for exit confirmation
void set_exit_enabled_settings(SettingsConfig *instance,
                               const gboolean no_exit_state);

//////////////////////////////////////////////////////////////////////////////
/// @brief Internal getter for raw prompt string
/// @param instance Pointer to SettingsConfig entity model
/// @return Const pointer to internal prompt string
const gchar *settings_model_get_prompt_raw(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Internal getter for raw address string
/// @param instance Pointer to SettingsConfig entity model
/// @return Const pointer to internal address string
const gchar *settings_model_get_address_raw(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Internal getter for raw port string
/// @param instance Pointer to SettingsConfig entity model
/// @return Const pointer to internal port string
const gchar *settings_model_get_port_raw(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Internal getter for raw exit confirmation string
/// @param instance Pointer to SettingsConfig entity model
/// @return Const pointer to internal exit confirmation string
const gchar *settings_model_get_exit_raw(const SettingsConfig *instance);

//////////////////////////////////////////////////////////////////////////////
/// @brief Free settings configuration model memory
/// @param instance Pointer to SettingsConfig entity model
void settings_free(SettingsConfig *instance);

G_END_DECLS
