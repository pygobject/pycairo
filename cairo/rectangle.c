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

static PyObject *
rectangle_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    double x, y, width, height;
    PyObject *tuple_args, *result;

    if (!PyArg_ParseTuple (args, "dddd:Rectangle.__new__",
            &x, &y, &width, &height))
        return NULL;

    tuple_args = Py_BuildValue ("(O)", args);
    if (tuple_args == NULL)
        return NULL;
    result = PyTuple_Type.tp_new (type, tuple_args, kwds);
    Py_DECREF (tuple_args);
    return result;
}

static PyObject*
rectangle_repr(PyObject *self) {
    PyObject *format, *result;

    format = PYCAIRO_PyUnicode_FromString (
        "cairo.Rectangle(x=%f, y=%f, width=%f, height=%f)");
    if (format == NULL)
        return NULL;
    result = PYCAIRO_PyUnicode_Format (format, self);
    Py_DECREF (format);
    return result;
}

static PyObject*
rectangle_getattro (PyObject *self, PyObject *name) {

    PyObject *value;
    int res;

    value = PYCAIRO_PyUnicode_FromString ("x");
    res = PyObject_RichCompareBool (name, value, Py_EQ);
    Py_DECREF (value);
    if (res == -1)
        return NULL;
    else if (res == 1)
        return PyTuple_GetItem (self, 0);

    value = PYCAIRO_PyUnicode_FromString ("y");
    res = PyObject_RichCompareBool (name, value, Py_EQ);
    Py_DECREF (value);
    if (res == -1)
        return NULL;
    else if (res == 1)
        return PyTuple_GetItem (self, 1);

    value = PYCAIRO_PyUnicode_FromString ("width");
    res = PyObject_RichCompareBool (name, value, Py_EQ);
    Py_DECREF (value);
    if (res == -1)
        return NULL;
    else if (res == 1)
        return PyTuple_GetItem (self, 2);

    value = PYCAIRO_PyUnicode_FromString ("height");
    res = PyObject_RichCompareBool (name, value, Py_EQ);
    Py_DECREF (value);
    if (res == -1)
        return NULL;
    else if (res == 1)
        return PyTuple_GetItem (self, 3);

    return PyTuple_Type.tp_getattro (self, name);
}

PyTypeObject PycairoRectangle_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cairo.Rectangle",                  /* tp_name */
    sizeof(PycairRectangle),            /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    (reprfunc)rectangle_repr,           /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    rectangle_getattro,                 /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    0,                                  /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)rectangle_new,             /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
