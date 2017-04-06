-*- mode: m4 -*-
AC_PREREQ(2.54)

# the pycairo version number
m4_define(pycairo_major_version, 0)
m4_define(pycairo_minor_version, 4)
m4_define(pycairo_micro_version, 0)
m4_define(pycairo_version, pycairo_major_version.pycairo_minor_version.pycairo_micro_version)

# versions of packages we require
m4_define(cairo_required_version,    0.4.0)
m4_define(gtk_required_version,      2.0.0)
m4_define(pygtk_required_version,    1.99.16)
m4_define(libsvg_cairo_required_version, 0.1.5)

AC_INIT(pycairo, pycairo_version)

AC_DEFINE(PYCAIRO_MAJOR_VERSION, pycairo_major_version,[Pycairo major version])
AC_DEFINE(PYCAIRO_MINOR_VERSION, pycairo_minor_version,[Pycairo minor version])
AC_DEFINE(PYCAIRO_MICRO_VERSION, pycairo_micro_version,[Pycairo macro version])
AC_SUBST(VERSION_INFO, [pycairo_major_version,pycairo_minor_version,pycairo_micro_version])

AC_CONFIG_SRCDIR([cairo/pycairo.h])
AC_CONFIG_HEADERS(config.h)

AM_INIT_AUTOMAKE([1.7 -Wall])

# Options
AC_ARG_WITH(pygtk,
            [AC_HELP_STRING([--with-pygtk],
                            [support pygtk [default=yes]])],,
	    [with_pygtk="yes"])


# put the ACLOCAL flags in the makefile
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AM_PATH_PYTHON(2.2)

AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers)])

# get rid of the -export-dynamic stuff from the configure flags ...
export_dynamic=`(./libtool --config; echo eval echo \\$export_dynamic_flag_spec) | sh`

# cairo
PKG_CHECK_MODULES(CAIRO, cairo >= cairo_required_version)
if test -n "$export_dynamic"; then
  CAIRO_LIBS=`echo $CAIRO_LIBS | sed -e "s/$export_dynamic//"`
fi

# cairo + gtk + pygtk
if test x"$with_pygtk" = xno; then
  with_pygtk=false
else
  PKG_CHECK_MODULES(CAIRO_GTK,
    [gtk+-2.0 >= gtk_required_version dnl
    pygtk-2.0 >= pygtk_required_version],
    [with_pygtk=true], [with_pygtk=false])
  if test -n "$export_dynamic"; then
    CAIRO_GTK_LIBS=`echo $CAIRO_GTK_LIBS | sed -e "s/$export_dynamic//"`
  fi
fi
AM_CONDITIONAL(WITH_PYGTK, $with_pygtk)

# Numeric Python
save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"
AC_CHECK_HEADER([Numeric/arrayobject.h],
  [have_numpy=true], [have_numpy=false],
  [#include <Python.h>])
CPPFLAGS="$save_CPPFLAGS"
if $have_numpy; then
  AC_DEFINE(HAVE_NUMPY,,[whether to include numeric python support])
fi
AM_CONDITIONAL(HAVE_NUMPY, $have_numpy)

# is libsvg-cairo installed?
PKG_CHECK_MODULES(LIBSVG_CAIRO, dnl
  libsvg-cairo >= libsvg_cairo_required_version,
  [have_libsvg_cairo=true],[have_libsvg_cairo=false])
if test -n "$export_dynamic"; then
  LIBSVG_CAIRO_LIBS=`echo $LIBSVG_CAIRO_LIBS | sed -e "s/$export_dynamic//"`
fi
AM_CONDITIONAL(HAVE_LIBSVG_CAIRO, $have_libsvg_cairo)


# add debugging options ...
changequote(,)dnl
if test "x$GCC" = xyes; then
  case " $CFLAGS " in
  *[\	\ ]-Wall[\	\ ]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac

  case " $CFLAGS " in
  *[\	\ ]-std=c9x[\	\ ]*) ;;
  *) CFLAGS="$CFLAGS -std=c9x" ;;
  esac
fi
changequote([,])dnl

AC_CONFIG_FILES(
  Makefile
  cairo/__init__.py
  cairo/Makefile
  examples/Makefile)
AC_OUTPUT

echo "
Configuration:
Installation prefix             ${prefix}

Additional modules:
	build cairo.gtk   ?	${with_pygtk}
	build cairo.numpy ?	${have_numpy}
	build cairo.svg   ?     ${have_libsvg_cairo}
"
