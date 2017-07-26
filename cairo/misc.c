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

#include "config.h"
#include "private.h"


PyObject*
Pycairo_tuple_getattro (PyObject *self, char **kwds, PyObject *name) {
    PyObject *value, *item;
    int res;
    Py_ssize_t i;

    for (i = 0; kwds[i] != NULL; i++) {
        value = PYCAIRO_PyUnicode_FromString (kwds[i]);
        res = PyObject_RichCompareBool (name, value, Py_EQ);
        Py_DECREF (value);
        if (res == -1) {
            return NULL;
        } else if (res == 1) {
            item = PyTuple_GetItem (self, i);
            if (item == NULL)
                return NULL;
            Py_INCREF (item);
            return item;
        }
    }

    return PyTuple_Type.tp_getattro (self, name);
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
#if PY_MAJOR_VERSION < 3
    if (PyInt_Check (pyobj)) {
        return PyNumber_Long (pyobj);
    } else if (!PyLong_Check (pyobj)) {
        PyErr_SetString (PyExc_TypeError, "not of type int or long");
        return NULL;
    }
    Py_INCREF (pyobj);
    return pyobj;
#else
    if (!PyLong_Check (pyobj)) {
        PyErr_SetString (PyExc_TypeError, "not of type int");
        return NULL;
    }
    Py_INCREF (pyobj);
    return pyobj;
#endif
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
