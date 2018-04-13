#include <Python.h>
/* not pycairo3.h because we use the one from the source directory */
#include <pycairo.h>
#include "cmodulelib.h"

static PyMethodDef CModMethods[] = {
    {"create_image_surface", create_image_surface, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

#ifdef __GNUC__
#define PYCAIRO_MODINIT_FUNC __attribute__((visibility("default"))) PyMODINIT_FUNC
#else
#define PYCAIRO_MODINIT_FUNC PyMODINIT_FUNC
#endif

#if PY_MAJOR_VERSION < 3

Pycairo_CAPI_t *Pycairo_CAPI;

PYCAIRO_MODINIT_FUNC
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

PYCAIRO_MODINIT_FUNC
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
