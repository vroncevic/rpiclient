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
	-pedantic \
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

# rpiclient source modules
rpiclient_SOURCES = \
	main.c \
	home/rpi_home.c \
	home/rpi_home_frame.c \
	home/rpi_menu.c \
	home/rpi_channel_status.c \
	home/rpi_channel_control.c \
	network/rpi_network.c \
	about/rpi_about_dialog.c \
	format/rpi_format.c \
	exit/rpi_exit_dialog.c \
	help/rpi_help_window.c \
	help/rpi_image_slider.c \
	info/rpi_info_dialog.c \
	misc/rpi_misc.c \
	resource/rpi_cpixbuf.c \
	resource/rpi_get_resource_dir.c \
	resource/rpi_get_resource_file_path.c \
	resource/rpi_get_config_dir.c \
	resource/rpi_get_config_file_path.c \
	settings/rpi_settings_network_window.c \
	settings/rpi_settings_general_window.c \
	settings/rpi_settings_config.c \
	settings/rpi_settings_user.c \
	settings/rpi_settings_plain.c \
	settings/rpi_settings_sqlite.c \
	warning/rpi_warning_dialog.c \
	widgets/rpi_vertical_bar.c \
	resource/rpi_resources.c

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

