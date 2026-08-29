##
## @info   gengtkpro ver.1.0
## @author Vladimir Roncevic <elektron.ronca@gmail.com>
## @brief  Process this file with automake to produce Makefile.in
##

# Sub dirs with source modules
AM_SUBDIRS = home about format exit help info resource settings warning widgets network

# Automake dirs
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

# GCC flags
AM_CFLAGS = \
	-Wall \
	-g \
	-Wno-pedantic \
	-Wall \
	-Wextra \
	-Wcast-align \
	-Wno-cast-qual \
	-Wdisabled-optimization \
	-Wformat=2 \
	-Winit-self \
	-Wlogical-op \
	-Wmissing-declarations \
	-Wmissing-include-dirs \
	-Wshadow \
	-Wstrict-overflow=5 \
	-Wundef \
	-Werror \
	-Wno-unused \
	-mtune=native \
	-std=c11 \
	$(GTK_CFLAGS) \
	$(SQLITE3_CFLAGS)

# Linker flags for building executable (symbols are accessible for dynamic linking)
rpiclient_LDFLAGS = \
	-Wl, \
	-export-dynamic

# Additional libraries to link
rpiclient_LDADD = \
	$(GTK_LIBS) \
	$(SQLITE3_LIBS)

# Executable name
bin_PROGRAMS = rpiclient

if USE_GTK3
GTK_VERSION_SOURCES = \
	main_gtk3.c \
	about/rpi_about_dialog_gtk3.c \
	home/rpi_home_gtk3.c \
	home/frame/rpi_home_frame_gtk3.c \
	home/frame/control/rpi_home_frame_control_gtk3.c \
	home/frame/status/rpi_home_frame_status_gtk3.c \
	home/menu/rpi_menu_gtk3.c \
	home/menu/file/rpi_menu_file_gtk3.c \
	home/menu/option/rpi_menu_option_gtk3.c \
	home/menu/settings/rpi_menu_settings_gtk3.c \
	home/menu/help/rpi_menu_help_gtk3.c \
	home/channels/rpi_channel_status_gtk3.c \
	home/channels/rpi_channel_control_gtk3.c \
	help/rpi_image_slider_gtk3.c \
	help/rpi_help_window_gtk3.c \
	exit/rpi_exit_dialog_gtk3.c \
	info/rpi_info_dialog_gtk3.c \
	warning/rpi_warning_dialog_gtk3.c \
	settings/ui/rpi_settings_general_window_gtk3.c \
	settings/ui/rpi_settings_network_window_gtk3.c \
	widgets/rpi_vertical_bar_gtk3.c
else
GTK_VERSION_SOURCES = \
	main_gtk4.c \
	about/rpi_about_dialog_gtk4.c \
	home/rpi_home_gtk4.c \
	home/frame/rpi_home_frame_gtk4.c \
	home/frame/control/rpi_home_frame_control_gtk4.c \
	home/frame/status/rpi_home_frame_status_gtk4.c \
	home/menu/rpi_menu_gtk4.c \
	home/channels/rpi_channel_status_gtk4.c \
	home/channels/rpi_channel_control_gtk4.c \
	help/rpi_image_slider_gtk4.c \
	help/rpi_help_window_gtk4.c \
	exit/rpi_exit_dialog_gtk4.c \
	info/rpi_info_dialog_gtk4.c \
	warning/rpi_warning_dialog_gtk4.c \
	settings/ui/rpi_settings_general_window_gtk4.c \
	settings/ui/rpi_settings_network_window_gtk4.c \
	widgets/rpi_vertical_bar_gtk4.c
endif

# rpiclient source modules
rpiclient_SOURCES = \
	network/rpi_network.c \
	network/client/rpi_network_client.c \
	network/sender/rpi_network_sender.c \
	network/receiver/rpi_network_receiver.c \
	format/crypto/rpi_format_crypto.c \
	format/protocol/rpi_format_protocol.c \
	resource/pixbuf/rpi_cpixbuf.c \
	resource/path/rpi_get_resource_dir.c \
	resource/path/rpi_get_resource_file_path.c \
	resource/path/rpi_get_config_dir.c \
	resource/path/rpi_get_config_file_path.c \
	settings/rpi_settings_model.c \
	settings/rpi_settings_config.c \
	settings/storage/rpi_settings_user.c \
	settings/storage/plain/rpi_settings_plain.c \
	settings/storage/plain/rpi_settings_plain_read.c \
	settings/storage/plain/rpi_settings_plain_write.c \
	settings/storage/sqlite/rpi_settings_sqlite.c \
	settings/storage/sqlite/rpi_settings_sqlite_db.c \
	settings/storage/sqlite/rpi_settings_sqlite_read.c \
	settings/storage/sqlite/rpi_settings_sqlite_write.c \
	resource/rpi_resources.c \
	$(GTK_VERSION_SOURCES)

BUILT_SOURCES = resource/rpi_resources.c
CLEANFILES = resource/rpi_resources.c

resource/rpi_resources.c: $(srcdir)/resource/rpi_resources.gresource.xml $(srcdir)/resource/style.css
	glib-compile-resources --target=$@ --sourcedir=$(srcdir)/resource --generate-source $<

# Specify installation directory for the binary
install-exec-hook:
	@echo "Installing binary to /usr/local/bin"
	@cp rpiclient /usr/local/bin/

# Unistall rules
uninstall-local:
	-rm -r $(pkgdatadir)
