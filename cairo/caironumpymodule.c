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
#include "pycairo.h"
#include <Numeric/arrayobject.h>

static PyObject *
surface_create_for_array(PyObject *self, PyObject *args)
{
    PyArrayObject *array;
    cairo_format_t format;
    int width, height, stride;
    char *data;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!:surface_create_for_array",
			  &PyArray_Type, &array))
	return NULL;

    if (array->descr->type_num != PyArray_UBYTE) {
	PyErr_SetString(PyExc_TypeError, "array data must be unsigned bytes");
	return NULL;
    }
    if (array->nd < 2) {
	PyErr_SetString(PyExc_TypeError,
			"array must have at least two dimensions");
	return NULL;
    }
    width = array->dimensions[1];
    height = array->dimensions[0];
    stride = array->strides[0];
    data = (char *)array->data;

    if (array->nd == 2 || (array->nd == 3 && array->dimensions[2] == 1)) {
	if (array->strides[1] != 1) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_A8;
    } else if (array->nd == 3 && array->dimensions[2] == 3) {
	if (array->strides[1] != 3) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_RGB24;
    } else if (array->nd == 3 && array->dimensions[2] == 4) {
	if (array->strides[1] != 4) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_ARGB32;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"array must be MxN or MxNxP where P is 1, 3 or 4");
	return NULL;
    }

    surface = cairo_surface_create_for_image(data, format,
					     width, height, stride);
    if (!surface)
	return PyErr_NoMemory();
    /* should get surface to hold reference to array ... */

    return pycairo_surface_wrap(surface);
}

static PyMethodDef caironumpy_functions[] = {
    { "surface_create_for_array", (PyCFunction)surface_create_for_array,
      METH_VARARGS },
    { NULL, NULL, 0 }
};

DL_EXPORT(void)
initnumpy(void)
{
    PyObject *mod;

    mod = Py_InitModule("cairo.numpy", caironumpy_functions);

    init_pycairo();
    import_array();
}
