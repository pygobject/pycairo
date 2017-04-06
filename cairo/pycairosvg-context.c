/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * PyCairo - Python bindings for Cairo
 *
 * Copyright © 2003-2004 Steve Chaplin
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
 *                 Steve Chaplin
 */

#include <Python.h>
#include "structmember.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "pycairo.h"
#include "pycairo-misc.h"
#include "pycairosvg-private.h"


typedef struct { 
    PyObject_HEAD
    svg_cairo_t *ctx;
} PyCairoSVGContext;


/* if status reports an error, then return 1 (True) and set the exception */
static int
_status_error (svg_cairo_status_t status)
{
    switch (status) {
    case SVG_CAIRO_STATUS_SUCCESS:
	return 0;
    case SVG_CAIRO_STATUS_NO_MEMORY:
	PyErr_NoMemory();
	return 1;
    case SVG_CAIRO_STATUS_IO_ERROR:
	PyErr_SetString(PyExc_IOError, "IO Error");
	return 1;
    case SVG_CAIRO_STATUS_FILE_NOT_FOUND:
	PyErr_SetString(PyExc_RuntimeError, "File not found");
	return 1;
    case SVG_CAIRO_STATUS_INVALID_VALUE:
	PyErr_SetString(PyExc_RuntimeError, "Invalid value");
	return 1;
    case SVG_CAIRO_STATUS_INVALID_CALL:
	PyErr_SetString(PyExc_RuntimeError, "Invalid call");
	return 1;
    case SVG_CAIRO_STATUS_PARSE_ERROR:
	PyErr_SetString(PyExc_RuntimeError, "Parse error");
	return 1;
    default:
	PyErr_SetString(PyExc_RuntimeError, "other cairo.svg error");
	return 1;
    }
}


static void
pycairosvg_dealloc(PyCairoSVGContext* self)                  
{
    if (self->ctx) {
	svg_cairo_destroy(self->ctx);
	self->ctx = NULL;
    }
    self->ob_type->tp_free((PyObject*)self);
}


static PyObject *
pycairosvg_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{               
    PyCairoSVGContext *self = (PyCairoSVGContext *)type->tp_alloc(type, 0);
    if (self && _status_error (svg_cairo_create (&self->ctx))) {
	Py_DECREF(self);
	return NULL;
    }
    return (PyObject *)self;
}

/* static int pycairosvg_init () not needed */


static PyObject *
pycairosvg_parse (PyCairoSVGContext *self, PyObject *args)
{
    char *filename;

    if (!PyArg_ParseTuple(args, "s:Context.parse", &filename))
	return NULL;

    if (_status_error (svg_cairo_parse (self->ctx, filename)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairosvg_parse_buffer (PyCairoSVGContext *self, PyObject *args)
{
    const char *buf;
    size_t count;

    if (!PyArg_ParseTuple(args, "t#:Context.parse_buffer", &buf, &count))
	return NULL;

    //svg_cairo_status_t svg_cairo_parse_buffer (svg_cairo_t *svg_cairo, const char *buf, size_t count);
    if (_status_error (svg_cairo_parse_buffer (self->ctx, buf, count)))
    	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairosvg_render (PyCairoSVGContext *self, PyObject *args)
{
    PyCairoContext *xrs;

    if (!PyArg_ParseTuple(args, "O!:Context.render", 
			  &PyCairoContext_Type, &xrs))
	return NULL;

    if (_status_error (svg_cairo_render (self->ctx, xrs->ctx)))
	return NULL;
    Py_RETURN_NONE;
}


static PyObject *
pycairosvg_get_size (PyCairoSVGContext *self, void *closure)
{
    int width, height;

    svg_cairo_get_size (self->ctx, &width, &height);
    return Py_BuildValue("ii", width, height);
}


static PyGetSetDef pycairosvg_getsets[] = {
    {"size",  (getter)pycairosvg_get_size,  (setter)0,   "size",    NULL},
    {NULL},
};


static PyMethodDef pycairosvg_methods[] = {
    {"parse",        (PyCFunction)pycairosvg_parse,  METH_VARARGS },
    {"parse_buffer", (PyCFunction)pycairosvg_parse_buffer,  METH_VARARGS },
    {"render",       (PyCFunction)pycairosvg_render, METH_VARARGS },
    {NULL},
};


PyTypeObject PyCairoSVGContext_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "cairo.svg.Context",       /*tp_name*/
    sizeof(PyCairoSVGContext), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pycairosvg_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "cairo.svg objects",       /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    pycairosvg_methods,        /* tp_methods */
    0,                         /* tp_members */
    pycairosvg_getsets,        /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)0,               /* tp_init */
    0,                         /* tp_alloc */
    pycairosvg_new,            /* tp_new */
};
