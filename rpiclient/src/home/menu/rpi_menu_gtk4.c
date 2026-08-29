/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * rpi_menu_gtk4.c
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
#include "rpi_menu.h"
#include "../../rpi_config.h"

static const gchar *const FAILED_MALLOC_RPI_MENU =
    "Failed to allocate memory for menu bar.\n";

typedef struct {
  RPIMenu *menu_instance;
  void (*callback)(GtkWidget *, gpointer);
} MenuActionContext;

struct _RPIMenu {
  GtkWidget *menu_bar;
  GMenu *menu_model;
  GSimpleActionGroup *action_group;
  MenuActionContext action_contexts[8];
  guint num_contexts;
};

static void on_action_activated(GSimpleAction *action, GVariant *parameter,
                                gpointer user_data) {
  (void)action;
  (void)parameter;
  MenuActionContext *ctx = (MenuActionContext *)user_data;
  if (ctx && ctx->callback && ctx->menu_instance) {
    ctx->callback(ctx->menu_instance->menu_bar, NULL);
  }
}

RPIMenu *new_rpi_menu(void) {
  RPIMenu *instance = g_malloc0(sizeof(RPIMenu));
  if (!instance) {
    g_critical("%s", FAILED_MALLOC_RPI_MENU);
    return NULL;
  }

  instance->action_group = g_simple_action_group_new();
  instance->menu_model = g_menu_new();

  // File menu
  GMenu *file_menu = g_menu_new();
  g_menu_append(file_menu, "Exit", "win.exit");
  g_menu_append_submenu(instance->menu_model, "File", G_MENU_MODEL(file_menu));
  g_object_unref(file_menu);

  // Option menu
  GMenu *option_menu = g_menu_new();
  g_menu_append(option_menu, "Connect", "win.connect");
  g_menu_append(option_menu, "Disconnect", "win.disconnect");
  g_menu_append_submenu(instance->menu_model, "Option",
                        G_MENU_MODEL(option_menu));
  g_object_unref(option_menu);

  // Settings menu
  GMenu *settings_menu = g_menu_new();
  g_menu_append(settings_menu, "General", "win.general");
  g_menu_append(settings_menu, "Network", "win.network");
  g_menu_append_submenu(instance->menu_model, "Settings",
                        G_MENU_MODEL(settings_menu));
  g_object_unref(settings_menu);

  // Help menu
  GMenu *help_menu = g_menu_new();
  g_menu_append(help_menu, "Help", "win.help");
  g_menu_append(help_menu, "About", "win.about");
  g_menu_append_submenu(instance->menu_model, "Help", G_MENU_MODEL(help_menu));
  g_object_unref(help_menu);

  instance->menu_bar = gtk_popover_menu_bar_new_from_model(
      G_MENU_MODEL(instance->menu_model));
  gtk_widget_insert_action_group(instance->menu_bar, "win",
                                 G_ACTION_GROUP(instance->action_group));

  return instance;
}

void show_rpi_menu(RPIMenu *instance) {
  if (instance && instance->menu_bar) {
    gtk_widget_set_visible(instance->menu_bar, TRUE);
  }
}

void hide_rpi_menu(RPIMenu *instance) {
  if (instance && instance->menu_bar) {
    gtk_widget_set_visible(instance->menu_bar, FALSE);
  }
}

GtkWidget *get_menu_bar_from_rpi_menu(RPIMenu *instance) {
  if (instance) {
    return instance->menu_bar;
  }
  return NULL;
}

GActionGroup *get_action_group_from_rpi_menu(RPIMenu *instance) {
  if (instance) {
    return G_ACTION_GROUP(instance->action_group);
  }
  return NULL;
}

void rpi_menu_connect_signal(RPIMenu *instance,
                             void (*on_callback)(GtkWidget *, gpointer),
                             const gchar *submenu) {
  if (!instance || !on_callback || !submenu) {
    return;
  }

  gchar action_name[32] = {0};
  if (g_strcmp0(submenu, "Exit") == 0) {
    g_strlcpy(action_name, "exit", sizeof(action_name));
  } else if (g_strcmp0(submenu, "Connect") == 0) {
    g_strlcpy(action_name, "connect", sizeof(action_name));
  } else if (g_strcmp0(submenu, "Disconnect") == 0) {
    g_strlcpy(action_name, "disconnect", sizeof(action_name));
  } else if (g_strcmp0(submenu, "General") == 0) {
    g_strlcpy(action_name, "general", sizeof(action_name));
  } else if (g_strcmp0(submenu, "Network") == 0) {
    g_strlcpy(action_name, "network", sizeof(action_name));
  } else if (g_strcmp0(submenu, "Help") == 0) {
    g_strlcpy(action_name, "help", sizeof(action_name));
  } else if (g_strcmp0(submenu, "About") == 0) {
    g_strlcpy(action_name, "about", sizeof(action_name));
  }

  if (action_name[0] != '\0' &&
      instance->num_contexts < (sizeof(instance->action_contexts) /
                                sizeof(instance->action_contexts[0]))) {
    guint idx = instance->num_contexts++;
    instance->action_contexts[idx].menu_instance = instance;
    instance->action_contexts[idx].callback = on_callback;

    GSimpleAction *act = g_simple_action_new(action_name, NULL);
    g_signal_connect(act, "activate", G_CALLBACK(on_action_activated),
                     &instance->action_contexts[idx]);
    g_action_map_add_action(G_ACTION_MAP(instance->action_group),
                            G_ACTION(act));
    g_object_unref(act);
  }
}

void destroy_rpi_menu(RPIMenu *instance) {
  if (instance) {
    if (instance->menu_model) {
      g_object_unref(instance->menu_model);
      instance->menu_model = NULL;
    }
    if (instance->action_group) {
      g_object_unref(instance->action_group);
      instance->action_group = NULL;
    }
    instance->menu_bar = NULL;
    g_free(instance);
  }
}
