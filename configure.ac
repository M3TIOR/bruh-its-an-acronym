AC_PREREQ([2.71])
AC_INIT([bruh], [0.0.0], [https://github.com/m3tior/bruh-its-an-acryonym])
AC_COPYRIGHT([This source code is licensed under the GPL-3.0 only])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])

ARCH="$(uname -m)";
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/config.h:config.h.in])

AC_CONFIG_COMMANDS([xxd embedding], [xxd -i src/main.c > src/bruh.xxd.c])

# Checks for programs.
AC_PROG_CC
#gl_EARLY
#gl_INIT

# Checks for header files.
#AC_CHECK_HEADERS([
#	systemd/sd-bus.h
#])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
#AC_CHECK_FUNCS([strerror])

# NOTE: The macro language this uses is EXTREMELY tempermental. Don't
#   add files you're not sure you absolutely need to lists.
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	deps/Makefile
])

AC_OUTPUT
