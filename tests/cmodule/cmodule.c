#include <Python.h>
/* not pycairo3.h because we use the one from the source directory */
#include <pycairo.h>

static Pycairo_CAPI_t *Pycairo_CAPI;

static PyObject *
create_image_surface (PyObject *self, PyObject *args)
{
    cairo_surface_t *surface;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 10, 10);

    return PycairoSurface_FromSurface (surface, NULL);
}

static PyMethodDef CModMethods[] = {
    {"create_image_surface", create_image_surface, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION < 3

PyMODINIT_FUNC
initcmod (void)
{
    Py_InitModule ("cmod", CModMethods);

    Pycairo_IMPORT;
}
#else

static struct PyModuleDef cmod_module = {
    PyModuleDef_HEAD_INIT,
    "cmod",
    NULL,
    -1,
    CModMethods,
};

PyMODINIT_FUNC
PyInit_cmod (void)
{
    PyObject *m;

    if (import_cairo () < 0)
        return NULL;

    m = PyModule_Create (&cmod_module);
    if (m == NULL)
        return NULL;

    return m;
}

#endif
