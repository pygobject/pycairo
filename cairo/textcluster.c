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

/* 0 on success */
int
_PyTextCluster_AsTextCluster (PyObject *pyobj, cairo_text_cluster_t *cluster) {
    long num_bytes, num_glyphs;

    if (!PyObject_TypeCheck (pyobj, &PycairoTextCluster_Type)) {
        PyErr_SetString (PyExc_TypeError,
            "item must be of type cairo.TextCluster");
        return -1;
    }

    num_bytes = PYCAIRO_PyLong_AsLong (
        PySequence_Fast_GET_ITEM (pyobj, 0));
    if (PyErr_Occurred ())
        return -1;
    if (num_bytes > INT_MAX || num_bytes < INT_MIN) {
        PyErr_SetString (PyExc_ValueError, "num_bytes out of range");
        return -1;
    }
    cluster->num_bytes = (int)num_bytes;

    num_glyphs = PYCAIRO_PyLong_AsLong (
        PySequence_Fast_GET_ITEM (pyobj, 1));
    if (PyErr_Occurred ())
        return -1;
    if (num_glyphs > INT_MAX || num_glyphs < INT_MIN) {
        PyErr_SetString (PyExc_ValueError, "num_glyphs out of range");
        return -1;
    }
    cluster->num_glyphs = (int)num_glyphs;

    return 0;
}

static char *KWDS[] = {"num_bytes", "num_glyphs", NULL};

static PyObject *
text_cluster_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    int num_bytes, num_glyphs;
    PyObject *tuple_args, *result;

    if (!PyArg_ParseTupleAndKeywords (args, kwds, "ii:TextCluster.__new__",
            KWDS, &num_bytes, &num_glyphs))
        return NULL;

    tuple_args = Py_BuildValue ("((ii))", num_bytes, num_glyphs);
    if (tuple_args == NULL)
        return NULL;
    result = PyTuple_Type.tp_new (type, tuple_args, NULL);
    Py_DECREF (tuple_args);
    return result;
}

static PyObject*
text_cluster_repr(PyObject *self) {
    PyObject *format, *result;

    format = PYCAIRO_PyUnicode_FromString (
        "cairo.TextCluster(num_bytes=%r, num_glyphs=%r)");
    if (format == NULL)
        return NULL;
    result = PYCAIRO_PyUnicode_Format (format, self);
    Py_DECREF (format);
    return result;
}

static PyObject *
text_cluster_get_num_bytes(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 0);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
text_cluster_get_num_glyphs(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 1);
    Py_XINCREF (obj);
    return obj;
}

static PyGetSetDef text_cluster_getset[] = {
    {"num_bytes", (getter)text_cluster_get_num_bytes},
    {"num_glyphs", (getter)text_cluster_get_num_glyphs},
    {NULL,},
};

PyTypeObject PycairoTextCluster_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cairo.TextCluster",                /* tp_name */
    sizeof(PycairTextCluster),          /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    (reprfunc)text_cluster_repr,        /* tp_repr */
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
    (newfunc)text_cluster_new,          /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
