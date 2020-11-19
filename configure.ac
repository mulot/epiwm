dnl Process this file with autoconf to produce a configure script.
AC_INIT(epwim, 0.5.6)
AC_CONFIG_SRCDIR(src/button_draw.c)
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET


dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(sys/time.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(select)

dnl Checking for X11
AC_PATH_X
AC_PATH_XTRA

dnl add options to CFLAGS
CFLAGS="-Wall $CFLAGS"


dnl All further test will use X libraries/headers
AC_CHECK_LIB(X11, XInternAtoms, , , [$X_LIBS $X_PRE_LIBS -lX11 $X_EXTRA_LIBS])
AC_CHECK_LIB(Xext, XShapeCombineRectangles, , , [$X_LIBS $X_PRE_LIBS -lXext $X_EXTRA_LIBS])

dnl test for xpm library
HAVE_XPM="no"
AC_ARG_ENABLE(xpm,
        [  --disable-xpm         disable xpm for logoepiwm],
        [ AC_DEFINE(NOXPM, 1, [Define to not use libxpm for logoepiwm])],
        [ HAVE_XPM="yes"
	AC_CHECK_LIB(Xpm, XpmCreatePixmapFromData, , , [$X_LIBS $X_PRE_LIBS -lXpm $X_EXTRA_LIBS])
	]
)

dnl test for imlib library
AC_ARG_ENABLE(imlib,
        [  --disable-imlib         disable use of imlib even if available],,
        [  AC_PATH_PROG(IMLIB_CONFIG, imlib-config, ,${with_imlib_arg-${PATH}})
	if test x"$IMLIB_CONFIG" = x; then
	    AC_MSG_ERROR([imlib-config can not be found])
	fi
	  imlib_libs=`$IMLIB_CONFIG --libs`
	  imlib_cflags=`$IMLIB_CONFIG --cflags`
	  IMLIB_LIBS="$imlib_libs"
	  IMLIB_CFLAGS="$imlib_cflags"
	  AC_DEFINE(IMLIB, 1, [Define to use Imlib for image rendering])]
)
AC_SUBST(IMLIB_LIBS)
AC_SUBST(IMLIB_CFLAGS)

dnl define config files path
dnl AC_ARG_WITH(config_dir, [  --with-config-dir=DIR  configuration files directory [/etc/X11/epiwm] ], CONFIG_PATH=$withval, CONFIG_PATH="/etc/X11/epiwm")
if test "x$sysconfdir" = "x\${prefix}/etc" -o "x$prefix" = "xNONE"; then
	CONFIG_PATH="/etc/X11/epiwm"
else
	CONFIG_PATH="$sysconfdir/epiwm"
fi
AC_DEFINE_UNQUOTED(CONFIG_PATH, "$CONFIG_PATH", [Define configuration files path])
AC_SUBST(CONFIG_PATH)


AC_OUTPUT([Makefile src/Makefile config/Makefile bin/Makefile bin/epiwm.inst modules/Makefile modules/logoepiwm/Makefile], [chmod +x bin/epiwm.inst])
