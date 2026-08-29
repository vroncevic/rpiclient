/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_format_protocol.c
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
#include "rpi_format_protocol.h"
#include "../../rpi_config.h"

#if RPI_VERBOSE == 1
static const gchar *const PIN_DATA_FORMAT = "Pin operation data: %s.\n";
static const gchar *const CHANNEL_DATA_FORMAT = "Channel operation data: %s.\n";
#endif

static const gchar *const START_REQ_FORMAT = "[";
static const gchar *const SEPARATOR_FORMAT = ":";
static const gchar *const END_REQ_FORMAT = "]";

//////////////////////////////////////////////////////////////////////////////
/// @brief RPI channels complex structure
///   pins - Formatted digital pin request strings
///   channels - Formatted digital channel request strings
///   channel_data - Channel data bytes array
struct _RPIChannels {
  gchar *pins[USED_CHANNELS_FORMAT];
  gchar *channels[USED_CHANNELS_FORMAT];
  gchar channel_data[USED_CHANNELS_FORMAT];
};

static gchar *pin_operation(const gchar *op, guint pin_id);
static gchar *channel_operation(const gchar *op, guint channel_id);

void rpi_format_init(RPIChannels *instance) {
  if (!instance) {
    return;
  }
  for (guint i = 0; i < USED_CHANNELS_FORMAT; i++) {
    instance->pins[i] = pin_operation("DP", i);
    instance->channels[i] = channel_operation("DC", i);
    instance->channel_data[i] = 0;
  }
}

static gchar *pin_operation(const gchar *op, guint pin_id) {
  gchar *pin_id_str = g_strdup_printf("%u", pin_id);
  gchar *pin_op = g_strconcat(START_REQ_FORMAT, op, SEPARATOR_FORMAT,
                              pin_id_str, END_REQ_FORMAT, NULL);
  g_free(pin_id_str);
  pin_id_str = NULL;
#if RPI_VERBOSE == 1
  g_debug(PIN_DATA_FORMAT, pin_op);
#endif
  return pin_op;
}

static gchar *channel_operation(const gchar *op, guint channel_id) {
  gchar *channel_id_str = g_strdup_printf("%u", channel_id);
  gchar *channel_op = g_strconcat(START_REQ_FORMAT, op, SEPARATOR_FORMAT,
                                  channel_id_str, END_REQ_FORMAT, NULL);
  g_free(channel_id_str);
  channel_id_str = NULL;
#if RPI_VERBOSE == 1
  g_debug(CHANNEL_DATA_FORMAT, channel_op);
#endif
  return channel_op;
}

void rpi_format_free(RPIChannels *instance) {
  if (instance) {
    for (guint i = 0; i < USED_CHANNELS_FORMAT; i++) {
      if (instance->pins[i]) {
        g_free(instance->pins[i]);
        instance->pins[i] = NULL;
      }
      if (instance->channels[i]) {
        g_free(instance->channels[i]);
        instance->channels[i] = NULL;
      }
    }
  }
}
