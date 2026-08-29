/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_model.c
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
#include "rpi_settings_model.h"

static const gchar *const MISSING_PARAMETER_SETTINGS_CONFIG =
    "Missing settings parameter for write settings configuration.\n";
static const gchar *const MISSING_IP_ADDRESS_PARAMETER_SETTINGS_CONFIG =
    "Missing server IP address parameter.\n";
static const gchar *const MISSING_PORT_NUMBER_PARAMETER_SETTINGS_CONFIG =
    "Missing server port number parameter.\n";

static const gchar *TRUE_SETTINGS_CONFIG = "true";
static const gchar *FALSE_SETTINGS_CONFIG = "false";

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings configuration entity data model
///   no_prompt - String state for prompt flag ("true" / "false")
///   ip_address - Server IP address string
///   port_number - Server port number string
///   no_exit - String state for exit confirmation flag ("true" / "false")
struct _SettingsConfig {
  gchar *no_prompt;
  gchar *ip_address;
  gchar *port_number;
  gchar *no_exit;
};

SettingsConfig *settings_model_new(const gchar *prompt, const gchar *address,
                                   const gchar *port, const gchar *exit_val) {
  SettingsConfig *instance = g_malloc(sizeof(SettingsConfig));
  if (!instance) {
    return NULL;
  }

  instance->no_prompt = g_strdup(prompt);
  instance->ip_address = g_strdup(address);
  instance->port_number = g_strdup(port);
  instance->no_exit = g_strdup(exit_val);

  return instance;
}

gboolean is_prompt_enabled_settings(const SettingsConfig *instance) {
  return instance && (g_strstr_len(instance->no_prompt, -1,
                                    TRUE_SETTINGS_CONFIG) != NULL)
             ? TRUE
             : FALSE;
}

void set_prompt_enabled_settings(SettingsConfig *instance,
                                 const gboolean no_prompt_state) {
  if (instance) {
    g_free(instance->no_prompt);
    if (no_prompt_state) {
      instance->no_prompt = g_strdup(TRUE_SETTINGS_CONFIG);
    } else {
      instance->no_prompt = g_strdup(FALSE_SETTINGS_CONFIG);
    }
  } else {
    g_critical("%s", MISSING_PARAMETER_SETTINGS_CONFIG);
  }
}

gchar *get_server_ip_address_from_settings(const SettingsConfig *instance) {
  return instance ? g_strdup(instance->ip_address) : NULL;
}

void set_server_ip_address_settings(SettingsConfig *instance,
                                    const gchar *ip_address) {
  if (instance) {
    if (ip_address) {
      g_free(instance->ip_address);
      instance->ip_address = g_strdup(ip_address);
    } else {
      g_critical("%s", MISSING_IP_ADDRESS_PARAMETER_SETTINGS_CONFIG);
    }
  } else {
    g_critical("%s", MISSING_PARAMETER_SETTINGS_CONFIG);
  }
}

gchar *get_server_port_number_from_settings(const SettingsConfig *instance) {
  return instance ? g_strdup(instance->port_number) : NULL;
}

void set_server_port_number_settings(SettingsConfig *instance,
                                     const gchar *port_number) {
  if (instance) {
    if (port_number) {
      g_free(instance->port_number);
      instance->port_number = g_strdup(port_number);
    } else {
      g_critical("%s", MISSING_PORT_NUMBER_PARAMETER_SETTINGS_CONFIG);
    }
  } else {
    g_critical("%s", MISSING_PARAMETER_SETTINGS_CONFIG);
  }
}

gboolean is_exit_enabled_settings(const SettingsConfig *instance) {
  return instance && (g_strstr_len(instance->no_exit, -1,
                                    TRUE_SETTINGS_CONFIG) != NULL)
             ? TRUE
             : FALSE;
}

void set_exit_enabled_settings(SettingsConfig *instance,
                               const gboolean no_exit_state) {
  if (instance) {
    g_free(instance->no_exit);
    if (no_exit_state) {
      instance->no_exit = g_strdup(TRUE_SETTINGS_CONFIG);
    } else {
      instance->no_exit = g_strdup(FALSE_SETTINGS_CONFIG);
    }
  } else {
    g_critical("%s", MISSING_PARAMETER_SETTINGS_CONFIG);
  }
}

const gchar *settings_model_get_prompt_raw(const SettingsConfig *instance) {
  return instance ? instance->no_prompt : NULL;
}

const gchar *settings_model_get_address_raw(const SettingsConfig *instance) {
  return instance ? instance->ip_address : NULL;
}

const gchar *settings_model_get_port_raw(const SettingsConfig *instance) {
  return instance ? instance->port_number : NULL;
}

const gchar *settings_model_get_exit_raw(const SettingsConfig *instance) {
  return instance ? instance->no_exit : NULL;
}

void settings_free(SettingsConfig *instance) {
  if (instance) {
    if (instance->no_prompt) {
      g_free(instance->no_prompt);
      instance->no_prompt = NULL;
    }

    if (instance->ip_address) {
      g_free(instance->ip_address);
      instance->ip_address = NULL;
    }

    if (instance->port_number) {
      g_free(instance->port_number);
      instance->port_number = NULL;
    }

    if (instance->no_exit) {
      g_free(instance->no_exit);
      instance->no_exit = NULL;
    }

    g_free(instance);
  }
}
