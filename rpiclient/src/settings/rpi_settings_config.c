/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_config.c
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
#include "../rpi_config.h"
#include "rpi_settings.h"

#include "rpi_settings_provider.h"
#include "rpi_settings_plain.h"
#include "rpi_settings_sqlite.h"

#if (RPI_USE_SETTINGS_PLAIN_CONFIG == 1) && (RPI_USE_SETTINGS_SQLITE3 == 1)
#error "You cannot use both RPI_USE_SETTINGS_PLAIN_CONFIG and RPI_USE_SETTINGS_SQLITE3 at the same time!"
#endif

#include "rpi_settings_config.h"

#if RPI_VERBOSE == 1
#define READ_SUCCESS_SETTINGS_CONFIG "Successfully read settings.\n"
#define WRITE_SUCCESS_SETTINGS_CONFIG "Successfully write settings.\n"
#endif

#define MISSING_PARAMETER_SETTINGS_CONFIG "Missing settings parameter for write settings configuration.\n"
#define MISSING_IP_ADDRESS_PARAMETER_SETTINGS_CONFIG "Missing server IP address parameter.\n"
#define MISSING_PORT_NUMBER_PARAMETER_SETTINGS_CONFIG "Missing server port number parameter.\n"

#define FAILED_MALLOC_READ_SETTINGS_CONFIG "Failed to allocate memory for read settings configuration.\n"
#define FAILED_READ_SETTINGS_CONFIG "Failed to read settings configuration.\n"
#define FAILED_WRITE_SETTINGS_CONFIG "Failed to write settings configuration.\n"

static const gchar* TRUE_SETTINGS_CONFIG = "true";
static const gchar* FALSE_SETTINGS_CONFIG = "false";

static guint sqlite_write_prompt_wrapper(const gchar* no_prompt) {
    return (guint)rpi_write_no_prompt_settings_sqlite(no_prompt);
}
static guint sqlite_write_address_wrapper(const gchar* ip_address) {
    return (guint)rpi_write_ip_address_settings_sqlite(ip_address);
}
static guint sqlite_write_port_wrapper(const gchar* port_number) {
    return (guint)rpi_write_port_number_settings_sqlite(port_number);
}
static guint sqlite_write_exit_wrapper(const gchar* no_exit) {
    return (guint)rpi_write_no_exit_settings_sqlite(no_exit);
}

static const SettingsStorageProvider plain_provider = {
    .read_prompt = rpi_read_prompt_settings_plain_file,
    .read_address = rpi_read_address_settings_plain_file,
    .read_port = rpi_read_port_settings_plain_file,
    .read_exit = rpi_read_exit_settings_plain_file,
    .write_prompt = rpi_write_prompt_settings_plain_file,
    .write_address = rpi_write_address_settings_plain_file,
    .write_port = rpi_write_port_settings_plain_file,
    .write_exit = rpi_write_exit_settings_plain_file
};

static const SettingsStorageProvider sqlite_provider = {
    .read_prompt = rpi_read_no_prompt_settings_sqlite,
    .read_address = rpi_read_ip_address_settings_sqlite,
    .read_port = rpi_read_port_number_settings_sqlite,
    .read_exit = rpi_read_no_exit_settings_sqlite,
    .write_prompt = sqlite_write_prompt_wrapper,
    .write_address = sqlite_write_address_wrapper,
    .write_port = sqlite_write_port_wrapper,
    .write_exit = sqlite_write_exit_wrapper
};

static const SettingsStorageProvider* get_current_provider(void)
{
#if (RPI_USE_SETTINGS_PLAIN_CONFIG == 1) && (RPI_USE_SETTINGS_SQLITE3 == 0)
    return &plain_provider;
#elif (RPI_USE_SETTINGS_PLAIN_CONFIG == 0) && (RPI_USE_SETTINGS_SQLITE3 == 1)
    return &sqlite_provider;
#else
    #error "Invalid configuration settings backend choice."
#endif
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Settings configuration structure
///   no_prompt - Configuration point for prompt
///   ip_address - Configuration point for server ip address
///   port_number - Configuration point for server port number
///   no_exit - Configuration point for exit
struct _SettingsConfig
{
    gchar *no_prompt;
    gchar *ip_address;
    gchar *port_number;
    gchar *no_exit;
};

SettingsConfig* settings_read(void)
{
    SettingsConfig* instance = g_malloc(sizeof(SettingsConfig));

    if (!instance)
    {
        g_critical(FAILED_MALLOC_READ_SETTINGS_CONFIG);
        return NULL;
    }

    const SettingsStorageProvider* provider = get_current_provider();
    gchar* prompt_config = provider->read_prompt();
    gchar* address_config = provider->read_address();
    gchar* port_config = provider->read_port();
    gchar* exit_config = provider->read_exit();
    gboolean nok_settings = (!prompt_config || !address_config || !port_config || !exit_config);

    if (nok_settings)
    {
        g_critical(FAILED_READ_SETTINGS_CONFIG);

        if (prompt_config) g_free(prompt_config);
        if (address_config) g_free(address_config);
        if (port_config) g_free(port_config);
        if (exit_config) g_free(exit_config);

        g_free(instance);
        return NULL;
    }

    instance->no_prompt = g_strdup(prompt_config);
    instance->ip_address = g_strdup(address_config);
    instance->port_number = g_strdup(port_config);
    instance->no_exit = g_strdup(exit_config);

    g_free(prompt_config);
    g_free(address_config);
    g_free(port_config);
    g_free(exit_config);

#if RPI_VERBOSE == 1
    g_debug(READ_SUCCESS_SETTINGS_CONFIG);
#endif

    return instance;
}

guint settings_write(const SettingsConfig* instance)
{
    if (!instance)
    {
        g_critical(MISSING_PARAMETER_SETTINGS_CONFIG);
        return FAILED_IO_SETTINGS_CONFIGURATION;
    }

    const SettingsStorageProvider* provider = get_current_provider();

#if (RPI_USE_SETTINGS_PLAIN_CONFIG == 1)
    guint err_flag = FAILED_SETTINGS_PLAIN;
#else
    guint err_flag = FAILED_SETTINGS_SQLITE;
#endif

    guint prompt_status = provider->write_prompt(instance->no_prompt);
    guint address_status = provider->write_address(instance->ip_address);
    guint port_status = provider->write_port(instance->port_number);
    guint exit_status = provider->write_exit(instance->no_exit);
    gboolean nok_write_status = (
        prompt_status == err_flag ||
        address_status == err_flag ||
        port_status == err_flag ||
        exit_status == err_flag
    );

    if (nok_write_status)
    {
        g_critical(FAILED_WRITE_SETTINGS_CONFIG);
        return FAILED_IO_SETTINGS_CONFIGURATION;
    }

#if RPI_VERBOSE == 1
    g_debug(WRITE_SUCCESS_SETTINGS_CONFIG);
#endif

    return SUCCESS_IO_SETTINGS_CONFIGURATION;
}

gboolean is_prompt_enabled_settings(const SettingsConfig* instance)
{
    return instance && (g_strstr_len(instance->no_prompt, -1, TRUE_SETTINGS_CONFIG) != NULL) ? TRUE : FALSE;
}

void set_prompt_enabled_settings(SettingsConfig* instance, const gboolean no_prompt_state)
{
    if (instance)
    {
        g_free(instance->no_prompt);
        if (no_prompt_state)
        {
            instance->no_prompt = g_strdup(TRUE_SETTINGS_CONFIG);
        }
        else
        {
            instance->no_prompt = g_strdup(FALSE_SETTINGS_CONFIG);
        }
    }
    else
    {
        g_critical(MISSING_PARAMETER_SETTINGS_CONFIG);
    }
}

gchar* get_server_ip_address_from_settings(const SettingsConfig* instance)
{
    return instance ? g_strdup(instance->ip_address) : NULL;
}

void set_server_ip_address_settings(SettingsConfig* instance, const gchar* ip_address)
{
    if (instance)
    {
        if (ip_address)
        {
            g_free(instance->ip_address);
            instance->ip_address = g_strdup(ip_address);
        }
        else
        {
            g_critical(MISSING_IP_ADDRESS_PARAMETER_SETTINGS_CONFIG);
        }
    }
    else
    {
        g_critical(MISSING_PARAMETER_SETTINGS_CONFIG);
    }
}

gchar* get_server_port_number_from_settings(const SettingsConfig* instance)
{
    return instance ? g_strdup(instance->port_number) : NULL;
}

void set_server_port_number_settings(SettingsConfig* instance, const gchar* port_number)
{
    if (instance)
    {
        if (port_number)
        {
            g_free(instance->port_number);
            instance->port_number = g_strdup(port_number);
        }
        else
        {
            g_critical(MISSING_PORT_NUMBER_PARAMETER_SETTINGS_CONFIG);
        }   
    }
    else
    {
        g_critical(MISSING_PARAMETER_SETTINGS_CONFIG);
    }
}

gboolean is_exit_enabled_settings(const SettingsConfig* instance)
{
    return instance && (g_strstr_len(instance->no_exit, -1, TRUE_SETTINGS_CONFIG) != NULL) ? TRUE : FALSE;
}

void set_exit_enabled_settings(SettingsConfig* instance, const gboolean no_exit_state)
{
    if (instance)
    {
        g_free(instance->no_exit);
        if (no_exit_state)
        {
            instance->no_exit = g_strdup(TRUE_SETTINGS_CONFIG);
        }
        else
        {
            instance->no_exit = g_strdup(FALSE_SETTINGS_CONFIG);
        }
    }
    else
    {
        g_critical(MISSING_PARAMETER_SETTINGS_CONFIG);
    }
}

void settings_free(SettingsConfig *instance)
{
    if (instance)
    {
        if (instance->no_prompt)
        {
            g_free(instance->no_prompt);
            instance->no_prompt = NULL;
        }

        if (instance->ip_address)
        {
            g_free(instance->ip_address);
            instance->ip_address = NULL;
        }

        if (instance->port_number)
        {
            g_free(instance->port_number);
            instance->port_number = NULL;
        }

        if (instance->no_exit)
        {
            g_free(instance->no_exit);
            instance->no_exit = NULL;
        }

        g_free(instance);
    }
}
