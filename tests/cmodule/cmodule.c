#include <Python.h>
#include <py3cairo.h>
#include "cmodulelib.h"

#ifdef __GNUC__
#define PYCAIRO_MODINIT_FUNC __attribute__((visibility("default"))) PyMODINIT_FUNC
#else
#define PYCAIRO_MODINIT_FUNC PyMODINIT_FUNC
#endif

static PyMethodDef cmod_functions[] = {
    {"create_image_surface", create_image_surface, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}};

static int exec_cmod(PyObject *m)
{
    if (import_cairo() < 0) {
        return -1;
    }
    return 0;
}

static PyModuleDef_Slot cmod_slots[] = {
    {Py_mod_exec, exec_cmod},
    {0, NULL}};

static struct PyModuleDef cmod_module = {
    PyModuleDef_HEAD_INIT,
    "cmod",
    NULL,
    0,
    cmod_functions,
    cmod_slots,
    NULL,
    NULL,
    NULL};

PYCAIRO_MODINIT_FUNC PyInit_cmod(void)
{
    return PyModuleDef_Init(&cmod_module);
}
