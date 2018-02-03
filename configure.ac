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

## Allow the package builder to decide whether to build with an external
## readline or libedit, or to use the bundled libedit (if it's present)
##
## You can specify any combination of:
##
## --with[out]-readline
## --with[out]-libedit
## --with[out]-included-libedit
##
## Explicitly specifying --with-readline will cause configuration to fail
## if it's not present; specifying --without-readline will cause the test
## to be skipped.

readline_type="auto"
AC_ARG_WITH([readline],[AS_HELP_STRING([--with[[out]]-readline],[do or do not use the readline library for line editing in the Flow interpreter])])
test x$with_readline = xyes && readline_type="readline"
AC_ARG_WITH([libedit],[AS_HELP_STRING([--with[[out]]-libedit],[do or do not use the libedit library for line editing in the Flow interpreter])])
test x$with_libedit = xyes && readline_type="libedit"
AC_ARG_WITH([included-libedit],[AS_HELP_STRING([--with[[out]]-included-libedit],[do or do not use the bundled libedit library for line editing in the Flow interpreter])])
test x$with_included_libedit = xyes && readline_type="included libedit"

if test "$readline_type" = "readline" || ( test "$readline_type" = auto && test x$with_readline != xno ) ; then
	AC_CHECK_LIB([readline],[rl_done],[
	readline_type="readline"
	AS_VAR_APPEND([LIBS], ["-lreadline "])
	AC_DEFINE_UNQUOTED([USE_READLINE], [1], [Define to use readline API in the interpreter])
	],[
	if test x$with_readline = xyes ; then
		AC_MSG_ERROR([cannot find the readline library])
	fi
	])
fi

if test "$readline_type" = "libedit" || ( test "$readline_type" = auto && test x$with_libedit != xno ) ; then
	AC_CHECK_LIB([edit],[history_init],[
	readline_type="libedit"
	AS_VAR_APPEND([LIBS], ["-ledit "])
	AC_DEFINE_UNQUOTED([USE_LIBEDIT], [1], [Define to use libedit API in the interpreter])
	],[
	if test x$with_libedit= xyes ; then
		AC_MSG_ERROR([cannot find the libedit library])
	fi
	])
fi

if test "$readline_type" = "included libedit" || ( test "$readline_type" = auto && test x$with_included_libedit != xno ) ; then
	readline_type="included libedit"
	if test -d libedit || test -d ${srcdir}/libedit ; then
		AC_CONFIG_SUBDIRS([libedit])
		AC_DEFINE_UNQUOTED([USE_LIBEDIT], [1], [Define to use the libedit API in the interpreter])
		AS_VAR_APPEND([AM_CPPFLAGS],['-I${top_srcdir}/libedit/src '])
		AS_VAR_APPEND([LOCAL_LIBS],['-I${top_builddir}/libedit/src/libedit.la'])
	else
		AC_MSG_ERROR([cannot locate the bundled libedit library (in libedit or $srcdir/libedit)])
	fi
fi

if test "$readline_type" = auto ; then
	readline_type="none"
fi
AC_MSG_CHECKING([type of line-editing library to use])
AC_MSG_RESULT([$readline_type])

AC_ARG_ENABLE([fp],[AS_HELP_STRING([disable-fp],[disable floating point support])],,[enable_fp=auto])

if test x"$enable_fp" != x"no" ; then
	AC_SEARCH_LIBS([cos],[m],[
		enable_fp=yes
	],[
		if test x"$enable_fp" = x"auto" ; then
			AC_MSG_WARN([cannot find mathematics library, disabling floating-point])
			enable_fp=no
		elif test x"$enable_fp" = x"yes" ; then
			AC_MSG_ERROR([cannot find mathematics library to enable floating point])
		fi
	])
fi
if test x"$enable_fp" = x"no" ; then
	AC_DEFINE_UNQUOTED([NO_FP],[1],[Define to disable floating-point])
fi
	
AC_MSG_CHECKING([whether to enable tests])
if test x"$cross_compiling" = x"yes" ; then
	enable_tests=no
else
	enable_tests=yes
fi
AC_MSG_RESULT([$enable_tests])
AM_CONDITIONAL([ENABLE_TESTS], [test x$enable_tests = xyes])

AC_SUBST([LIBS])
AC_SUBST([LOCAL_LIBS])
AC_SUBST([AM_CPPFLAGS])

AC_CONFIG_FILES([
Makefile
interpreter/Makefile
t/Makefile
])

AC_OUTPUT
