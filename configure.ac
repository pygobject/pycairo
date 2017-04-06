-*- mode: m4 -*-
AC_PREREQ(2.59)

# the pycairo version number
m4_define(pycairo_version_major, 1)
m4_define(pycairo_version_minor, 2)
m4_define(pycairo_version_micro, 2)
m4_define(pycairo_version, pycairo_version_major.pycairo_version_minor.pycairo_version_micro)

# versions of packages we require
m4_define(cairo_required_version, 1.2.2)
m4_define(gtk_required_version,   2.2.0)
m4_define(pygtk_required_version, 2.2.0)

AC_INIT([pycairo],
	[pycairo_version],
	[http://bugs.freedesktop.org/enter_bug.cgi?product=pycairo])

AC_SUBST(PYCAIRO_VERSION_MAJOR, [pycairo_version_major])
AC_SUBST(PYCAIRO_VERSION_MINOR, [pycairo_version_minor])
AC_SUBST(PYCAIRO_VERSION_MICRO, [pycairo_version_micro])
AC_SUBST(VERSION_INFO,
	[pycairo_version_major,pycairo_version_minor,pycairo_version_micro])

AC_CONFIG_SRCDIR([cairo/pycairo.h])
AC_CONFIG_HEADERS(config.h)

AM_INIT_AUTOMAKE([1.9 -Wall])

# Options ---
AC_ARG_WITH(pygtk,
            [AC_HELP_STRING([--with-pygtk], [support pygtk [default=yes]])],
	    [],
	    [with_pygtk="yes"])


# put the ACLOCAL flags in the makefile
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

# Checks for programs ---
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AM_PATH_PYTHON(2.3)

# Checks for libraries ---

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
    pygtk-2.0 >= pygtk_required_version
    pygtk-2.0 < 2.7.0],
    [], [with_pygtk=no])
  if test -n "$export_dynamic"; then
    GTK_LIBS=`echo $GTK_LIBS | sed -e "s/$export_dynamic//"`
  fi
fi
AM_CONDITIONAL(WITH_PYGTK, test x$with_pygtk = xyes)

# Checks for header files ---
AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers)])

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

# checks for types and compilers ---
AC_C_CONST

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

# Checks for library functions ---
# Checks for system services ---

# Output ---
AC_CONFIG_FILES([
Makefile
pycairo.pc
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
	build Numeric support?  ${have_numpy}
"
