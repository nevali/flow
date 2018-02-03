AC_INIT([flow],[master])
AC_CONFIG_SRCDIR([interpreter/attributes.c])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE

LT_INIT

AC_PROG_CC
AC_PROG_CPP

if test x"$GCC" = x"yes" ; then
	AS_ECHO(["enabling GCC-specific warning flags"])
	AS_VAR_APPEND([AM_CPPFLAGS],["-W -Wall -Wno-unused-parameter "])
fi

AC_CONFIG_HEADER([config.h])

AC_DEFINE_UNQUOTED([UNIX_HOST], [1], [Define if building on a UNIX-like system])

AC_CONFIG_SUBDIRS([libedit])
AC_CONFIG_FILES([readline/readline.h:libedit/src/editline/readline.h])
AS_VAR_APPEND([AM_CPPFLAGS],['-I${top_srcdir}/libedit/src '])
AC_DEFINE_UNQUOTED([USE_READLINE], [1], [Define to use readline API in the interpreter])

AC_SUBST([AM_CPPFLAGS])

AC_CONFIG_FILES([
Makefile
interpreter/Makefile
])

AC_OUTPUT
