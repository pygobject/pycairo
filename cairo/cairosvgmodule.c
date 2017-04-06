/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2003-2005 Steve Chaplin
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

#include <Python.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "pycairosvg-private.h"


/* A module specific exception */
static PyObject *CairoSVGError = NULL;

static Pycairo_CAPI_t *Pycairo_CAPI;


#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initsvg (void)
{
    PyObject* m;

    if (PyType_Ready(&PycairoSVGContext_Type) < 0)
        return;

    m = Py_InitModule ("cairo.svg", NULL);
    if (m == NULL)
	return;

    Pycairo_IMPORT;

    Py_INCREF(&PycairoSVGContext_Type);
    PyModule_AddObject(m, "Context", (PyObject *)&PycairoSVGContext_Type);

    /* Add 'cairo.svg.Error' to the module */
    if (CairoSVGError == NULL) {
	CairoSVGError = PyErr_NewException("cairo.svg.Error", NULL, NULL);
	if (CairoSVGError == NULL)
	    return;
    }
    Py_INCREF(CairoSVGError);
    if (PyModule_AddObject(m, "Error", CairoSVGError) < 0)
	return;
}


/* if status reports an error, then return 1 (True) and set the exception */
int
Pycairosvg_Check_Status (svg_cairo_status_t status)
{
    switch (status) {
    case SVG_CAIRO_STATUS_SUCCESS:
	return 0;
    case SVG_CAIRO_STATUS_NO_MEMORY:
	PyErr_NoMemory();
	break;
    case SVG_CAIRO_STATUS_IO_ERROR:
	PyErr_SetString(CairoSVGError, "IO Error");
	break;
    case SVG_CAIRO_STATUS_FILE_NOT_FOUND:
	PyErr_SetString(CairoSVGError, "File not found");
	break;
    case SVG_CAIRO_STATUS_INVALID_VALUE:
	PyErr_SetString(CairoSVGError, "Invalid value");
	break;
    case SVG_CAIRO_STATUS_INVALID_CALL:
	PyErr_SetString(CairoSVGError, "Invalid call");
	break;
    case SVG_CAIRO_STATUS_PARSE_ERROR:
	PyErr_SetString(CairoSVGError, "Parse error");
	break;
    default:
	PyErr_SetString(CairoSVGError, "other cairo.svg error");
    }
    return 1;
}


/* class cairo.svg.Context ------------------------------------------------ */

static void
pycairosvg_dealloc (PycairoSVGContext *o)                  
{
    if (o->ctx) {
	svg_cairo_destroy (o->ctx);
	o->ctx = NULL;
    }
    o->ob_type->tp_free((PyObject*)o);
}


static PyObject *
pycairosvg_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{               
    PyObject *o = type->tp_alloc(type, 0);

    if (o) {
	PycairoSVGContext *svg = ((PycairoSVGContext *)o);
	svg_cairo_status_t status = svg_cairo_create (&svg->ctx);
	if (Pycairosvg_Check_Status (status)) {
	    Py_DECREF(o);
	    return NULL;
	}
    }
    return o;
}

static PyObject *
pycairosvg_parse (PycairoSVGContext *o, PyObject *args)
{
    const char *filename;

    if (!PyArg_ParseTuple(args, "s:Context.parse", &filename))
	return NULL;

    if (Pycairosvg_Check_Status (svg_cairo_parse (o->ctx, filename)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairosvg_parse_buffer (PycairoSVGContext *o, PyObject *args)
{
    const char *buf;
    size_t count;

    if (!PyArg_ParseTuple(args, "t#:Context.parse_buffer", &buf, &count))
	return NULL;

    /*svg_cairo_status_t svg_cairo_parse_buffer (svg_cairo_t *svg_cairo, 
      const char *buf, size_t count);*/
    if (Pycairosvg_Check_Status (svg_cairo_parse_buffer (o->ctx, buf, 
							 count)))
    	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairosvg_render (PycairoSVGContext *o, PyObject *args)
{
    PycairoContext *xrs;

    if (!PyArg_ParseTuple(args, "O!:Context.render", 
			  &PycairoContext_Type, &xrs))
	return NULL;

    if (Pycairosvg_Check_Status (svg_cairo_render (o->ctx, xrs->ctx)))
	return NULL;
    Py_RETURN_NONE;
}


static PyObject *
pycairosvg_get_size (PycairoSVGContext *o, void *closure)
{
    int width, height;
    svg_cairo_get_size (o->ctx, &width, &height);
    return Py_BuildValue("ii", width, height);
}


static PyMethodDef pycairosvg_methods[] = {
    {"parse",        (PyCFunction)pycairosvg_parse,         METH_VARARGS },
    {"parse_buffer", (PyCFunction)pycairosvg_parse_buffer,  METH_VARARGS },
    /* parse_file */
    /* parse_file_chunk_begin */
    /* parse_file_chunk */
    /* parse_file_chunk_end */
    {"render",       (PyCFunction)pycairosvg_render,        METH_VARARGS },
    /* set_viewport_dimension */
    {"get_size",     (PyCFunction)pycairosvg_get_size,      METH_NOARGS},
    {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoSVGContext_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                  /*ob_size*/
    "cairo.svg.Context",                /*tp_name*/
    sizeof(PycairoSVGContext),          /*tp_basicsize*/
    0,                                  /*tp_itemsize*/
    (destructor)pycairosvg_dealloc,     /*tp_dealloc*/
    0,                         		/*tp_print*/
    0,                         		/*tp_getattr*/
    0,                         		/*tp_setattr*/
    0,                         		/*tp_compare*/
    0,                         		/*tp_repr*/
    0,                         		/*tp_as_number*/
    0,                         		/*tp_as_sequence*/
    0,                         		/*tp_as_mapping*/
    0,                         		/*tp_hash */
    0,                         		/*tp_call*/
    0,                         		/*tp_str*/
    0,                         		/*tp_getattro*/
    0,                         		/*tp_setattro*/
    0,                         		/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,/*tp_flags*/
    0,                                  /* tp_doc */
    0,		               		/* tp_traverse */
    0,		               		/* tp_clear */
    0,		               		/* tp_richcompare */
    0,		               		/* tp_weaklistoffset */
    0,		               		/* tp_iter */
    0,		               		/* tp_iternext */
    pycairosvg_methods,        		/* tp_methods */
    0,                         		/* tp_members */
    0,        		                /* tp_getset */
    0,                         		/* tp_base */
    0,                         		/* tp_dict */
    0,                         		/* tp_descr_get */
    0,                         		/* tp_descr_set */
    0,                         		/* tp_dictoffset */
    0,               		        /* tp_init */
    0,                         		/* tp_alloc */
    (newfunc)pycairosvg_new,   		/* tp_new */
};
