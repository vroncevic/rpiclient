/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_settings_plain.c
 * Copyright (C) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>
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
#include <stdio.h>
#include "../rpi_config.h"
#include "../resource/rpi_resource.h"
#include "rpi_settings.h"
#include "rpi_settings_user.h"
#include "rpi_settings_plain.h"

#if RPI_VERBOSE == 1
#define DIR_SETTINGS_PLAIN "Used configuration directory path: %s.\n"
#define OPEN_FILE_SETTINGS_PLAIN "Successfully opened configuration file: %s with mode: %s.\n"
#define READ_FILE_PARAMETER_SETTINGS_PLAIN "Read from file: %s parameter: %s.\n"
#define WRITE_FILE_PARAMETER_SETTINGS_PLAIN "Write to file: %s parameter: %s.\n"
#define CLOSE_FILE_SETTINGS_PLAIN "Successfully closed file: %s.\n"
#endif

#define MISSING_FILE_PARAMETERS_SETTINGS_PLAIN "Missing parameters for opening file.\n"
#define MISSING_FILE_NAME_SETTINGS_PLAIN "Missing name for opening file.\n"
#define MISSING_FILE_MODE_SETTINGS_PLAIN "Missing mode for opening file.\n"
#define MISSING_FILE_CONTENT_SETTINGS_PLAIN "Missing default configuration content in case of missing file.\n"
#define MISSING_PARAMETER_SETTINGS_PLAIN "Missing configuration parameter for write operation to file.\n"

#define FAILED_GET_CONFIGURATION_DIR_SETTINGS_PLAIN "Failed to get configuration directory path.\n"
#define FAILED_FILE_PATH_SETTINGS_PLAIN "Failed to get path for file.\n"
#define FAILED_FILE_OPEN_SETTINGS_PLAIN "Failed to open file.\n"
#define FAILED_FILE_READ_SETTINGS_PLAIN "Failed to read file.\n"
#define FAILED_FILE_WRITE_SETTINGS_PLAIN "Failed to write to file.\n"
#define FAILED_FILE_CLOSE_SETTINGS_PLAIN "Failed to close file.\n"

static const gchar* PROMPT_FILE_NAME_SETTINGS_PLAIN = "prompt.config";
static const gchar* SERVER_ADDRESS_FILE_NAME_SETTINGS_PLAIN = "server_address.config";
static const gchar* SERVER_PORT_FILE_NAME_SETTINGS_PLAIN = "server_port.config";
static const gchar* EXIT_FILE_NAME_SETTINGS_PLAIN = "exit.config";
static const gchar* DEFAULT_PROMPT_PARAMETER_SETTINGS_PLAIN = "false";
static const gchar* DEFAULT_SERVER_ADDRESS_PARAMETER_SETTINGS_PLAIN = "192.168.1.100";
static const gchar* DEFAULT_SERVER_PORT_PARAMETER_SETTINGS_PLAIN = "8888";
static const gchar* DEFAULT_EXIT_PARAMETER_SETTINGS_PLAIN = "true";
static const gchar* READ_MODE_SETTINGS_PLAIN = "rb";
static const gchar* WRITE_MODE_SETTINGS_PLAIN = "wb";

typedef struct
{
    gchar* name;
    gchar* mode;
    gchar* content;
} ConfigFile;

static FILE* rpi_open_settings_plain_file(const ConfigFile* config);
static void rpi_close_settings_plain_file(const ConfigFile* config, FILE* config_file);
static void rpi_free_settings_plain_file(ConfigFile* config);
static gchar* rpi_read_plain_file_helper(const gchar* name, const gchar* default_val);
static guint rpi_write_plain_file_helper(const gchar* name, const gchar* param, const gchar* default_val);

static gchar* rpi_read_plain_file_helper(const gchar* name, const gchar* default_val)
{
    ConfigFile setup =
    {
        .name = g_strdup(name),
        .mode = g_strdup(READ_MODE_SETTINGS_PLAIN),
        .content = g_strdup(default_val)
    };

    gchar* config_val = NULL;
    FILE* config_file = rpi_open_settings_plain_file(&setup);

    if (config_file)
    {
        config_val = g_malloc(64 * sizeof(gchar));
        if (!config_val)
        {
            g_critical(FAILED_FILE_READ_SETTINGS_PLAIN);
            rpi_close_settings_plain_file(&setup, config_file);
            rpi_free_settings_plain_file(&setup);
            return NULL;
        }

        if (fscanf(config_file, "%63s", config_val) != 1)
        {
            g_critical(FAILED_FILE_READ_SETTINGS_PLAIN);
            g_free(config_val);
            config_val = NULL;
            rpi_close_settings_plain_file(&setup, config_file);
            rpi_free_settings_plain_file(&setup);
            return NULL;
        }

#if RPI_VERBOSE == 1
        g_debug(READ_FILE_PARAMETER_SETTINGS_PLAIN, name, config_val);
#endif

        rpi_close_settings_plain_file(&setup, config_file);
    }

    rpi_free_settings_plain_file(&setup);
    return config_val;
}

static guint rpi_write_plain_file_helper(const gchar* name, const gchar* param, const gchar* default_val)
{
    if (!param)
    {
        g_critical(MISSING_PARAMETER_SETTINGS_PLAIN);
        return FAILED_SETTINGS_PLAIN;
    }

    ConfigFile setup =
    {
        .name = g_strdup(name),
        .mode = g_strdup(WRITE_MODE_SETTINGS_PLAIN),
        .content = g_strdup(default_val)
    };

    FILE* config_file = rpi_open_settings_plain_file(&setup);
    if (!config_file)
    {
        rpi_free_settings_plain_file(&setup);
        return FAILED_SETTINGS_PLAIN;
    }

    gint status_put = fputs(param, config_file);
    gint status_flash = fflush(config_file);

    if (status_put < 0 || status_flash < 0)
    {
        g_critical(FAILED_FILE_WRITE_SETTINGS_PLAIN);
        rpi_close_settings_plain_file(&setup, config_file);
        rpi_free_settings_plain_file(&setup);
        return FAILED_SETTINGS_PLAIN;
    }

#if RPI_VERBOSE == 1
    g_debug(WRITE_FILE_PARAMETER_SETTINGS_PLAIN, name, param);
#endif

    rpi_close_settings_plain_file(&setup, config_file);
    rpi_free_settings_plain_file(&setup);
    return SUCCESS_SETTINGS_PLAIN;
}

gchar* rpi_read_prompt_settings_plain_file(void)
{
    return rpi_read_plain_file_helper(PROMPT_FILE_NAME_SETTINGS_PLAIN, DEFAULT_PROMPT_PARAMETER_SETTINGS_PLAIN);
}

guint rpi_write_prompt_settings_plain_file(const gchar* prompt_config)
{
    return rpi_write_plain_file_helper(PROMPT_FILE_NAME_SETTINGS_PLAIN, prompt_config, DEFAULT_PROMPT_PARAMETER_SETTINGS_PLAIN);
}

gchar* rpi_read_address_settings_plain_file(void)
{
    return rpi_read_plain_file_helper(SERVER_ADDRESS_FILE_NAME_SETTINGS_PLAIN, DEFAULT_SERVER_ADDRESS_PARAMETER_SETTINGS_PLAIN);
}

guint rpi_write_address_settings_plain_file(const gchar* address_config)
{
    return rpi_write_plain_file_helper(SERVER_ADDRESS_FILE_NAME_SETTINGS_PLAIN, address_config, DEFAULT_SERVER_ADDRESS_PARAMETER_SETTINGS_PLAIN);
}

gchar* rpi_read_port_settings_plain_file(void)
{
    return rpi_read_plain_file_helper(SERVER_PORT_FILE_NAME_SETTINGS_PLAIN, DEFAULT_SERVER_PORT_PARAMETER_SETTINGS_PLAIN);
}

guint rpi_write_port_settings_plain_file(const gchar* port_config)
{
    return rpi_write_plain_file_helper(SERVER_PORT_FILE_NAME_SETTINGS_PLAIN, port_config, DEFAULT_SERVER_PORT_PARAMETER_SETTINGS_PLAIN);
}

gchar* rpi_read_exit_settings_plain_file(void)
{
    return rpi_read_plain_file_helper(EXIT_FILE_NAME_SETTINGS_PLAIN, DEFAULT_EXIT_PARAMETER_SETTINGS_PLAIN);
}

guint rpi_write_exit_settings_plain_file(const gchar* exit_config)
{
    return rpi_write_plain_file_helper(EXIT_FILE_NAME_SETTINGS_PLAIN, exit_config, DEFAULT_EXIT_PARAMETER_SETTINGS_PLAIN);
}

static FILE* rpi_open_settings_plain_file(const ConfigFile* config)
{
    if (!config)
    {
        g_critical(MISSING_FILE_PARAMETERS_SETTINGS_PLAIN);
        return NULL;
    }

    if (!config->name)
    {
        g_critical(MISSING_FILE_NAME_SETTINGS_PLAIN);
        return NULL;
    }

    if (!config->mode)
    {
        g_critical(MISSING_FILE_MODE_SETTINGS_PLAIN);
        return NULL;
    }

    if (!config->content)
    {
        g_critical(MISSING_FILE_CONTENT_SETTINGS_PLAIN);
        return NULL;
    }

    config_dir_path = rpi_get_config_dir();
    if (!config_dir_path)
    {
        g_critical(FAILED_GET_CONFIGURATION_DIR_SETTINGS_PLAIN);
        return NULL;
    }

#if RPI_VERBOSE == 1
    g_debug(DIR_SETTINGS_PLAIN, config_dir_path);
#endif

    gchar *config_file_path = rpi_get_config_file_path(config->name, config->content);
    if (!config_file_path)
    {
        g_critical(FAILED_FILE_PATH_SETTINGS_PLAIN);
        return NULL;
    }

    FILE *config_file = fopen(config_file_path, config->mode);
    if (!config_file)
    {
        g_critical(FAILED_FILE_OPEN_SETTINGS_PLAIN);
        g_free(config_file_path);
        return NULL;
    }

#if RPI_VERBOSE == 1
    g_debug(OPEN_FILE_SETTINGS_PLAIN, config_file_path, config->mode);
#endif

    g_free(config_file_path);
    return config_file;
}

static void rpi_close_settings_plain_file(const ConfigFile* config, FILE* config_file)
{
    if (config_file)
    {
        gint status = fclose(config_file);
        if (status != 0)
        {
            g_critical(FAILED_FILE_CLOSE_SETTINGS_PLAIN);
        }

#if RPI_VERBOSE == 1
        if (config && config->name && status == 0)
        {
            g_debug(CLOSE_FILE_SETTINGS_PLAIN, config->name);
        }
#endif
    }
}

static void rpi_free_settings_plain_file(ConfigFile* config)
{
    if (config)
    {
        if (config->name)
        {
            g_free(config->name);
            config->name = NULL;
        }

        if (config->mode)
        {
            g_free(config->mode);
            config->mode = NULL;
        }

        if (config->content)
        {
            g_free(config->content);
            config->content = NULL;
        }
    }
}
