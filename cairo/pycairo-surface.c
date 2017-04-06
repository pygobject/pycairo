/* -*- mode: C; c-basic-offset: 4 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2003-2005 James Henstridge
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

#include <stdint.h>
#include "pycairo-private.h"


#ifdef HAVE_NUMPY
#  include <Numeric/arrayobject.h>
   static int load_numpy (void);
#endif

/* Class Surface ---------------------------------------------------------- */

/* PycairoSurface_FromSurface
 * Create a new PycairoSurface from a cairo_surface_t
 * surface - a cairo_surface_t to 'wrap' into a Python object.
 *           it is unreferenced if the PycairoSurface creation fails, or if
 *           the cairo_surface_t has an error status
 * base - the base object used to create the context, or NULL.
 *        it is referenced to keep it alive while the cairo_surface_t is
 *        being used
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoSurface_FromSurface (cairo_surface_t *surface, PyObject *base)
{
    PyTypeObject *type;
    PyObject *o;

    assert (surface != NULL);

    if (Pycairo_Check_Status (cairo_surface_status (surface))) {
	cairo_surface_destroy (surface);
	return NULL;
    }

    switch (cairo_surface_get_type (surface)) {
    case CAIRO_SURFACE_TYPE_IMAGE:
	type = &PycairoImageSurface_Type;
	break;
#if CAIRO_HAS_PDF_SURFACE
    case CAIRO_SURFACE_TYPE_PDF:
	type = &PycairoPDFSurface_Type;
	break;
#endif
#if CAIRO_HAS_PS_SURFACE
    case CAIRO_SURFACE_TYPE_PS:
	type = &PycairoPSSurface_Type;
	break;
#endif
#if CAIRO_HAS_SVG_SURFACE
    case CAIRO_SURFACE_TYPE_SVG:
	type = &PycairoSVGSurface_Type;
	break;
#endif
#if CAIRO_HAS_WIN32_SURFACE
    case CAIRO_SURFACE_TYPE_WIN32:
	type = &PycairoWin32Surface_Type;
	break;
#endif
#if CAIRO_HAS_XLIB_SURFACE
    case CAIRO_SURFACE_TYPE_XLIB:
	type = &PycairoXlibSurface_Type;
	break;
#endif
    default:
	PyErr_SetString(CairoError, "Unsupported Surface type");
	return NULL;
    }

    o = type->tp_alloc (type, 0);
    if (o == NULL) {
	cairo_surface_destroy (surface);
    } else {
	((PycairoSurface *)o)->surface = surface;
	Py_XINCREF(base);
	((PycairoSurface *)o)->base = base;
    }
    return o;
}

/* for use with
 * cairo_surface_write_to_png_stream()
 * cairo_pdf/ps/svg_surface_create_for_stream()
 */
static cairo_status_t
_write_func (void *closure, const unsigned char *data, unsigned int length)
{
    PyObject *res = PyObject_CallMethod ((PyObject *)closure, "write", "(s#)",
					 data, length);
    if (res == NULL) {
	/* an exception has occurred, it will be picked up later by
	 * Pycairo_Check_Status()
	 */
	return CAIRO_STATUS_WRITE_ERROR;
    }
    Py_DECREF(res);
    return CAIRO_STATUS_SUCCESS;
}

static void
surface_dealloc (PycairoSurface *o)
{
#ifdef DEBUG
    printf("surface_dealloc start\n");
#endif
    if (o->surface) {
	cairo_surface_destroy(o->surface);
	o->surface = NULL;
    }
    Py_CLEAR(o->base);

    o->ob_type->tp_free((PyObject *)o);
#ifdef DEBUG
    printf("surface_dealloc end\n");
#endif
}

static PyObject *
surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyErr_SetString(PyExc_TypeError,
		    "The Surface type cannot be instantiated");
    return NULL;
}

static PyObject *
surface_create_similar (PycairoSurface *o, PyObject *args)
{
    cairo_content_t content;
    int width, height;

    if (!PyArg_ParseTuple (args, "iii:Surface.create_similar",
			   &content, &width, &height))
	return NULL;
    return PycairoSurface_FromSurface (
	     cairo_surface_create_similar (o->surface, content, width, height),
	     NULL);
}

static PyObject *
surface_finish (PycairoSurface *o)
{
    cairo_surface_finish (o->surface);
    Py_CLEAR(o->base);

    if (Pycairo_Check_Status (cairo_surface_status(o->surface)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
surface_flush (PycairoSurface *o)
{
    cairo_surface_flush (o->surface);

    if (Pycairo_Check_Status (cairo_surface_status(o->surface)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
surface_get_device_offset (PycairoSurface *o)
{
    double x_offset, y_offset;
    cairo_surface_get_device_offset (o->surface, &x_offset, &y_offset);
    return Py_BuildValue("(dd)", x_offset, y_offset);
}

static PyObject *
surface_get_font_options (PycairoSurface *o)
{
    cairo_font_options_t *options = cairo_font_options_create();

    cairo_surface_get_font_options (o->surface, options);
    /* there is no reference fn */
    return PycairoFontOptions_FromFontOptions (options);
}

static PyObject *
surface_mark_dirty (PycairoSurface *o, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"x", "y", "width", "height", NULL};
    int x = 0, y = 0, width = -1, height = -1;

    if (!PyArg_ParseTupleAndKeywords(args, kwds,
				     "|iiii:Surface.mark_dirty", kwlist,
				     &x, &y, &width, &height))
	return NULL;

    cairo_surface_mark_dirty_rectangle (o->surface, x, y, width, height);

    if (Pycairo_Check_Status (cairo_surface_status(o->surface)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
surface_set_device_offset (PycairoSurface *o, PyObject *args)
{
    double x_offset, y_offset;

    if (!PyArg_ParseTuple (args, "dd:Surface.set_device_offset",
			   &x_offset, &y_offset))
	return NULL;

    cairo_surface_set_device_offset (o->surface, x_offset, y_offset);
    Py_RETURN_NONE;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
/* METH_O */
static PyObject *
surface_write_to_png (PycairoSurface *o, PyObject *file)
{
    cairo_status_t status;

    if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
	/* string (filename) argument */
	status = cairo_surface_write_to_png (o->surface,
					     PyString_AsString(file));

    } else {  /* file or file-like object argument */
	PyObject* writer = PyObject_GetAttrString (file, "write");
	if (writer == NULL || !PyCallable_Check (writer)) {
	    Py_XDECREF(writer);
	    PyErr_SetString(PyExc_TypeError,
"Surface.write_to_png takes one argument which must be a filename (str), file "
"object, or a file-like object which has a \"write\" method (like StringIO)");
	    return NULL;
	}
	Py_DECREF(writer);
	status = cairo_surface_write_to_png_stream (o->surface, _write_func,
						    file);
    }
    if (Pycairo_Check_Status (status))
	return NULL;
    Py_RETURN_NONE;
}
#endif  /* CAIRO_HAS_PNG_FUNCTIONS */


static PyMethodDef surface_methods[] = {
    /* methods never exposed in a language binding:
     * cairo_surface_destroy()
     * cairo_surface_get_type()
     * cairo_surface_get_user_data()
     * cairo_surface_reference()
     * cairo_surface_set_user_data()
     */
    {"create_similar", (PyCFunction)surface_create_similar,    METH_VARARGS },
    {"finish",         (PyCFunction)surface_finish,            METH_NOARGS },
    {"flush",          (PyCFunction)surface_flush,             METH_NOARGS },
    {"get_device_offset",(PyCFunction)surface_get_device_offset,
                                                                METH_NOARGS },
    {"get_font_options",(PyCFunction)surface_get_font_options, METH_NOARGS },
    {"mark_dirty",     (PyCFunction)surface_mark_dirty,        METH_KEYWORDS },
    {"set_device_offset",(PyCFunction)surface_set_device_offset,
                                                                METH_VARARGS },
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    {"write_to_png",   (PyCFunction)surface_write_to_png,      METH_O },
#endif
    {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoSurface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Surface",                    /* tp_name */
    sizeof(PycairoSurface),             /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)surface_dealloc,        /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,/* tp_flags */
    0,                                  /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    surface_methods,                    /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0, /* &PyBaseObject_Type, */        /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)surface_new,               /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};


/* Class ImageSurface(Surface) -------------------------------------------- */

static PyObject *
image_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    cairo_format_t format;
    int width, height;

    if (!PyArg_ParseTuple (args, "iii:ImageSurface.__new__",
			   &format, &width, &height))
	return NULL;
    return PycairoSurface_FromSurface (
	       cairo_image_surface_create (format, width, height),
	       NULL);
}

#ifdef HAVE_NUMPY
static PyObject *
image_surface_create_for_array (PyTypeObject *type, PyObject *args)
{
    PyArrayObject *array;
    cairo_format_t format;
    cairo_surface_t *surface;
    int nd;

    if (!load_numpy())
	return NULL;

    if (!PyArg_ParseTuple(args, "O!:surface_create_for_array",
			  &PyArray_Type, &array))
	return NULL;

    if (array->descr->type_num != PyArray_UBYTE) {
	PyErr_SetString(PyExc_TypeError, "array data must be unsigned bytes");
	return NULL;
    }

    nd = array->nd;
    if (nd < 2) {
	PyErr_SetString(PyExc_TypeError,
			"array must have at least two dimensions");
	return NULL;
    }
    if (nd == 2 || (nd == 3 && array->dimensions[2] == 1)) {
	if (array->strides[1] != 1) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_A8;
    } else if (nd == 3 && array->dimensions[2] == 3) {
	if (array->strides[1] != 3) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_RGB24;
    } else if (nd == 3 && array->dimensions[2] == 4) {
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
    surface = cairo_image_surface_create_for_data(
		                          (unsigned char *) array->data,
					  format,
					  array->dimensions[1],
					  array->dimensions[0],
					  array->strides[0]);
    return PycairoSurface_FromSurface(surface, (PyObject *)array);
}
#endif /* HAVE_NUMPY */

static PyObject *
image_surface_create_for_data (PyTypeObject *type, PyObject *args)
{
    cairo_surface_t *surface;
    cairo_format_t format;
    unsigned char *buffer;
    int buffer_len, width, height, stride = -1, res;
    PyObject *obj;

    if (!PyArg_ParseTuple(args, "Oiii|i:Surface.create_for_data",
			  &obj, &format, &width, &height, &stride))
	return NULL;

    res = PyObject_AsWriteBuffer (obj, (void **)&buffer, &buffer_len);
    if (res == -1)
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
    if (stride < 0) {
	switch (format) {
	case CAIRO_FORMAT_ARGB32:
	case CAIRO_FORMAT_RGB24:
	    stride = width * 4;
	    break;
	case CAIRO_FORMAT_A8:
	    stride = width;
	    break;
	case CAIRO_FORMAT_A1:
	    stride = (width + 1) / 8;
	    break;
	default:
	    ASSERT_NOT_REACHED;
	}
    }
    if (height * stride > buffer_len) {
	PyErr_SetString(PyExc_TypeError, "buffer is not long enough");
	return NULL;
    }
    surface = cairo_image_surface_create_for_data (buffer, format, width,
						   height, stride);
    return PycairoSurface_FromSurface(surface, obj);
}


#ifdef CAIRO_HAS_PNG_FUNCTIONS
static cairo_status_t
_read_func1 (void *closure, unsigned char *data, unsigned int length)
{
    if (fread (data, 1, (size_t) length, (FILE *)closure) != length)
	return CAIRO_STATUS_READ_ERROR;
    return CAIRO_STATUS_SUCCESS;
}

static cairo_status_t
_read_func2 (void *closure, unsigned char *data, unsigned int length)
{
    char *str;
    PyObject *pystr = PyObject_CallMethod ((PyObject *)closure, "read", "(i)",
					   length);
    if (pystr == NULL)
	return CAIRO_STATUS_READ_ERROR;
    str = PyString_AsString(pystr);
    Py_DECREF(pystr);
    if (str == NULL)
	return CAIRO_STATUS_READ_ERROR;
    /* don't use strncpy() since png data may contain NUL bytes */
    memcpy (data, str, length);
    return CAIRO_STATUS_SUCCESS;
}

/* METH_O | METH_CLASS */
static PyObject *
image_surface_create_from_png (PyTypeObject *type, PyObject *file)
{
    FILE *fp = NULL;
    cairo_surface_t *surface;
    unsigned int mode = 0;

    if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
	fp = fopen (PyString_AsString(file), "rb");
	if (fp == NULL) {
	    PyErr_SetString(PyExc_IOError, "unable to open file for reading");
	    return NULL;
	}
	mode = 1;
    } else if (PyObject_TypeCheck (file, &PyFile_Type)) {
	fp = PyFile_AsFile(file);
	mode = 1;
    } else {
	PyObject* reader = PyObject_GetAttrString (file, "read");
	if (reader) {
	    if (PyCallable_Check (reader))
		mode = 2;
	    Py_DECREF(reader);
	}
    }
    if (mode == 1) {
	surface = cairo_image_surface_create_from_png_stream (_read_func1, fp);
    } else if (mode == 2) {
	surface = cairo_image_surface_create_from_png_stream (_read_func2,
							      file);
    } else {
	PyErr_SetString(PyExc_TypeError,
			"ImageSurface.create_from_png takes one argument "
			"which must be a filename (str), file object, or an "
			"object that has a \"read\" method (like StringIO)");
	return NULL;
    }

    if (PyObject_TypeCheck (file, &PyBaseString_Type))
	fclose (fp);

    return PycairoSurface_FromSurface (surface, NULL);
}
#endif /* CAIRO_HAS_PNG_FUNCTIONS */

static PyObject *
image_surface_get_height (PycairoImageSurface *o)
{
    return Py_BuildValue("i", cairo_image_surface_get_height (o->surface));
}

static PyObject *
image_surface_get_width (PycairoImageSurface *o)
{
    return Py_BuildValue("i", cairo_image_surface_get_width (o->surface));
}

#ifdef HAVE_GETDATA
/* This was modified from cairo/src/cairo-png.c unpremultiply_data() */
/* ARGB32 (native-endian, premultiplied) => RGBA */
static void
_argb32_to_unpremultiplied_rgba (uint8_t *data, int length)
{
    unsigned int i;

    for (i = 0; i < length; i += 4) {
        uint8_t *b = &data[i];
        uint32_t pixel;
        uint8_t  alpha;

	memcpy (&pixel, b, sizeof (uint32_t));
	alpha = (pixel & 0xff000000) >> 24;
        if (alpha == 0) {
	    b[0] = b[1] = b[2] = b[3] = 0;
	} else {
            b[0] = (((pixel & 0xff0000) >> 16) * 255 + alpha / 2) / alpha;
            b[1] = (((pixel & 0x00ff00) >>  8) * 255 + alpha / 2) / alpha;
            b[2] = (((pixel & 0x0000ff) >>  0) * 255 + alpha / 2) / alpha;
	    b[3] = alpha;
	}
    }
}

/* return a Python buffer object containing the ImageSurface data in RGBA
 * format.
 * It requires a patched version of cairo which provides:
 * cairo_image_surface_get_data()
 * cairo_image_surface_get_format()
 * cairo_image_surface_get_stride()
 */
static PyObject *
image_surface_to_rgba (PycairoImageSurface *o)
{
    PyObject *buf;
    unsigned char *data;
    uint8_t *buffer;
    int height, stride, length;
    cairo_surface_t *surface = o->surface;
    cairo_format_t format = cairo_image_surface_get_format (surface);

    if (format != CAIRO_FORMAT_ARGB32) {
	PyErr_SetString(PyExc_TypeError, "ImageSurface.to_rgba() can only be "
			"called on a cairo.FORMAT_ARGB32 surface");
	return NULL;
    }
    data   = cairo_image_surface_get_data (surface);
    height = cairo_image_surface_get_height (surface);
    stride = cairo_image_surface_get_stride (surface);

    buf = PyBuffer_New(height * stride);
    if (buf != NULL) {
	if (PyObject_AsWriteBuffer(buf, (void **)&buffer, &length)) {
	    Py_DECREF(buf);
	    return NULL;
	}
	memcpy (buffer, data, length);
	_argb32_to_unpremultiplied_rgba (buffer, length);
    }
    return buf;
}
#endif

static PyMethodDef image_surface_methods[] = {
#ifdef HAVE_NUMPY
    {"create_for_array",(PyCFunction)image_surface_create_for_array,
                                                   METH_VARARGS | METH_CLASS },
#endif
    {"create_for_data",(PyCFunction)image_surface_create_for_data,
                                                   METH_VARARGS | METH_CLASS },
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    {"create_from_png", (PyCFunction)image_surface_create_from_png,
                                                   METH_O | METH_CLASS },
#endif
    {"get_height",    (PyCFunction)image_surface_get_height,     METH_NOARGS},
    {"get_width",     (PyCFunction)image_surface_get_width,      METH_NOARGS},
#ifdef HAVE_GETDATA
    {"to_rgba",       (PyCFunction)image_surface_to_rgba,        METH_NOARGS},
#endif
    {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoImageSurface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.ImageSurface",               /* tp_name */
    sizeof(PycairoImageSurface),        /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
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
    image_surface_methods,              /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0, /* &PycairoSurface_Type, */      /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)image_surface_new,         /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};


/* Class PDFSurface(Surface) ---------------------------------------------- */
#ifdef CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>

static PyObject *
pdf_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    double width_in_points, height_in_points;
    PyObject *file;

    if (!PyArg_ParseTuple(args, "Odd:PDFSurface.__new__",
			  &file, &width_in_points, &height_in_points))
	return NULL;

    if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
	/* string (filename) argument */
	return PycairoSurface_FromSurface (
                  cairo_pdf_surface_create (PyString_AsString(file),
                                     width_in_points, height_in_points),
	       NULL);

    } else {  /* file or file-like object argument */
	PyObject* writer = PyObject_GetAttrString (file, "write");
	if (writer == NULL || !PyCallable_Check (writer)) {
	    Py_XDECREF(writer);
	    PyErr_SetString(PyExc_TypeError,
"PDFSurface argument 1 must be a filename (str), file object, or an object "
"that has a \"write\" method (like StringIO)");
	    return NULL;
	}
	Py_DECREF(writer);

	return PycairoSurface_FromSurface (
	           cairo_pdf_surface_create_for_stream (_write_func,
		       file, width_in_points, height_in_points),
	       file);
    }
}

static PyObject *
pdf_surface_set_dpi (PycairoPDFSurface *o, PyObject *args)
{
    double x_dpi, y_dpi;

    if (!PyArg_ParseTuple(args, "dd:PDFSurface.set_dpi", &x_dpi, &y_dpi))
	return NULL;
    cairo_pdf_surface_set_dpi (o->surface, x_dpi, y_dpi);
    Py_RETURN_NONE;
}

static PyObject *
pdf_surface_set_size (PycairoPDFSurface *o, PyObject *args)
{
    double width_in_points, height_in_points;

    if (!PyArg_ParseTuple(args, "dd:PDFSurface.set_size", &width_in_points,
			  &height_in_points))
	return NULL;
    cairo_pdf_surface_set_size (o->surface, width_in_points,
				height_in_points);
    Py_RETURN_NONE;
}

static PyMethodDef pdf_surface_methods[] = {
    {"set_dpi",  (PyCFunction)pdf_surface_set_dpi,     METH_VARARGS },
    {"set_size", (PyCFunction)pdf_surface_set_size,    METH_VARARGS },
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoPDFSurface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.PDFSurface",                 /* tp_name */
    sizeof(PycairoPDFSurface),          /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
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
    pdf_surface_methods,                /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0, /* &PycairoSurface_Type, */      /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)pdf_surface_new,           /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
#endif /* CAIRO_HAS_PDF_SURFACE */


/* Class PSSurface(Surface) ----------------------------------------------- */
#ifdef CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>

static PyObject *
ps_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    double width_in_points, height_in_points;
    PyObject *file;

    if (!PyArg_ParseTuple(args, "Odd:PSSurface.__new__",
			  &file, &width_in_points, &height_in_points))
	return NULL;

    if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
	/* string (filename) argument */
	return PycairoSurface_FromSurface (
                  cairo_ps_surface_create (PyString_AsString(file),
                                     width_in_points, height_in_points),
	       NULL);

    } else {  /* file or file-like object argument */
	PyObject* writer = PyObject_GetAttrString (file, "write");
	if (writer == NULL || !PyCallable_Check (writer)) {
	    Py_XDECREF(writer);
	    PyErr_SetString(PyExc_TypeError,
"PSSurface argument 1 must be a filename (str), file object, or an object "
"that has a \"write\" method (like StringIO)");
	    return NULL;
	}
	Py_DECREF(writer);

	return PycairoSurface_FromSurface (
	           cairo_ps_surface_create_for_stream (_write_func,
		       file, width_in_points, height_in_points),
	       file);
    }
}

static PyObject *
ps_surface_dsc_begin_page_setup (PycairoPSSurface *o)
{
    cairo_ps_surface_dsc_begin_page_setup (o->surface);
    if (Pycairo_Check_Status (cairo_surface_status (o->surface)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
ps_surface_dsc_begin_setup (PycairoPSSurface *o)
{
    cairo_ps_surface_dsc_begin_setup (o->surface);
    if (Pycairo_Check_Status (cairo_surface_status (o->surface)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
ps_surface_dsc_comment (PycairoPSSurface *o, PyObject *args)
{
    const char *comment;

    if (!PyArg_ParseTuple(args, "s:PSSurface.dsc_comment", &comment))
	return NULL;

    cairo_ps_surface_dsc_comment (o->surface, comment);
    if (Pycairo_Check_Status (cairo_surface_status (o->surface)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
ps_surface_set_dpi (PycairoPSSurface *o, PyObject *args)
{
    double x_dpi, y_dpi;

    if (!PyArg_ParseTuple(args, "dd:PSSurface.set_dpi", &x_dpi, &y_dpi))
	return NULL;
    cairo_ps_surface_set_dpi (o->surface, x_dpi, y_dpi);
    Py_RETURN_NONE;
}

static PyObject *
ps_surface_set_size (PycairoPSSurface *o, PyObject *args)
{
    double width_in_points, height_in_points;

    if (!PyArg_ParseTuple(args, "dd:PSSurface.set_size",
			  &width_in_points, &height_in_points))
	return NULL;
    cairo_ps_surface_set_size (o->surface, width_in_points, height_in_points);
    Py_RETURN_NONE;
}

static PyMethodDef ps_surface_methods[] = {
    {"dsc_begin_page_setup",
                   (PyCFunction)ps_surface_dsc_begin_page_setup, METH_NOARGS },
    {"dsc_begin_setup", (PyCFunction)ps_surface_dsc_begin_setup, METH_NOARGS },
    {"dsc_comment", (PyCFunction)ps_surface_dsc_comment,        METH_VARARGS },
    {"set_dpi",  (PyCFunction)ps_surface_set_dpi,               METH_VARARGS },
    {"set_size", (PyCFunction)ps_surface_set_size,              METH_VARARGS },
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoPSSurface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.PSSurface",                  /* tp_name */
    sizeof(PycairoPSSurface),           /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
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
    ps_surface_methods,                 /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0, /* &PycairoSurface_Type, */      /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)ps_surface_new,            /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
#endif  /* CAIRO_HAS_PS_SURFACE */


/* Class SVGSurface(Surface) ----------------------------------------------- */
#ifdef CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>

static PyObject *
svg_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    double width_in_points, height_in_points;
    PyObject *file;

    if (!PyArg_ParseTuple(args, "Odd:SVGSurface.__new__",
			  &file, &width_in_points, &height_in_points))
	return NULL;

    if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
	/* string (filename) argument */
	return PycairoSurface_FromSurface (
                  cairo_svg_surface_create (PyString_AsString(file),
                                     width_in_points, height_in_points),
	       NULL);

    } else {  /* file or file-like object argument */
	PyObject* writer = PyObject_GetAttrString (file, "write");
	if (writer == NULL || !PyCallable_Check (writer)) {
	    Py_XDECREF(writer);
	    PyErr_SetString(PyExc_TypeError,
"SVGSurface argument 1 must be a filename (str), file object, or an object "
"that has a \"write\" method (like StringIO)");
	    return NULL;
	}
	Py_DECREF(writer);

	return PycairoSurface_FromSurface (
	           cairo_svg_surface_create_for_stream (_write_func,
		       file, width_in_points, height_in_points),
	       file);
    }
}

static PyObject *
svg_surface_set_dpi (PycairoSVGSurface *o, PyObject *args)
{
    double x_dpi, y_dpi;

    if (!PyArg_ParseTuple(args, "dd:SVGSurface.set_dpi", &x_dpi, &y_dpi))
	return NULL;
    cairo_svg_surface_set_dpi (o->surface, x_dpi, y_dpi);
    Py_RETURN_NONE;
}

static PyMethodDef svg_surface_methods[] = {
    /* TODO
     * cairo_svg_surface_restrict_to_version
     * cairo_svg_get_versions
     * cairo_svg_version_to_string
     */
    {"set_dpi", (PyCFunction)svg_surface_set_dpi,    METH_VARARGS },
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoSVGSurface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.SVGSurface",                 /* tp_name */
    sizeof(PycairoSVGSurface),          /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
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
    svg_surface_methods,                /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0, /* &PycairoSurface_Type, */      /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)svg_surface_new,           /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
#endif  /* CAIRO_HAS_SVG_SURFACE */


/* Class Win32Surface(Surface) -------------------------------------------- */
#if CAIRO_HAS_WIN32_SURFACE
#include <cairo-win32.h>

static PyObject *
win32_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    int hdc;

    if (!PyArg_ParseTuple(args, "i:Win32Surface.__new__", &hdc))
	return NULL;
    return PycairoSurface_FromSurface (
	       cairo_win32_surface_create ((HDC)hdc), NULL);
}

static PyMethodDef win32_surface_methods[] = {
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoWin32Surface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Win32Surface",               /* tp_name */
    sizeof(PycairoWin32Surface),        /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
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
    win32_surface_methods,              /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0, /* &PycairoSurface_Type, */      /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)win32_surface_new,         /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
#endif  /* CAIRO_HAS_WIN32_SURFACE */


/* Class XlibSurface(Surface) --------------------------------------------- */
#ifdef CAIRO_HAS_XLIB_SURFACE
#include <cairo-xlib.h>

static PyObject *
xlib_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyErr_SetString(PyExc_TypeError,
		    "The XlibSurface type cannot be directly instantiated");
    return NULL;
}

static PyObject *
xlib_surface_get_depth (PycairoXlibSurface *o)
{
    return Py_BuildValue("i", cairo_xlib_surface_get_depth (o->surface));
}

static PyMethodDef xlib_surface_methods[] = {
    {"get_depth", (PyCFunction)xlib_surface_get_depth,    METH_NOARGS },
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoXlibSurface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.XlibSurface",                /* tp_name */
    sizeof(PycairoXlibSurface),         /* tp_basicsize */
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
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
    xlib_surface_methods,               /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0, /* &PycairoSurface_Type, */      /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)xlib_surface_new,          /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
#endif  /* CAIRO_HAS_XLIB_SURFACE */


/* Numeric routines ------------------------------------------------------- */

#ifdef HAVE_NUMPY
/* load the Numeric Python module
 * Return 1 if Numeric is available,
 *        0 and set exception if it is not.
 *
 * copied from pygtk
 */
static int
load_numpy(void)
{
    static int import_done = 0;
    static PyObject *exc_type=NULL, *exc_value=NULL;
    PyObject *exc_tb=NULL;

    if (exc_type != NULL) {
	PyErr_Restore(exc_type, exc_value, NULL);
	return 0;
    }
    if (!import_done) {
	import_done = 1;
	import_array();
	if (PyErr_Occurred()) {
	    PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
	    Py_INCREF(exc_type);
	    Py_XINCREF(exc_value);
	    PyErr_Restore(exc_type, exc_value, exc_tb);
	    return 0;
	}
    }
    return 1;
}
#endif /* HAVE_NUMPY */
