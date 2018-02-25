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

static char *KWDS[] = {"x_bearing", "y_bearing", "width", "height",
                       "x_advance", "y_advance", NULL};

static PyObject *
text_extents_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    cairo_text_extents_t e;
    PyObject *tuple_args, *result;

    if (!PyArg_ParseTupleAndKeywords (args, kwds, "dddddd:TextExtents.__new__",
            KWDS, &e.x_bearing, &e.y_bearing, &e.width, &e.height,
            &e.x_advance, &e.y_advance))
        return NULL;

    tuple_args = Py_BuildValue ("((dddddd))", e.x_bearing, e.y_bearing,
                                e.width, e.height, e.x_advance, e.y_advance);
    if (tuple_args == NULL)
        return NULL;
    result = PyTuple_Type.tp_new (type, tuple_args, NULL);
    Py_DECREF (tuple_args);
    return result;
}

static PyObject*
text_extents_repr(PyObject *self) {
    PyObject *format, *result;

    format = PYCAIRO_PyUnicode_FromString (
        "cairo.TextExtents(x_bearing=%r, y_bearing=%r, width=%r, height=%r, "
        "x_advance=%r, y_advance=%r)");
    if (format == NULL)
        return NULL;
    result = PYCAIRO_PyUnicode_Format (format, self);
    Py_DECREF (format);
    return result;
}

static PyObject *
text_extents_get_x_bearing(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 0);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
text_extents_get_y_bearing(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 1);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
text_extents_get_width(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 2);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
text_extents_get_height(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 3);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
text_extents_get_x_advance(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 4);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
text_extents_get_y_advance(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 5);
    Py_XINCREF (obj);
    return obj;
}

static PyGetSetDef text_cluster_getset[] = {
    {"x_bearing", (getter)text_extents_get_x_bearing},
    {"y_bearing", (getter)text_extents_get_y_bearing},
    {"width", (getter)text_extents_get_width},
    {"height", (getter)text_extents_get_height},
    {"x_advance", (getter)text_extents_get_x_advance},
    {"y_advance", (getter)text_extents_get_y_advance},
    {NULL,},
};

PyTypeObject PycairoTextExtents_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cairo.TextExtents",                /* tp_name */
    sizeof(PycairoTextExtents),         /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    (reprfunc)text_extents_repr,        /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
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
    text_cluster_getset,                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)text_extents_new,          /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
