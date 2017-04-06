-*- mode: m4 -*-
AC_PREREQ(2.54)

# the pycairo version number
m4_define(pycairo_major_version, 0)
m4_define(pycairo_minor_version, 6)
m4_define(pycairo_micro_version, 0)
m4_define(pycairo_version, pycairo_major_version.pycairo_minor_version.pycairo_micro_version)

# versions of packages we require
m4_define(cairo_required_version,    0.6.0)
m4_define(gtk_required_version,      2.2.0)
m4_define(pygtk_required_version,    2.2.0)
m4_define(libsvg_cairo_required_version, 0.1.6)

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
            [AC_HELP_STRING([--with-pygtk], [support pygtk [default=yes]])],
	    [],
	    [with_pygtk="yes"])


# put the ACLOCAL flags in the makefile
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AM_PATH_PYTHON(2.3)

AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers)])

# get rid of the -export-dynamic stuff from the configure flags ...
export_dynamic=`(./libtool --config; echo eval echo \\$export_dynamic_flag_spec) | sh`

# cairo
PKG_CHECK_MODULES(CAIRO, cairo >= cairo_required_version)
if test -n "$export_dynamic"; then
  CAIRO_LIBS=`echo $CAIRO_LIBS | sed -e "s/$export_dynamic//"`
fi

# cairo + cairo-xlib + gtk + pygtk
if test x"$with_pygtk" = xyes; then
  # was cairo compiled with cairo-xlib enabled?
  save_LIBS="$LIBS"
  LIBS="$CAIRO_LIBS"
  AC_CHECK_LIB([cairo], [cairo_xlib_surface_create], [], [with_pygtk=no])
  LIBS="$save_LIBS"
fi

if test x"$with_pygtk" = xyes; then
  PKG_CHECK_MODULES(GTK,
    [gtk+-2.0 >= gtk_required_version
    pygtk-2.0 >= pygtk_required_version],
    [], [with_pygtk=no])
  if test -n "$export_dynamic"; then
    GTK_LIBS=`echo $GTK_LIBS | sed -e "s/$export_dynamic//"`
  fi
fi
AM_CONDITIONAL(WITH_PYGTK, test x$with_pygtk = xyes)

# Define HAVE_GTK28 if gtk+ > 2.7 is found
if test x"$with_pygtk" = xyes; then
  AC_MSG_CHECKING([for gtk+ 2.8 API])
  if $PKG_CONFIG --modversion "gtk+-2.0 >= 2.7.0" > /dev/null 2>&1; then
     AC_MSG_RESULT([found])
     AC_DEFINE([HAVE_GTK28], [1], [Define to 1 if gtk+ 2.8 API is found])
  else
     AC_MSG_RESULT([not found])
  fi
fi

# Numeric Python
save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"
AC_CHECK_HEADER([Numeric/arrayobject.h],
  [have_numpy=yes], [have_numpy=no],
  [#include <Python.h>])
CPPFLAGS="$save_CPPFLAGS"
if test x$have_numpy = xyes; then
  AC_DEFINE(HAVE_NUMPY, [1], [Define to 1 if Numeric python is found])
fi
AM_CONDITIONAL(HAVE_NUMPY, test x$have_numpy = xyes)

# is libsvg-cairo installed?
PKG_CHECK_MODULES(LIBSVG_CAIRO,
  libsvg-cairo >= libsvg_cairo_required_version,
  [have_libsvg_cairo=yes],[have_libsvg_cairo=no])
if test -n "$export_dynamic"; then
  LIBSVG_CAIRO_LIBS=`echo $LIBSVG_CAIRO_LIBS | sed -e "s/$export_dynamic//"`
fi
AM_CONDITIONAL(HAVE_LIBSVG_CAIRO, test x$have_libsvg_cairo = xyes)


# add gcc debugging options
changequote(,)dnl
if test "x$GCC" = xyes; then
  case " $CFLAGS " in
  *[\	\ ]-std=c99[\	\ ]*) ;;
  *[\	\ ]-std=c9x[\	\ ]*) ;;
  *) CFLAGS="$CFLAGS -std=c99" ;;
  esac

  case " $CFLAGS " in
  *[\	\ ]-Wall[\	\ ]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac
fi
changequote([,])dnl

case $GCC in
yes)
  # code using Py_True, Py_False will receive 
  # "warning: dereferencing type-punned pointer will break strict-aliasing rules"
  # -fno-strict-aliasing (as used in Python build) switches warnings off
    AC_MSG_CHECKING(whether $CC accepts -fno-strict-aliasing)
     ac_save_cc="$CC"
     CC="$CC -fno-strict-aliasing"
     AC_TRY_RUN([int main() { return 0; }],
     ac_cv_no_strict_aliasing_ok=yes,
     ac_cv_no_strict_aliasing_ok=no,
     ac_cv_no_strict_aliasing_ok=no)
     CC="$ac_save_cc"
    AC_MSG_RESULT($ac_cv_no_strict_aliasing_ok)
    if test $ac_cv_no_strict_aliasing_ok = yes
    then
      CFLAGS="$CFLAGS -fno-strict-aliasing"
    fi
esac

AC_CONFIG_FILES([
Makefile
pycairo.pc
cairo/__init__.py
cairo/Makefile
examples/Makefile
test/Makefile
])

AC_OUTPUT

echo "
Configuration:
Installation prefix             ${prefix}

Additional modules:
	build cairo.gtk?        ${with_pygtk}
	build cairo.svg?        ${have_libsvg_cairo}
	build Numeric support?  ${have_numpy}
"
