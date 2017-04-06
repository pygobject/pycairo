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
 *                 Maarten Breddels
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo-private.h"

int
pycairo_check_status(cairo_status_t status)
{
    switch (status) {
    case CAIRO_STATUS_SUCCESS:
	return 0;
    case CAIRO_STATUS_NO_MEMORY:
	PyErr_NoMemory();
	return 1;
    case CAIRO_STATUS_INVALID_RESTORE:
	PyErr_SetString(PyExc_RuntimeError, "Context.restore without "
			"matching Context.save");
	return 1;
    case CAIRO_STATUS_INVALID_POP_GROUP:
	PyErr_SetString(PyExc_RuntimeError, "Context.pop_group without "
			"matching Context.push_group");
	return 1;
    case CAIRO_STATUS_NO_CURRENT_POINT:
	PyErr_SetString(PyExc_ValueError, "no current point defined");
	return 1;
    case CAIRO_STATUS_INVALID_MATRIX:
	PyErr_SetString(PyExc_ValueError, "invalid matrix (not invertible)");
	return 1;
    case CAIRO_STATUS_NO_TARGET_SURFACE:
	PyErr_SetString(PyExc_RuntimeError, "no target surface has been set");
	return 1;
    case CAIRO_STATUS_NULL_POINTER:
	PyErr_SetString(PyExc_RuntimeError, "NULL pointer");
	return 1;
    case CAIRO_STATUS_INVALID_STRING:
	PyErr_SetString(PyExc_RuntimeError, "invalid string");
	return 1;
    default:
	PyErr_SetString(PyExc_RuntimeError, "other cairo error");
	return 1;
    }
}

static PyObject *
pycairo_image_surface_create_for_data(PyObject *self, PyObject *args)
{
    char *data;
    cairo_format_t format;
    int length, width, height, stride = -1;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "w#iii|i:surface_create_for_image",
			  &data, &length, &format, &width, &height, &stride))
	return NULL;

    if (width <= 0) {
	PyErr_SetString(PyExc_ValueError, "width must be positive");
	return NULL;
    }
    if (height <= 0) {
	PyErr_SetString(PyExc_ValueError, "height must be positive");
	return NULL;
    }
    /* if stride is missing, calculate it from width */
    if (stride < 0)
	switch (format) {
	case CAIRO_FORMAT_ARGB32:
	    stride = width * 4;
	    break;
	case CAIRO_FORMAT_RGB24:
	    stride = width * 3;
	    break;
	case CAIRO_FORMAT_A8:
	    stride = width;
	    break;
	case CAIRO_FORMAT_A1:
	    stride = (width + 1) / 8;
	    break;
	}
    if (height * stride > length) {
	PyErr_SetString(PyExc_TypeError, "buffer is not long enough");
	return NULL;
    }

    surface = cairo_image_surface_create_for_data(data, format,
						  width, height, stride);
    if (!surface)
	return PyErr_NoMemory();
    /* should get surface to hold reference to buffer ... */

    return pycairo_surface_wrap(surface);
}

#ifdef CAIRO_HAS_PS_SURFACE
static PyObject *
pycairo_ps_surface_create(PyObject *self, PyObject *args)
{
    PyObject *file_object;
    int width_inches, height_inches, x_pixels_per_inch, y_pixels_per_inch;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!iiii:ps_surface_create",
			  &PyFile_Type, &file_object, &width_inches, &height_inches, &x_pixels_per_inch, &y_pixels_per_inch))
	return NULL;
    if (width_inches <= 0) {
	PyErr_SetString(PyExc_ValueError, "width_inches must be positive");
	return NULL;
    }
    if (height_inches <= 0) {
	PyErr_SetString(PyExc_ValueError, "height_inches must be positive");
	return NULL;
    }
    if (x_pixels_per_inch <= 0) {
	PyErr_SetString(PyExc_ValueError, "x_pixels_per_inch must be positive");
	return NULL;
    }
    if (y_pixels_per_inch <= 0) {
	PyErr_SetString(PyExc_ValueError, "y_pixels_per_inch must be positive");
	return NULL;
    }
    surface = cairo_ps_surface_create(PyFile_AsFile(file_object), width_inches, height_inches, x_pixels_per_inch, y_pixels_per_inch);
    if (!surface)
	return PyErr_NoMemory();

    return pycairo_surface_wrap(surface);
}
#endif /* CAIRO_HAS_PS_SURFACE */

#ifdef CAIRO_HAS_PDF_SURFACE
static PyObject *
pycairo_pdf_surface_create(PyObject *self, PyObject *args)
{
    PyObject *file_object;
    int width_inches, height_inches, x_pixels_per_inch, y_pixels_per_inch;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!iiii:pdf_surface_create",
			  &PyFile_Type, &file_object, &width_inches, &height_inches, &x_pixels_per_inch, &y_pixels_per_inch))
	return NULL;
    if (width_inches <= 0) {
	PyErr_SetString(PyExc_ValueError, "width_inches must be positive");
	return NULL;
    }
    if (height_inches <= 0) {
	PyErr_SetString(PyExc_ValueError, "height_inches must be positive");
	return NULL;
    }
    if (x_pixels_per_inch <= 0) {
	PyErr_SetString(PyExc_ValueError, "x_pixels_per_inch must be positive");
	return NULL;
    }
    if (y_pixels_per_inch <= 0) {
	PyErr_SetString(PyExc_ValueError, "y_pixels_per_inch must be positive");
	return NULL;
    }
    surface = cairo_pdf_surface_create(PyFile_AsFile(file_object), width_inches, height_inches, x_pixels_per_inch, y_pixels_per_inch);
    if (!surface)
	return PyErr_NoMemory();

    return pycairo_surface_wrap(surface);
}
#endif /* CAIRO_HAS_PDF_SURFACE */

#ifdef CAIRO_HAS_PNG_SURFACE
static PyObject *
pycairo_png_surface_create(PyObject *self, PyObject *args)
{
    PyObject *file_object;
    cairo_format_t format;
    int width, height;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!iii:png_surface_create",
			  &PyFile_Type, &file_object, &format, &width, &height))
	return NULL;

    if (width <= 0) {
	PyErr_SetString(PyExc_ValueError, "width must be positive");
	return NULL;
    }
    if (height <= 0) {
	PyErr_SetString(PyExc_ValueError, "height must be positive");
	return NULL;
    }

    surface = cairo_png_surface_create(PyFile_AsFile(file_object), format, width, height);
    if (!surface)
	return PyErr_NoMemory();

    return pycairo_surface_wrap(surface);
}
#endif /* CAIRO_HAS_PNG_SURFACE */


static PyMethodDef cairo_functions[] = {
    { "image_surface_create_for_data", (PyCFunction)pycairo_image_surface_create_for_data, METH_VARARGS, "" },
#ifdef CAIRO_HAS_PDF_SURFACE
    { "pdf_surface_create", (PyCFunction)pycairo_pdf_surface_create, METH_VARARGS, "" },
#endif
#ifdef CAIRO_HAS_PNG_SURFACE
    { "png_surface_create", (PyCFunction)pycairo_png_surface_create, METH_VARARGS, "" },
#endif
#ifdef CAIRO_HAS_PS_SURFACE
    { "ps_surface_create", (PyCFunction)pycairo_ps_surface_create, METH_VARARGS, "" },
#endif
    /* this is the old function name, should use image_surface_create_for_data */
    { "surface_create_for_image", (PyCFunction)pycairo_image_surface_create_for_data, METH_VARARGS, "this is the old function name, should use image_surface_create_for_data" },
    { NULL, NULL, 0 }
};

static struct _PyCairo_FunctionStruct api = {
    pycairo_check_status,
    &PyCairoMatrix_Type,
    pycairo_matrix_wrap,
    &PyCairoSurface_Type,
    pycairo_surface_wrap,
    &PyCairoFont_Type,
    pycairo_font_wrap,
    &PyCairoContext_Type,
    pycairo_context_wrap,
    &PyCairoPattern_Type,
    pycairo_pattern_wrap,
};

DL_EXPORT(void)
init_cairo(void)
{
    PyObject *mod;

#define INIT_TYPE(tp) \
    if (!tp.ob_type) tp.ob_type = &PyType_Type; \
    if (!tp.tp_alloc) tp.tp_alloc = PyType_GenericAlloc; \
    if (!tp.tp_new) tp.tp_new = PyType_GenericNew; \
    if (PyType_Ready(&tp) < 0) \
        return;

    INIT_TYPE(PyCairoMatrix_Type);
    INIT_TYPE(PyCairoSurface_Type);
    INIT_TYPE(PyCairoPattern_Type);
    INIT_TYPE(PyCairoFont_Type);
    INIT_TYPE(PyCairoContext_Type);

#undef INIT_TYPE

    mod = Py_InitModule("cairo._cairo", cairo_functions);

    PyModule_AddObject(mod, "Matrix",  (PyObject *)&PyCairoMatrix_Type);
    PyModule_AddObject(mod, "Surface", (PyObject *)&PyCairoSurface_Type);
    PyModule_AddObject(mod, "Pattern", (PyObject *)&PyCairoPattern_Type);
    PyModule_AddObject(mod, "Font", (PyObject *)&PyCairoFont_Type);
    PyModule_AddObject(mod, "Context", (PyObject *)&PyCairoContext_Type);

    PyModule_AddObject(mod, "_PyCairo_API",
		       PyCObject_FromVoidPtr(&api, NULL));

    /* constants */
#define CONSTANT(x) PyModule_AddIntConstant(mod, #x, CAIRO_##x)
    CONSTANT(FORMAT_ARGB32);
    CONSTANT(FORMAT_RGB24);
    CONSTANT(FORMAT_A8);
    CONSTANT(FORMAT_A1);

    CONSTANT(OPERATOR_CLEAR);
    CONSTANT(OPERATOR_SRC);
    CONSTANT(OPERATOR_DST);
    CONSTANT(OPERATOR_OVER);
    CONSTANT(OPERATOR_OVER_REVERSE);
    CONSTANT(OPERATOR_IN);
    CONSTANT(OPERATOR_IN_REVERSE);
    CONSTANT(OPERATOR_OUT);
    CONSTANT(OPERATOR_OUT_REVERSE);
    CONSTANT(OPERATOR_ATOP);
    CONSTANT(OPERATOR_ATOP_REVERSE);
    CONSTANT(OPERATOR_XOR);
    CONSTANT(OPERATOR_ADD);
    CONSTANT(OPERATOR_SATURATE);

    CONSTANT(FILL_RULE_WINDING);
    CONSTANT(FILL_RULE_EVEN_ODD);

    CONSTANT(LINE_CAP_BUTT);
    CONSTANT(LINE_CAP_ROUND);
    CONSTANT(LINE_CAP_SQUARE);

    CONSTANT(LINE_JOIN_MITER);
    CONSTANT(LINE_JOIN_ROUND);
    CONSTANT(LINE_JOIN_BEVEL);

    CONSTANT(FILTER_FAST);
    CONSTANT(FILTER_GOOD);
    CONSTANT(FILTER_BEST);
    CONSTANT(FILTER_NEAREST);
    CONSTANT(FILTER_BILINEAR);
    CONSTANT(FILTER_GAUSSIAN);

    CONSTANT(FONT_WEIGHT_NORMAL);
    CONSTANT(FONT_WEIGHT_BOLD);

    CONSTANT(FONT_SLANT_NORMAL);
    CONSTANT(FONT_SLANT_ITALIC);
    CONSTANT(FONT_SLANT_OBLIQUE);

    CONSTANT(EXTEND_NONE);
    CONSTANT(EXTEND_REPEAT);
    CONSTANT(EXTEND_REFLECT);
#undef CONSTANT

}
