// Header file providing new C API functions to old Python versions.
//
// File distributed under the Zero Clause BSD (0BSD) license.
// Copyright Contributors to the pythoncapi_compat project.
//
// Homepage:
// https://github.com/python/pythoncapi_compat
//
// Latest version:
// https://raw.githubusercontent.com/python/pythoncapi-compat/main/pythoncapi_compat.h
//
// SPDX-License-Identifier: 0BSD

#ifndef PYTHONCAPI_COMPAT
#define PYTHONCAPI_COMPAT

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>

// gh-106004 added PyDict_GetItemRef() and PyDict_GetItemStringRef()
// to Python 3.13.0a1
#if PY_VERSION_HEX < 0x030D00A1
static inline int
PyDict_GetItemRef(PyObject *mp, PyObject *key, PyObject **result)
{
#if PY_VERSION_HEX >= 0x03000000
    PyObject *item = PyDict_GetItemWithError(mp, key);
#else
    PyObject *item = _PyDict_GetItemWithError(mp, key);
#endif
    if (item != NULL) {
        *result = Py_NewRef(item);
        return 1;  // found
    }
    if (!PyErr_Occurred()) {
        *result = NULL;
        return 0;  // not found
    }
    *result = NULL;
    return -1;
}

static inline int
PyDict_GetItemStringRef(PyObject *mp, const char *key, PyObject **result)
{
    int res;
#if PY_VERSION_HEX >= 0x03000000
    PyObject *key_obj = PyUnicode_FromString(key);
#else
    PyObject *key_obj = PyString_FromString(key);
#endif
    if (key_obj == NULL) {
        *result = NULL;
        return -1;
    }
    res = PyDict_GetItemRef(mp, key_obj, result);
    Py_DECREF(key_obj);
    return res;
}
#endif

// bpo-1635741 added PyModule_AddObjectRef() to Python 3.10.0a3
#if PY_VERSION_HEX < 0x030A00A3
static inline int
PyModule_AddObjectRef(PyObject *module, const char *name, PyObject *value)
{
    int res;

    if (!value && !PyErr_Occurred()) {
        // PyModule_AddObject() raises TypeError in this case
        PyErr_SetString(PyExc_SystemError,
                        "PyModule_AddObjectRef() must be called "
                        "with an exception raised if value is NULL");
        return -1;
    }

    Py_XINCREF(value);
    res = PyModule_AddObject(module, name, value);
    if (res < 0) {
        Py_XDECREF(value);
    }
    return res;
}
#endif

// gh-106307 added PyModule_Add() to Python 3.13.0a1
#if PY_VERSION_HEX < 0x030D00A1
static inline int
PyModule_Add(PyObject *mod, const char *name, PyObject *value)
{
    int res = PyModule_AddObjectRef(mod, name, value);
    Py_XDECREF(value);
    return res;
}
#endif

#ifdef __cplusplus
}
#endif
#endif  // PYTHONCAPI_COMPAT
