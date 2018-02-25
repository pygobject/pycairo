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

/* read a Python sequence of (i,x,y) sequences
 * return cairo_glyph_t *
 *        num_glyphs
 *        must call PyMem_Free(glyphs) when finished using the glyphs
 */
cairo_glyph_t *
_PycairoGlyphs_AsGlyphs (PyObject *py_object, int *num_glyphs)
{
    Py_ssize_t length, i;
    cairo_glyph_t *glyphs = NULL, *glyph;
    PyObject *py_glyphs, *py_seq = NULL;

    py_glyphs = PySequence_Fast (py_object, "glyphs must be a sequence");
    if (py_glyphs == NULL)
        return NULL;

    length = PySequence_Fast_GET_SIZE (py_glyphs);
    if (length > INT_MAX) {
        Py_DECREF (py_glyphs);
        PyErr_SetString (PyExc_ValueError, "sequence too large");
        return NULL;
    }

    if (*num_glyphs < 0 || *num_glyphs > length)
        *num_glyphs = (int)length;

    glyphs = PyMem_Malloc ((unsigned int)*num_glyphs * sizeof(cairo_glyph_t));
    if (glyphs == NULL) {
        PyErr_NoMemory();
        goto error;
    }
    for (i = 0, glyph = glyphs; i < *num_glyphs; i++, glyph++) {
        long index;
        PyObject *py_item = PySequence_Fast_GET_ITEM (py_glyphs, i);
        py_seq = PySequence_Fast (py_item, "glyph items must be a sequence");
        if (py_seq == NULL)
            goto error;
        if (PySequence_Fast_GET_SIZE (py_seq) != 3) {
            PyErr_SetString (PyExc_ValueError,
                             "each glyph item must be an (i,x,y) sequence");
            goto error;
        }
        index = PYCAIRO_PyLong_AsLong (PySequence_Fast_GET_ITEM (py_seq, 0));
        if (PyErr_Occurred ())
            goto error;
        glyph->index = (unsigned long)index;
        glyph->x = PyFloat_AsDouble (PySequence_Fast_GET_ITEM (py_seq, 1));
        glyph->y = PyFloat_AsDouble (PySequence_Fast_GET_ITEM (py_seq, 2));
        if (PyErr_Occurred())
            goto error;
        Py_DECREF (py_seq);
    }

    Py_DECREF (py_glyphs);
    return glyphs;

error:
    Py_DECREF (py_glyphs);
    Py_XDECREF (py_seq);
    PyMem_Free (glyphs);
    return NULL;
}

/* 0 on success */
int
_PyGlyph_AsGlyph (PyObject *pyobj, cairo_glyph_t *glyph) {
    long index;

    if (!PyObject_TypeCheck (pyobj, &PycairoGlyph_Type)) {
        PyErr_SetString (PyExc_TypeError, "item must be of type cairo.Glyph");
        return -1;
    }

    index = PYCAIRO_PyLong_AsLong (PySequence_Fast_GET_ITEM (pyobj, 0));
    if (PyErr_Occurred ())
        return -1;
    if (index < 0) {
        PyErr_SetString (PyExc_ValueError, "negative index");
        return -1;
    }
    glyph->index = (unsigned long) index;
    glyph->x = PyFloat_AsDouble (PySequence_Fast_GET_ITEM (pyobj, 1));
    glyph->y = PyFloat_AsDouble (PySequence_Fast_GET_ITEM (pyobj, 2));
    return 0;
}

static char *KWDS[] = {"index", "x", "y", NULL};

static PyObject *
glyph_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    double x, y;
    unsigned long index;
    PyObject *pyindex, *tuple_args, *result;

    if (!PyArg_ParseTupleAndKeywords (args, kwds, "Odd:Glyph.__new__",
            KWDS, &pyindex, &x, &y))
        return NULL;

    if (_conv_pyobject_to_ulong (pyindex, &index) < 0)
        return NULL;

    tuple_args = Py_BuildValue ("((kdd))", index, x, y);
    if (tuple_args == NULL)
        return NULL;
    result = PyTuple_Type.tp_new (type, tuple_args, NULL);
    Py_DECREF (tuple_args);
    return result;
}

static PyObject*
glyph_repr(PyObject *self) {
    PyObject *format, *result;

    format = PYCAIRO_PyUnicode_FromString (
        "cairo.Glyph(index=%r, x=%r, y=%r)");
    if (format == NULL)
        return NULL;
    result = PYCAIRO_PyUnicode_Format (format, self);
    Py_DECREF (format);
    return result;
}

static PyObject *
glyph_get_index(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 0);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
glyph_get_x(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 1);
    Py_XINCREF (obj);
    return obj;
}

static PyObject *
glyph_get_y(PyObject *self, void *closure)
{
    PyObject *obj = PyTuple_GetItem (self, 2);
    Py_XINCREF (obj);
    return obj;
}

static PyGetSetDef glyph_getset[] = {
    {"index", (getter)glyph_get_index},
    {"x", (getter)glyph_get_x},
    {"y", (getter)glyph_get_y},
    {NULL,},
};


PyTypeObject PycairoGlyph_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cairo.Glyph",                      /* tp_name */
    sizeof(PycairGlyph),                /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    (reprfunc)glyph_repr,               /* tp_repr */
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
    glyph_getset,                       /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)glyph_new,                 /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
