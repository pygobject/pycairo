/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * PyCairo - Python bindings for Cairo
 *
 * Copyright © 2003-2004 James Henstridge
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
 *
 * Contributor(s):
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo-private.h"
#include "pycairo-misc.h"

PyObject *
pycairo_font_wrap(cairo_font_t *font)
{
    PyCairoFont *self;

    self = PyObject_New(PyCairoFont, &PyCairoFont_Type);
    if (!self) {
	cairo_font_destroy(font);
	return NULL;
    }

    self->font = font;

    return (PyObject *)self;
}

static void
pycairo_font_dealloc(PyCairoFont *self)
{
    if (self->font)
	cairo_font_destroy(self->font);
    self->font = NULL;

    if (self->ob_type->tp_free)
	self->ob_type->tp_free((PyObject *)self);
    else
	PyObject_Del(self);
}


static PyObject *
pycairo_font_set_transform(PyCairoFont *self, PyObject *args)
{
    PyCairoMatrix *matrix;

    if (!PyArg_ParseTuple(args, "O!:Font.set_transform",
			  &PyCairoMatrix_Type, &matrix))
	return NULL;

    cairo_font_set_transform(self->font, matrix->matrix);
    Py_RETURN_NONE;
}

static PyMethodDef pycairo_font_methods[] = {
    { "set_transform", (PyCFunction)pycairo_font_set_transform, METH_VARARGS },
    { NULL, NULL, 0 }
};

static PyObject *
pycairo_font_current_transform(PyCairoFont *self)
{
    cairo_matrix_t *matrix;

    matrix = cairo_matrix_create();
    if (!matrix)
	return PyErr_NoMemory();
    cairo_font_current_transform(self->font, matrix);
    return pycairo_matrix_wrap(matrix);
}

static PyGetSetDef pycairo_font_getsets[] = {
    { "transform", (getter)pycairo_font_current_transform, (setter)0 },
    { NULL, (getter)0, (setter)0 }
};


PyTypeObject PyCairoFont_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Font",                       /* tp_name */
    sizeof(PyCairoFont),                /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)pycairo_font_dealloc,   /* tp_dealloc */
    (printfunc)0,                       /* tp_print */
    (getattrfunc)0,                     /* tp_getattr */
    (setattrfunc)0,                     /* tp_setattr */
    (cmpfunc)0,                         /* tp_compare */
    (reprfunc)0,                        /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    (hashfunc)0,                        /* tp_hash */
    (ternaryfunc)0,                     /* tp_call */
    (reprfunc)0,                        /* tp_str */
    (getattrofunc)0,                    /* tp_getattro */
    (setattrofunc)0,                    /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    NULL, /* Documentation string */
    (traverseproc)0,                    /* tp_traverse */
    (inquiry)0,                         /* tp_clear */
    (richcmpfunc)0,                     /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    (getiterfunc)0,                     /* tp_iter */
    (iternextfunc)0,                    /* tp_iternext */
    pycairo_font_methods,               /* tp_methods */
    0,                                  /* tp_members */
    pycairo_font_getsets,               /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)0,                        /* tp_init */
    (allocfunc)0,                       /* tp_alloc */
    (newfunc)0,                         /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
