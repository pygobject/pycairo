/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2004-2005 Steve Chaplin
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
#include "pycairo-private.h"


/* PycairoPattern_FromPattern
 * Create a new PycairoPattern from a cairo_pattern_t
 * pattern - a cairo_pattern_t to 'wrap' into a Python object.
 *           pattern is unreferenced if the PycairoPattern creation fails, or
 *           if the pattern is in an error status.
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoPattern_FromPattern (cairo_pattern_t *pattern)
{
    PyObject *o;

    assert (pattern != NULL);

    if (Pycairo_Check_Status (cairo_pattern_status (pattern))) {
	cairo_pattern_destroy (pattern);
	return NULL;
    }

    o = PycairoPattern_Type.tp_alloc (&PycairoPattern_Type, 0);
    if (o)
	((PycairoPattern *)o)->pattern = pattern;
    else
	cairo_pattern_destroy (pattern);
    return o;
}

static void
pattern_dealloc (PycairoPattern *o)
{
#ifdef DEBUG
    printf("pattern_dealloc start\n");
#endif
    if (o->pattern) {
	cairo_pattern_destroy (o->pattern);
	o->pattern = NULL;
    }
    o->ob_type->tp_free((PyObject *)o);
#ifdef DEBUG
    printf("pattern_dealloc end\n");
#endif
}

static PyObject *
pattern_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyErr_SetString(PyExc_TypeError,
		    "The Pattern type cannot be instantiated");
    return NULL;

}

static PyObject *
pattern_create_for_surface (PyTypeObject *type, PyObject *args)
{
    PycairoSurface *s;
    cairo_pattern_t *pattern;

    if (!PyArg_ParseTuple (args, "O!:Pattern.create_for_surface", 
			   &PycairoSurface_Type, &s))
	return NULL;

    pattern = cairo_pattern_create_for_surface (s->surface);
    return PycairoPattern_FromPattern (pattern);
}

static PyObject *
pattern_create_linear (PyTypeObject *type, PyObject *args)
{
    double x0, y0, x1, y1;
    cairo_pattern_t *pattern;
    
    if (!PyArg_ParseTuple(args, "dddd:Pattern.create_linear", 
			  &x0, &y0, &x1, &y1))
	return NULL;

    pattern = cairo_pattern_create_linear (x0, y0, x1, y1);
    return PycairoPattern_FromPattern (pattern);
}

static PyObject *
pattern_create_radial (PyTypeObject *type, PyObject *args)
{
    cairo_pattern_t *pattern;
    double cx0, cy0, radius0, cx1, cy1, radius1;

    if (!PyArg_ParseTuple(args, "dddddd:Pattern.create_radial", 
			  &cx0, &cy0, &radius0, &cx1, &cy1, &radius1))
	return NULL;

    pattern = cairo_pattern_create_radial (cx0, cy0, radius0, 
					   cx1, cy1, radius1);
    return PycairoPattern_FromPattern (pattern);
}

static PyObject *
pattern_add_color_stop_rgb (PycairoPattern *o, PyObject *args)
{
    double offset, red, green, blue;

    if (!PyArg_ParseTuple(args, "dddd:Pattern.add_color_stop_rgb",
			  &offset, &red, &green, &blue))
	return NULL;

    cairo_pattern_add_color_stop_rgb (o->pattern, offset, red, green, blue);
    if (Pycairo_Check_Status (cairo_pattern_status (o->pattern)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pattern_add_color_stop_rgba (PycairoPattern *o, PyObject *args)
{
    double offset, red, green, blue, alpha;

    if (!PyArg_ParseTuple(args, "ddddd:Pattern.add_color_stop_rgba",
			  &offset, &red, &green, &blue, &alpha))
	return NULL;

    cairo_pattern_add_color_stop_rgba (o->pattern, offset, red, 
				       green, blue, alpha);
    if (Pycairo_Check_Status (cairo_pattern_status (o->pattern)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pattern_get_extend (PycairoPattern *o)
{
    return PyInt_FromLong (cairo_pattern_get_extend (o->pattern));
}

static PyObject *
pattern_get_filter (PycairoPattern *o)
{
    return PyInt_FromLong (cairo_pattern_get_filter (o->pattern));
}

static PyObject *
pattern_get_matrix (PycairoPattern *o)
{
    cairo_matrix_t matrix;
    cairo_pattern_get_matrix (o->pattern, &matrix);
    return PycairoMatrix_FromMatrix (&matrix);
}

static PyObject *
pattern_set_extend (PycairoPattern *o, PyObject *args)
{
    int extend;

    if (!PyArg_ParseTuple(args, "i:Pattern.set_extend", &extend))
 	return NULL;
 
    cairo_pattern_set_extend (o->pattern, extend);
    Py_RETURN_NONE;
}
 
static PyObject *
pattern_set_filter (PycairoPattern *o, PyObject *args)
{
    int filter;

    if (!PyArg_ParseTuple (args, "i:Pattern.set_filter", &filter))
	return NULL;

    cairo_pattern_set_filter (o->pattern, filter);
    Py_RETURN_NONE;
}
  
static PyObject *
pattern_set_matrix (PycairoPattern *o, PyObject *args)
{
    PycairoMatrix *m;

    if (!PyArg_ParseTuple (args, "O!:Pattern.set_matrix",
			   &PycairoMatrix_Type, &m))
	return NULL;

    cairo_pattern_set_matrix (o->pattern, &m->matrix);
    Py_RETURN_NONE;
}

static PyMethodDef pattern_methods[] = {
    /* methods never exposed in a language binding:
     * cairo_pattern_destroy()
     * cairo_pattern_reference()
     *
     * cairo_pattern_status()
     * - not needed since Pycairo handles status checking
     */
    {"add_color_stop_rgb",(PyCFunction)pattern_add_color_stop_rgb,  
                                                               METH_VARARGS },
    {"add_color_stop_rgba",(PyCFunction)pattern_add_color_stop_rgba,  
                                                               METH_VARARGS },
    {"create_for_surface",(PyCFunction)pattern_create_for_surface, 
                                                  METH_VARARGS | METH_CLASS },
    {"create_linear",    (PyCFunction)pattern_create_linear, 
                                                  METH_VARARGS | METH_CLASS },
    {"create_radial",    (PyCFunction)pattern_create_radial, 
                                                  METH_VARARGS | METH_CLASS },
    {"get_extend",       (PyCFunction)pattern_get_extend,      METH_NOARGS },
    {"get_filter",       (PyCFunction)pattern_get_filter,      METH_NOARGS },
    {"get_matrix",       (PyCFunction)pattern_get_matrix,      METH_NOARGS },
    {"set_extend",       (PyCFunction)pattern_set_extend,      METH_VARARGS },
    {"set_filter",       (PyCFunction)pattern_set_filter,      METH_VARARGS },
    {"set_matrix",       (PyCFunction)pattern_set_matrix,      METH_VARARGS },
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoPattern_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                  /* ob_size */
    "cairo.Pattern",                    /* tp_name */
    sizeof(PycairoPattern),             /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)pattern_dealloc,        /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,                                  /* tp_repr */
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
    pattern_methods,                    /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    &PyBaseObject_Type,                 /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)pattern_new,               /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
