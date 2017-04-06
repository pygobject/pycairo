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

#include "pycairo-private.h"


#ifdef HAVE_NUMPY
#  include <Numeric/arrayobject.h>
   static int load_numpy (void);
#endif

/* Class Surface ---------------------------------------------------------- */

/* PycairoSurface_FromSurface
 * Create a new PycairoSurface from a cairo_surface_t
 * surface - a cairo_surface_t to 'wrap' into a Python object.
 *           it is unreferenced if the PycairoSurface creation fails
 * type - the type of the object to instantiate; it can be NULL,
 *        meaning a base cairo.Surface type, or it can be a subclass of
 *        cairo.Surface.
 * base - the base object used to create the context, or NULL.
 *        it is referenced to keep it alive while the cairo_surface_t is 
 *        being used
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoSurface_FromSurface (cairo_surface_t *surface, PyTypeObject *type, 
			    PyObject *base)
{
    PyObject *o;

    assert (surface != NULL);
    if (type == NULL)
        type = &PycairoSurface_Type;
    o = PycairoSurface_Type.tp_alloc (type, 0);
    if (o) {
	((PycairoSurface *)o)->surface = surface;
	Py_XINCREF(base);
	((PycairoSurface *)o)->base = base;
    } else {
	cairo_surface_destroy (surface);
    }
    return o;
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
    cairo_surface_t *surface;
    cairo_format_t format;
    int width, height;

    if (!PyArg_ParseTuple (args, "iii:Surface.create_similar",
			   &format, &width, &height))
	return NULL;

    surface = cairo_surface_create_similar (o->surface, format, width, height);
    if (!surface)
	return PyErr_NoMemory();
    /* bug #2765 - "How do we identify surface types?"
     * should pass surface type as arg2
     */
    return PycairoSurface_FromSurface (surface, NULL, NULL);
}

static PyObject *
surface_finish (PycairoSurface *o)
{
    cairo_status_t status = cairo_surface_finish (o->surface);
    Py_CLEAR(o->base);

    if (Pycairo_Check_Status (status))
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
static PyObject *
surface_write_to_png (PycairoSurface *o, PyObject *args)
{
    const char *filename;
    cairo_status_t status;

    if (!PyArg_ParseTuple (args, "s:Surface.write_to_png", &filename))
	return NULL;

    status = cairo_surface_write_to_png (o->surface, filename);
    if (Pycairo_Check_Status (status))
	return NULL;
    Py_RETURN_NONE;
}
#endif  /* CAIRO_HAS_PNG_FUNCTIONS */


static PyMethodDef surface_methods[] = {
    /* methods never exposed in a language binding:
     * cairo_surface_destroy()
     * cairo_surface_reference()
     * cairo_surface_get_user_data()
     * cairo_surface_set_user_data()
     */
    {"create_similar", (PyCFunction)surface_create_similar,    METH_VARARGS },
    {"finish",         (PyCFunction)surface_finish,            METH_NOARGS },
    {"set_device_offset",(PyCFunction)surface_set_device_offset, 
                                                                METH_VARARGS },
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    {"write_to_png",   (PyCFunction)surface_write_to_png,      METH_VARARGS },
    /* write_to_png_stream */
#endif
    {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoSurface_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    &PyBaseObject_Type,                 /* tp_base */
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
    cairo_surface_t *surface;
    PyObject *o;

    if (!PyArg_ParseTuple (args, "iii:ImageSurface.__new__",
			   &format, &width, &height))
	return NULL;

    o = type->tp_alloc(type, 0);
    if (o) {
	surface = cairo_image_surface_create (format, width, height);
	if (surface) {
	    ((PycairoImageSurface *)o)->surface = surface;
	} else {
	    Py_DECREF(o);
	    return PyErr_NoMemory();
	}
    }
    return o;
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

    if (!surface)
	return PyErr_NoMemory();
    return PycairoSurface_FromSurface(surface, &PycairoImageSurface_Type, 
				      (PyObject *)array);
}
#endif /* HAVE_NUMPY */

#if 0 /* disable until a reference to the buffer is added to the surface */
/* alternative constructor */
static PyObject *
image_surface_create_for_data (PyTypeObject *type, PyObject *args)
{
    cairo_surface_t *surface;
    cairo_format_t format;
    char *data;
    int length, width, height, stride = -1;

    if (!PyArg_ParseTuple(args, "w#iii|i:Surface.create_for_data",
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

    surface = cairo_image_surface_create_for_data
	((unsigned char *)data, format, width, height, stride);
    if (!surface)
	return PyErr_NoMemory();

    return PycairoSurface_FromSurface(surface, &PycairoImageSurface_Type, 
				      NULL);
    /* FIXME: get surface to hold a reference to buffer */
}
#endif

#ifdef CAIRO_HAS_PNG_FUNCTIONS
static PyObject *
image_surface_create_from_png (PyTypeObject *type, PyObject *args)
{
    const char *filename;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "s:Surface.create_from_png", &filename))
	return NULL;

    surface = cairo_image_surface_create_from_png (filename);
    if (!surface) {
	PyErr_SetString(PyExc_ValueError, "invalid PNG file or memory could "
			"not be allocated for operation");
	return NULL;
    }
    return PycairoSurface_FromSurface (surface, &PycairoImageSurface_Type, 
				       NULL);
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


static PyMethodDef image_surface_methods[] = {
#ifdef HAVE_NUMPY
    {"create_for_array",(PyCFunction)image_surface_create_for_array, 
                                                   METH_VARARGS | METH_CLASS },
#endif
#if 0
    {"create_for_data",(PyCFunction)image_surface_create_for_data, 
                                                   METH_VARARGS | METH_CLASS },
#endif
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    {"create_from_png", (PyCFunction)image_surface_create_from_png, 
                                                   METH_VARARGS | METH_CLASS },
    /* create_from_png_stream */
#endif
    {"get_height",    (PyCFunction)image_surface_get_height,      METH_NOARGS},
    {"get_width",     (PyCFunction)image_surface_get_width,       METH_NOARGS},
    {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoImageSurface_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    &PycairoSurface_Type,               /* tp_base */
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
    const char *filename;
    double width_in_points, height_in_points;
    cairo_surface_t *surface;
    PyObject *o;

    if (!PyArg_ParseTuple(args, "sdd:PDFSurface.__new__",
			  &filename, &width_in_points, &height_in_points))
	return NULL;

    o = type->tp_alloc(type, 0);
    if (o) {
	surface = cairo_pdf_surface_create (filename, width_in_points, 
					    height_in_points);
	if (surface) {
	    ((PycairoPDFSurface *)o)->surface = surface;
	} else {
	    Py_DECREF(o);
	    return PyErr_NoMemory();
	}
    }
    return o;
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

static PyMethodDef pdfsurface_methods[] = {
    {"set_dpi", (PyCFunction)pdf_surface_set_dpi,    METH_VARARGS },
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoPDFSurface_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    pdfsurface_methods,                 /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    &PycairoSurface_Type,               /* tp_base */
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
    const char *filename;
    double width_in_points, height_in_points;
    cairo_surface_t *surface;
    PyObject *o;

    if (!PyArg_ParseTuple(args, "sdd:PSSurface.__new__",
			  &filename, &width_in_points, &height_in_points))
	return NULL;

    o = type->tp_alloc(type, 0);
    if (o) {
	surface = cairo_ps_surface_create (filename, width_in_points, 
					   height_in_points);
	if (surface) {
	    ((PycairoPSSurface *)o)->surface = surface;
	} else {
	    Py_DECREF(o);
	    return PyErr_NoMemory();
	}
    }
    return o;
}
/* declared in cairo-ps.h but function does not exist!
static PyObject *
ps_surface_set_dpi (PycairoPSSurface *o, PyObject *args)
{
    double x_dpi, y_dpi;

    if (!PyArg_ParseTuple(args, "dd:PSSurface.set_dpi", &x_dpi, &y_dpi))
	return NULL;
    cairo_ps_surface_set_dpi (o->surface, x_dpi, y_dpi);
    Py_RETURN_NONE;
}
*/
static PyMethodDef pssurface_methods[] = {
    /*{"set_dpi", (PyCFunction)ps_surface_set_dpi,    METH_VARARGS },*/
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoPSSurface_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    pssurface_methods,                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    &PycairoSurface_Type,               /* tp_base */
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


/* Class Win32Surface(Surface) -------------------------------------------- */
#if CAIRO_HAS_WIN32_SURFACE
#include <cairo-win32.h>

static PyObject *
win32_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    int hdc;
    cairo_surface_t *surface;
    PyObject *o;

    if (!PyArg_ParseTuple(args, "i:Win32Surface.__new__",  &hdc))
	return NULL;

    o = type->tp_alloc(type, 0);
    if (o) {
	surface = cairo_win32_surface_create ((HDC)hdc);
	if (surface) {
	    ((PycairoPSSurface *)o)->surface = surface;
	} else {
	    Py_DECREF(o);
	    return PyErr_NoMemory();
	}
    }
    return o;
}

static PyMethodDef win32surface_methods[] = {
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoWin32Surface_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    win32surface_methods,               /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    &PycairoSurface_Type,               /* tp_base */
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
