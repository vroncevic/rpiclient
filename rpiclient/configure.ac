dnl ***************************************************************************
dnl @info   gengtkpro ver.1.0
dnl @author Vladimir Roncevic <elektron.ronca@gmail.com>
dnl @brief  Process this file with autoconf to produce a configure script.
dnl ***************************************************************************
AC_INIT([rpiclient], [1.0.0])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])
AC_PROG_CC
IT_PROG_INTLTOOL([0.51.0])

GETTEXT_PACKAGE=rpiclient
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
GLIB_GNU_GETTEXT
AM_PROG_LIBTOOL
LT_INIT

AC_ARG_WITH([gtk],
  [AS_HELP_STRING([--with-gtk=3.0|4.0], [which GTK+ version to compile against (default: 3.0)])],
  [case "$with_gtk" in
     3.0|4.0) ;;
     *) AC_MSG_ERROR([invalid GTK version specified: $with_gtk]) ;;
   esac],
  [with_gtk=3.0])

AS_IF([test "x$with_gtk" = "x3.0"], [
  PKG_CHECK_MODULES([GTK], [gtk+-3.0])
], [
  PKG_CHECK_MODULES([GTK], [gtk4])
])

AM_CONDITIONAL([USE_GTK3], [test "x$with_gtk" = "x3.0"])
AM_CONDITIONAL([USE_GTK4], [test "x$with_gtk" = "x4.0"])

AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

PKG_CHECK_MODULES([SQLITE3], [sqlite3])
AC_SUBST([SQLITE3_CFLAGS])
AC_SUBST([SQLITE3_LIBS])

AC_CONFIG_COMMANDS_PRE([
  if test -f src/settings/rpi_settings.h; then
    sed -i 's/#define RPI_USE_SETTINGS_SQLITE3 0/#define RPI_USE_SETTINGS_SQLITE3 1/' src/settings/rpi_settings.h
  else
    AC_MSG_ERROR([src/settings/rpi_settings.h does not exist. Ensure it is available.])
  fi
])

GIT_REVISION=$(git -C .. rev-parse --short HEAD 2>/dev/null || echo "unknown")
RPI_CLIENT_VERSION_WITH_REV="${PACKAGE_VERSION}-${GIT_REVISION}"
AC_MSG_NOTICE([RPI_CLIENT_VERSION_WITH_REV is $RPI_CLIENT_VERSION_WITH_REV])

AC_CONFIG_COMMANDS_PRE([
  if test -f src/rpi_config.h; then
    sed -i "s|^#define RPI_CLIENT_VERSION \".*\"|#define RPI_CLIENT_VERSION \"${RPI_CLIENT_VERSION_WITH_REV}\"|" src/rpi_config.h
  else
    AC_MSG_ERROR([src/rpi_config.h does not exist. Ensure it is available.])
  fi
])

AC_OUTPUT([
    Makefile
    po/Makefile.in
    src/Makefile
    rc/Makefile
])
