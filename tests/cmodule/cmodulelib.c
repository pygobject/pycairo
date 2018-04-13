#include <Python.h>
#define PYCAIRO_NO_IMPORT
#include <pycairo.h>
#include "cmodulelib.h"

#if PY_MAJOR_VERSION < 3
extern Pycairo_CAPI_t *Pycairo_CAPI;
#endif

PyObject *
create_image_surface (PyObject *self, PyObject *args)
{
    cairo_surface_t *surface;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 10, 10);

    return PycairoSurface_FromSurface (surface, NULL);
}
