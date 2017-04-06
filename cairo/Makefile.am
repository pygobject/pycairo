
pycairodir = $(pythondir)/cairo
pycairoexecdir = $(pyexecdir)/cairo

pkgincludedir = $(includedir)/pycairo
pkginclude_HEADERS = pycairo.h

nodist_pycairo_PYTHON = __init__.py

pycairoexec_LTLIBRARIES = _cairo.la
_cairo_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_cairo
_cairo_la_LIBADD = $(CAIRO_LIBS)
_cairo_la_CPPFLAGS = $(PYTHON_INCLUDES) $(CAIRO_CFLAGS)
_cairo_la_SOURCES = \
  pycairo.h \
  pycairo-private.h \
  pycairo-context.c \
  pycairo-font.c \
  pycairo-matrix.c \
  pycairo-path.c \
  pycairo-pattern.c \
  pycairo-surface.c \
  cairomodule.c

if HAVE_LIBSVG_CAIRO
pycairoexec_LTLIBRARIES += svg.la
svg_la_LDFLAGS = -module -avoid-version -export-symbols-regex initsvg
svg_la_LIBADD  = $(LIBSVG_CAIRO_LIBS)
svg_la_CPPFLAGS = $(PYTHON_INCLUDES) $(LIBSVG_CAIRO_CFLAGS)
svg_la_SOURCES = \
  pycairosvg.h \
  pycairosvg-private.h \
  cairosvgmodule.c
endif

if WITH_PYGTK
pycairoexec_LTLIBRARIES += gtk.la
gtk_la_LDFLAGS = -module -avoid-version -export-symbols-regex initgtk
gtk_la_LIBADD = $(CAIRO_LIBS) $(GTK_LIBS)
gtk_la_CPPFLAGS = $(PYTHON_INCLUDES) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
gtk_la_SOURCES = \
  cairogtkmodule.c
endif
