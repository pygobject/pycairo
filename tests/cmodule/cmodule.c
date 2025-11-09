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
    {"create_context", create_context, METH_NOARGS, NULL},
    {"check_status", check_status, METH_VARARGS, NULL},
    {"test_capi", test_capi, METH_NOARGS, NULL},
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
#if PY_VERSION_HEX >= 0x030D0000
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
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
