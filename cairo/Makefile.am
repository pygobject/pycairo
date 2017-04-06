
AM_CPPFLAGS = $(PYTHON_INCLUDES) $(CAIRO_CFLAGS) $(CAIRO_GTK_CFLAGS)

pycairodir = $(pythondir)/cairo
pycairoexecdir = $(pyexecdir)/cairo

nodist_pycairo_PYTHON = __init__.py

pycairoexec_LTLIBRARIES = _cairo.la

_cairo_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_cairo
_cairo_la_LIBADD = $(CAIRO_LIBS)
_cairo_la_SOURCES = \
  pycairo.h \
  pycairo-context.c \
  pycairo-font.c \
  pycairo-matrix.c \
  pycairo-misc.h \
  pycairo-pattern.c \
  pycairo-private.h \
  pycairo-surface.c \
  pycairosvg-private.h \
  cairomodule.c

if WITH_PYGTK
pycairoexec_LTLIBRARIES += gtk.la
endif
gtk_la_LDFLAGS = -module -avoid-version -export-symbols-regex initgtk
gtk_la_LIBADD = $(CAIRO_LIBS) $(CAIRO_GTK_LIBS)
gtk_la_SOURCES = \
  cairogtkmodule.c

if HAVE_NUMPY
pycairoexec_LTLIBRARIES += numpy.la
endif
numpy_la_LDFLAGS = -module -avoid-version -export-symbols-regex initnumpy
numpy_la_LIBADD = $(CAIRO_LIBS)
numpy_la_SOURCES = \
  caironumpymodule.c

if HAVE_LIBSVG_CAIRO
pycairoexec_LTLIBRARIES += svg.la
svg_la_LDFLAGS = -module -avoid-version -export-symbols-regex initsvg
svg_la_LIBADD  = $(CAIRO_LIBS) $(LIBSVG_CAIRO_LIBS)
svg_la_SOURCES = \
  pycairosvg-context.c \
  cairosvgmodule.c
endif
