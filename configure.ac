-*- mode: m4 -*-
AC_PREREQ(2.59)

# the pycairo version number
m4_define(pycairo_version_major, 1)
m4_define(pycairo_version_minor, 10)
m4_define(pycairo_version_micro, 0)
m4_define(pycairo_version, pycairo_version_major.pycairo_version_minor.pycairo_version_micro)

# versions of packages we require
m4_define(cairo_required_version, 1.10.0)
m4_define(xpyb_required_version, 1.3)

AC_INIT([py2cairo],
	[pycairo_version],
	[http://bugs.freedesktop.org/enter_bug.cgi?product=pycairo])

AC_DEFINE(PYCAIRO_VERSION_MAJOR, pycairo_version_major, [pycairo major version])
AC_DEFINE(PYCAIRO_VERSION_MINOR, pycairo_version_minor, [pycairo minor version])
AC_DEFINE(PYCAIRO_VERSION_MICRO, pycairo_version_micro, [pycairo macro version])

AC_CONFIG_SRCDIR([src/pycairo.h])
AC_CONFIG_HEADERS(src/config.h)

AM_INIT_AUTOMAKE([1.9.6 -Wall])

# Options ---

# put the ACLOCAL flags in the makefile
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

# Checks for programs ---
AC_DISABLE_STATIC
AC_PROG_LIBTOOL dnl required version (1.4) DON'T REMOVE - used by autogen.sh
dnl when using libtool 2.x create libtool early, because it's used in configure
m4_ifdef([LT_OUTPUT], [LT_OUTPUT])

AM_PATH_PYTHON(2.6)

# Checks for libraries ---

# get rid of the -export-dynamic stuff from the configure flags ...
export_dynamic=`(./libtool --config; echo eval echo \\$export_dynamic_flag_spec) | sh`

# cairo
PKG_CHECK_MODULES(CAIRO, cairo >= cairo_required_version)
if test -n "$export_dynamic"; then
  CAIRO_LIBS=`echo $CAIRO_LIBS | sed -e "s/$export_dynamic//"`
fi

# xpyb
PKG_CHECK_MODULES(XPYB, xpyb >= xpyb_required_version,[have_xpyb=yes],[have_xpyb=no])
AS_IF([test "x$have_xpyb" = "xyes"],
      [AC_DEFINE([HAVE_XPYB], [1], [Define to 1 if xpyb is available])])

# Checks for header files ---
AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers)])

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
pycairo-uninstalled.pc
src/Makefile
doc/Makefile
examples/Makefile
test/Makefile
])

AC_OUTPUT

echo "
Configuration:
Installation prefix             ${prefix}
"
