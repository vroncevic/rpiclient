/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_format_crypto.c
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
#include "rpi_format_crypto.h"
#include "../../rpi_config.h"
#include <string.h>

#if RPI_VERBOSE == 1
static const gchar *const IN_DATA_FORMAT = "Input data for operation: %s.\n";
static const gchar *const SHIFT_DATA_FORMAT = "Shift value for operation: %u.\n";
static const gchar *const ENCRYPTED_DATA_FORMAT = "Encrypted data: %s.\n";
static const gchar *const DECRYPTED_DATA_FORMAT = "Decrypted data: %s.\n";
#endif

static const gchar *const FAILED_FORMAT = "Unable to process data format.\n";

static const gint NUMBER_OF_ASCII_LETTERS = 26;
static const gint NUMBER_OF_ASCII_NUMBERS = 10;
static const gint UPPERCASE_ASCII_A = 65;
static const gint LOWERCASE_ASCII_A = 97;
static const gint ASCII_ZERO = 48;

gchar *rpi_format_encrypt(const gchar *in, guint shift) {
  if (!in) {
    g_critical("%s", FAILED_FORMAT);
    return NULL;
  }

#if RPI_VERBOSE == 1
  g_debug(IN_DATA_FORMAT, in);
  g_debug(SHIFT_DATA_FORMAT, shift);
#endif

  gsize len = strlen(in);
  gchar *out = g_malloc0(len + 1);
  if (!out) {
    return NULL;
  }

  for (gsize i = 0; i < len; i++) {
    gchar c = in[i];
    if (g_ascii_isupper(c)) {
      out[i] = (gchar)((((c - UPPERCASE_ASCII_A) + shift) %
                        NUMBER_OF_ASCII_LETTERS) +
                       UPPERCASE_ASCII_A);
    } else if (g_ascii_islower(c)) {
      out[i] = (gchar)((((c - LOWERCASE_ASCII_A) + shift) %
                        NUMBER_OF_ASCII_LETTERS) +
                       LOWERCASE_ASCII_A);
    } else if (g_ascii_isdigit(c)) {
      out[i] = (gchar)((((c - ASCII_ZERO) + shift) %
                        NUMBER_OF_ASCII_NUMBERS) +
                       ASCII_ZERO);
    } else {
      out[i] = c;
    }
  }

#if RPI_VERBOSE == 1
  g_debug(ENCRYPTED_DATA_FORMAT, out);
#endif

  return out;
}

gchar *rpi_format_decrypt(const gchar *in, guint shift) {
  if (!in) {
    g_critical("%s", FAILED_FORMAT);
    return NULL;
  }

#if RPI_VERBOSE == 1
  g_debug(IN_DATA_FORMAT, in);
  g_debug(SHIFT_DATA_FORMAT, shift);
#endif

  gsize len = strlen(in);
  gchar *out = g_malloc0(len + 1);
  if (!out) {
    return NULL;
  }

  for (gsize i = 0; i < len; i++) {
    gchar c = in[i];
    if (g_ascii_isupper(c)) {
      out[i] = (gchar)((((c - UPPERCASE_ASCII_A) + NUMBER_OF_ASCII_LETTERS -
                         (shift % NUMBER_OF_ASCII_LETTERS)) %
                        NUMBER_OF_ASCII_LETTERS) +
                       UPPERCASE_ASCII_A);
    } else if (g_ascii_islower(c)) {
      out[i] = (gchar)((((c - LOWERCASE_ASCII_A) + NUMBER_OF_ASCII_LETTERS -
                         (shift % NUMBER_OF_ASCII_LETTERS)) %
                        NUMBER_OF_ASCII_LETTERS) +
                       LOWERCASE_ASCII_A);
    } else if (g_ascii_isdigit(c)) {
      out[i] = (gchar)((((c - ASCII_ZERO) + NUMBER_OF_ASCII_NUMBERS -
                         (shift % NUMBER_OF_ASCII_NUMBERS)) %
                        NUMBER_OF_ASCII_NUMBERS) +
                       ASCII_ZERO);
    } else {
      out[i] = c;
    }
  }

#if RPI_VERBOSE == 1
  g_debug(DECRYPTED_DATA_FORMAT, out);
#endif

  return out;
}
