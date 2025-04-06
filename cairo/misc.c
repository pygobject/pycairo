/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2017 Christoph Reiter
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "private.h"

/* Returns 1 if the object has the correct file type for a filesystem path.
 * Parsing it with Pycairo_fspath_converter() might still fail.
 */
int
Pycairo_is_fspath (PyObject *obj) {
    PyObject *real = PyOS_FSPath (obj);
    if (real == NULL) {
        PyErr_Clear ();
        return 0;
    } else {
        Py_DECREF (real);
        return 1;
    }
}

/* Converts a Python object to a cairo path. The result needs to be freed with
 * PyMem_Free().
 */
int
Pycairo_fspath_converter (PyObject *obj, char** result) {
    char *internal, *buf;
    PyObject *bytes;

#if defined(MS_WINDOWS)
    PyObject *uni;

    if (PyUnicode_FSDecoder (obj, &uni) == 0)
        return 0;

    bytes = PyUnicode_AsEncodedString (uni, "utf-8", "strict");

    Py_DECREF (uni);
    if (bytes == NULL)
        return 0;

    if (PyBytes_AsStringAndSize (bytes, &internal, NULL) == -1) {
        Py_DECREF (bytes);
        return 0;
    }
#else
    if (PyUnicode_FSConverter (obj, &bytes) == 0)
        return 0;

    if (PyBytes_AsStringAndSize (bytes, &internal, NULL) == -1) {
        Py_DECREF (bytes);
        return 0;
    }
#endif

    buf = PyMem_Malloc (strlen (internal) + 1);
    if (buf == NULL) {
        Py_DECREF (bytes);
        PyErr_NoMemory ();
        return 0;
    }
    strcpy (buf, internal);
    Py_DECREF (bytes);
    *result = buf;
    return 1;
}

/* Verifies that the object has a callable write() method.
 * Gives a borrowed reference.
 */
int
Pycairo_writer_converter (PyObject *obj, PyObject** file) {
    PyObject *res;

    /* Check that the file is opened in binary mode by writing a zero length
     * bytes object to it */
    res = PyObject_CallMethod (obj, "write", "(y#)", "", (Py_ssize_t) 0);
    if (res == NULL)
        return  0;
    Py_DECREF (res);

    *file = obj;
    return 1;
}

int
Pycairo_reader_converter (PyObject *obj, PyObject** file) {
    PyObject *res;

    /* Check that the file is opened in binary mode by reading a zero length
     * bytes object from it */
    res = PyObject_CallMethod (obj, "read", "(i)", (int) 0);
    if (res == NULL)
        return 0;

    if (!PyBytes_Check (res)) {
        Py_DECREF (res);
        PyErr_SetString (
            PyExc_TypeError, "'read' does not return bytes");
        return 0;
    }
    Py_DECREF (res);

    *file = obj;
    return 1;
}

int
Pycairo_fspath_none_converter (PyObject *obj, char** result) {
    if (obj == Py_None) {
        *result = NULL;
        return 1;
    }

    return Pycairo_fspath_converter (obj, result);
}

PyObject*
Pycairo_richcompare (void* a, void *b, int op)
{
    PyObject *res;

    switch (op) {
        case Py_EQ:
            res = (a == b) ? Py_True : Py_False;
            break;
      case Py_NE:
            res = (a != b) ? Py_True : Py_False;
            break;
      case Py_LT:
            res = (a < b) ? Py_True : Py_False;
            break;
      case Py_LE:
            res = (a <= b) ? Py_True : Py_False;
            break;
      case Py_GT:
            res = (a > b) ? Py_True : Py_False;
            break;
      case Py_GE:
            res = (a >= b) ? Py_True : Py_False;
            break;
      default:
            res = Py_NotImplemented;
            break;
    }

    Py_INCREF (res);
    return res;
}

/* NULL on error */
static PyObject *
_conv_pyobject_to_pylong (PyObject *pyobj) {
    if (!PyLong_Check (pyobj)) {
        PyErr_SetString (PyExc_TypeError, "not of type int");
        return NULL;
    }
    Py_INCREF (pyobj);
    return pyobj;
}

/* -1 on error */
int
_conv_pyobject_to_ulong (PyObject *pyobj, unsigned long *result) {
    unsigned long temp;
    PyObject *pylong;

    pylong = _conv_pyobject_to_pylong (pyobj);
    if (pylong == NULL)
        return -1;

    temp = PyLong_AsUnsignedLong (pylong);
    if (PyErr_Occurred ())
        return -1;

    *result = temp;
    return 0;
}
