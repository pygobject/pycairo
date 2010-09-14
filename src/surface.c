/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2003 James Henstridge, Steven Chaplin
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

#include "config.h"
#include "private.h"


/* Class Surface ---------------------------------------------------------- */

/* PycairoSurface_FromSurface
 * Create a new
 *   PycairoImageSurface,
 *   PycairoPDFSurface,
 *   PycairoPSSurface,
 *   PycairoSVGSurface,
 *   PycairoWin32Surface,
 *   PycairoWin32PrintingSurface,
 *   PycairoXCBSurface, or
 *   PycairoXlibSurface
 * from a cairo_surface_t.
 * surface - a cairo_surface_t to 'wrap' into a Python object.
 *   It is unreferenced if the PycairoSurface creation fails, or if the
 *   cairo_surface_t has an error status.
 * base - the base object used to create the surface, or NULL.
 *   It is referenced to keep it alive while the cairo_surface_t is being used.
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoSurface_FromSurface (cairo_surface_t *surface, PyObject *base) {
  PyTypeObject *type = NULL;
  PyObject *o;

  assert (surface != NULL);

  if (Pycairo_Check_Status (cairo_surface_status (surface))) {
    cairo_surface_destroy (surface);
    return NULL;
  }

  switch (cairo_surface_get_type (surface)) {
#if CAIRO_HAS_IMAGE_SURFACE
  case CAIRO_SURFACE_TYPE_IMAGE:
    type = &PycairoImageSurface_Type;
    break;
#endif
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
  case CAIRO_SURFACE_TYPE_WIN32_PRINTING:
    type = &PycairoWin32PrintingSurface_Type;
    break;
#endif
#if CAIRO_HAS_XCB_SURFACE
  case CAIRO_SURFACE_TYPE_XCB:
    type = &PycairoXCBSurface_Type;
    break;
#endif
#if CAIRO_HAS_XLIB_SURFACE
  case CAIRO_SURFACE_TYPE_XLIB:
    type = &PycairoXlibSurface_Type;
    break;
#endif
  default:
    type = &PycairoSurface_Type;
    break;
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
_write_func (void *closure, const unsigned char *data, unsigned int length) {
  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject *res = PyObject_CallMethod ((PyObject *)closure, "write", "(s#)",
				       data, (Py_ssize_t)length);
  if (res == NULL) {
    /* an exception has occurred, it will be picked up later by
     * Pycairo_Check_Status()
     */
    PyGILState_Release(gstate);
    return CAIRO_STATUS_WRITE_ERROR;
  }
  Py_DECREF(res);
  PyGILState_Release(gstate);
  return CAIRO_STATUS_SUCCESS;
}

static void
surface_dealloc (PycairoSurface *o) {
  if (o->surface) {
    cairo_surface_destroy(o->surface);
    o->surface = NULL;
  }
  Py_CLEAR(o->base);

  o->ob_type->tp_free((PyObject *)o);
}

static PyObject *
surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PyErr_SetString(PyExc_TypeError,
		  "The Surface type cannot be instantiated");
  return NULL;
}

static PyObject *
surface_copy_page (PycairoSurface *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_surface_copy_page (o->surface);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
surface_create_similar (PycairoSurface *o, PyObject *args) {
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
surface_finish (PycairoSurface *o) {
  cairo_surface_finish (o->surface);
  Py_CLEAR(o->base);
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
surface_flush (PycairoSurface *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_surface_flush (o->surface);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
surface_get_content (PycairoSurface *o) {
  return PyInt_FromLong (cairo_surface_get_content (o->surface));
}

static PyObject *
surface_get_device_offset (PycairoSurface *o) {
  double x_offset, y_offset;
  cairo_surface_get_device_offset (o->surface, &x_offset, &y_offset);
  return Py_BuildValue("(dd)", x_offset, y_offset);
}

static PyObject *
surface_get_fallback_resolution (PycairoSurface *o) {
  double x_ppi, y_ppi;
  cairo_surface_get_fallback_resolution (o->surface, &x_ppi, &y_ppi);
  return Py_BuildValue("(dd)", x_ppi, y_ppi);
}

static PyObject *
surface_get_font_options (PycairoSurface *o) {
  cairo_font_options_t *options = cairo_font_options_create();

  cairo_surface_get_font_options (o->surface, options);
  /* there is no reference fn */
  return PycairoFontOptions_FromFontOptions (options);
}

static PyObject *
surface_mark_dirty (PycairoSurface *o) {
  cairo_surface_mark_dirty (o->surface);
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
surface_mark_dirty_rectangle (PycairoSurface *o, PyObject *args) {
  int x, y, width, height;

  if (!PyArg_ParseTuple(args, "iiii:Surface.mark_dirty_rectangle",
			&x, &y, &width, &height))
    return NULL;

  cairo_surface_mark_dirty_rectangle (o->surface, x, y, width, height);
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
surface_set_device_offset (PycairoSurface *o, PyObject *args) {
  double x_offset, y_offset;

  if (!PyArg_ParseTuple (args, "dd:Surface.set_device_offset",
			 &x_offset, &y_offset))
    return NULL;

  cairo_surface_set_device_offset (o->surface, x_offset, y_offset);
  Py_RETURN_NONE;
}

static PyObject *
surface_set_fallback_resolution (PycairoSurface *o, PyObject *args) {
  double x_ppi, y_ppi;

  if (!PyArg_ParseTuple(args, "dd:Surface.set_fallback_resolution",
			&x_ppi, &y_ppi))
    return NULL;
  cairo_surface_set_fallback_resolution (o->surface, x_ppi, y_ppi);
  Py_RETURN_NONE;
}

static PyObject *
surface_show_page (PycairoSurface *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_surface_show_page (o->surface);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
static PyObject *
surface_write_to_png (PycairoSurface *o, PyObject *args) {
  cairo_status_t status;
  PyObject *file;

  if (!PyArg_ParseTuple(args, "O:Surface.write_to_png", &file))
    return NULL;

  if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
    /* filename (str or unicode) argument */
    char *name = NULL;  // the encoded filename

    if (!PyArg_ParseTuple(args, "et:Surface.write_to_png",
			  Py_FileSystemDefaultEncoding, &name))
      return NULL;

    Py_BEGIN_ALLOW_THREADS;
    status = cairo_surface_write_to_png (o->surface, name);
    Py_END_ALLOW_THREADS;

    PyMem_Free(name);

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
    Py_BEGIN_ALLOW_THREADS;
    status = cairo_surface_write_to_png_stream (o->surface, _write_func,
						file);
    Py_END_ALLOW_THREADS;
  }
  RETURN_NULL_IF_CAIRO_ERROR(status);
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
  {"copy_page",      (PyCFunction)surface_copy_page,          METH_NOARGS},
  {"create_similar", (PyCFunction)surface_create_similar,     METH_VARARGS},
  {"finish",         (PyCFunction)surface_finish,             METH_NOARGS},
  {"flush",          (PyCFunction)surface_flush,              METH_NOARGS},
  {"get_content",    (PyCFunction)surface_get_content,        METH_NOARGS},
  {"get_device_offset",(PyCFunction)surface_get_device_offset,METH_NOARGS},
  {"get_fallback_resolution",(PyCFunction)surface_get_fallback_resolution,
   METH_NOARGS},
  {"get_font_options",(PyCFunction)surface_get_font_options,  METH_NOARGS},
  {"mark_dirty",     (PyCFunction)surface_mark_dirty,         METH_NOARGS},
  {"mark_dirty_rectangle", (PyCFunction)surface_mark_dirty_rectangle,
   METH_VARARGS},
  {"set_device_offset",(PyCFunction)surface_set_device_offset,METH_VARARGS},
  {"set_fallback_resolution",(PyCFunction)surface_set_fallback_resolution,
   METH_VARARGS},
  {"show_page",      (PyCFunction)surface_show_page,          METH_NOARGS},
#ifdef CAIRO_HAS_PNG_FUNCTIONS
  {"write_to_png",   (PyCFunction)surface_write_to_png,       METH_VARARGS},
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
  0,                                  /* tp_base */
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
#ifdef CAIRO_HAS_IMAGE_SURFACE

static PyObject *
image_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  cairo_format_t format;
  int width, height;

  if (!PyArg_ParseTuple (args, "iii:ImageSurface.__new__",
			 &format, &width, &height))
    return NULL;
  return PycairoSurface_FromSurface (
	     cairo_image_surface_create (format, width, height),
	     NULL);
}

/* METH_CLASS */
static PyObject *
image_surface_create_for_data (PyTypeObject *type, PyObject *args) {
  cairo_surface_t *surface;
  cairo_format_t format;
  unsigned char *buffer;
  int width, height, stride = -1, res;
  Py_ssize_t buffer_len;
  PyObject *obj;

  if (!PyArg_ParseTuple(args, "Oiii|i:ImageSurface.create_for_data",
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
    stride = cairo_format_stride_for_width (format, width);
    if (stride == -1){
      PyErr_SetString(CairoError,
		      "format is invalid or the width too large");
      return NULL;
    }
  }
  if (height * stride > buffer_len) {
    PyErr_SetString(PyExc_TypeError, "buffer is not long enough");
    return NULL;
  }
  Py_BEGIN_ALLOW_THREADS;
  surface = cairo_image_surface_create_for_data (buffer, format, width,
						 height, stride);
  Py_END_ALLOW_THREADS;
  return PycairoSurface_FromSurface(surface, obj);
}


#ifdef CAIRO_HAS_PNG_FUNCTIONS
static cairo_status_t
_read_func (void *closure, unsigned char *data, unsigned int length) {
  char *buffer;
  Py_ssize_t str_length;
  cairo_status_t status = CAIRO_STATUS_READ_ERROR;
  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject *pystr = PyObject_CallMethod ((PyObject *)closure, "read", "(i)",
					 length);
  if (pystr == NULL) {
    /* an exception has occurred, it will be picked up later by
     * Pycairo_Check_Status()
     */
    goto end;
  }
  int ret = PyString_AsStringAndSize(pystr, &buffer, &str_length);
  if (ret == -1 || str_length < length) {
    goto end;
  }
  /* don't use strncpy() since png data may contain NUL bytes */
  memcpy (data, buffer, str_length);
  status = CAIRO_STATUS_SUCCESS;
 end:
  Py_XDECREF(pystr);
  PyGILState_Release(gstate);
  return status;
}

/* METH_CLASS */
static PyObject *
image_surface_create_from_png (PyTypeObject *type, PyObject *args) {
  cairo_surface_t *is;
  PyObject *reader, *file;

  if (!PyArg_ParseTuple(args, "O:ImageSurface.create_from_png", &file))
    return NULL;

  if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
    char *name = NULL;  // the encoded filename

    if (!PyArg_ParseTuple(args, "et:Surface.create_from_png",
			  Py_FileSystemDefaultEncoding, &name))
      return NULL;

    Py_BEGIN_ALLOW_THREADS;
    is = cairo_image_surface_create_from_png (name);
    Py_END_ALLOW_THREADS;

    PyMem_Free(name);
    return PycairoSurface_FromSurface (is, NULL);
  }

  /* file or file-like object argument */
  reader = PyObject_GetAttrString (file, "read");
  if (reader == NULL || !PyCallable_Check (reader)) {
    Py_XDECREF(reader);
    PyErr_SetString(PyExc_TypeError,
"ImageSurface.create_from_png argument must be a filename (str), file object, "
"or an object that has a \"read\" method (like StringIO)");
    return NULL;
  }
  Py_DECREF(reader);

  Py_BEGIN_ALLOW_THREADS;
  is = cairo_image_surface_create_from_png_stream (_read_func, file);
  Py_END_ALLOW_THREADS;
  return PycairoSurface_FromSurface (is, NULL);
}
#endif /* CAIRO_HAS_PNG_FUNCTIONS */

/* METH_STATIC */
static PyObject *
image_surface_format_stride_for_width (PyObject *self, PyObject *args) {
  cairo_format_t format;
  int width;
  if (!PyArg_ParseTuple(args, "ii:format_stride_for_width", &format, &width))
    return NULL;
  return PyInt_FromLong (cairo_format_stride_for_width (format, width));
}

static PyObject *
image_surface_get_data (PycairoImageSurface *o) {
  return PyBuffer_FromReadWriteObject((PyObject *)o, 0, Py_END_OF_BUFFER);
}

static PyObject *
image_surface_get_format (PycairoImageSurface *o) {
  return PyInt_FromLong (cairo_image_surface_get_format (o->surface));
}

static PyObject *
image_surface_get_height (PycairoImageSurface *o) {
  return PyInt_FromLong (cairo_image_surface_get_height (o->surface));
}

static PyObject *
image_surface_get_stride (PycairoImageSurface *o) {
  return PyInt_FromLong (cairo_image_surface_get_stride (o->surface));
}

static PyObject *
image_surface_get_width (PycairoImageSurface *o) {
  return PyInt_FromLong (cairo_image_surface_get_width (o->surface));
}


/* Buffer interface functions, used by ImageSurface.get_data() */
static int
image_surface_buffer_getreadbuf (PycairoImageSurface *o, int segment,
				 const void **ptr) {
  cairo_surface_t *surface = o->surface;
  int height, stride;

  if (segment != 0) {
    PyErr_SetString(PyExc_SystemError,
		    "accessing non-existent ImageSurface segment");
    return -1;
  }
  height = cairo_image_surface_get_height (surface);
  stride = cairo_image_surface_get_stride (surface);
  *ptr = (void *) cairo_image_surface_get_data (surface);
  return height * stride;
}

static int
image_surface_buffer_getwritebuf (PycairoImageSurface *o, int segment,
				  const void **ptr) {
  cairo_surface_t *surface = o->surface;
  int height, stride;

  if (segment != 0) {
    PyErr_SetString(PyExc_SystemError,
		    "accessing non-existent ImageSurface segment");
    return -1;
  }
  height = cairo_image_surface_get_height (surface);
  stride = cairo_image_surface_get_stride (surface);
  *ptr = (void *) cairo_image_surface_get_data (surface);
  return height * stride;
}

static int
image_surface_buffer_getsegcount (PycairoImageSurface *o, int *lenp) {
  if (lenp) {
    /* report the sum of the sizes (in bytes) of all segments */
    cairo_surface_t *surface = o->surface;
    int height = cairo_image_surface_get_height (surface);
    int stride = cairo_image_surface_get_stride (surface);
    *lenp = height * stride;
  }
  return 1;  /* surface data is all in one segment */
}

/* See Python C API Manual 10.7 */
static PyBufferProcs image_surface_as_buffer = {
  (readbufferproc) image_surface_buffer_getreadbuf,
  (writebufferproc)image_surface_buffer_getwritebuf,
  (segcountproc)   image_surface_buffer_getsegcount,
  (charbufferproc) NULL,
};

static PyMethodDef image_surface_methods[] = {
  {"create_for_data",(PyCFunction)image_surface_create_for_data,
   METH_VARARGS | METH_CLASS},
#ifdef CAIRO_HAS_PNG_FUNCTIONS
  {"create_from_png", (PyCFunction)image_surface_create_from_png,
   METH_VARARGS | METH_CLASS},
#endif
  {"format_stride_for_width",
   (PyCFunction)image_surface_format_stride_for_width,
   METH_VARARGS | METH_STATIC},
  {"get_data",      (PyCFunction)image_surface_get_data,        METH_NOARGS},
  {"get_format",    (PyCFunction)image_surface_get_format,      METH_NOARGS},
  {"get_height",    (PyCFunction)image_surface_get_height,      METH_NOARGS},
  {"get_stride",    (PyCFunction)image_surface_get_stride,      METH_NOARGS},
  {"get_width",     (PyCFunction)image_surface_get_width,       METH_NOARGS},
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
  &image_surface_as_buffer,		/* tp_as_buffer */
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
#endif /* CAIRO_HAS_IMAGE_SURFACE */


/* Class PDFSurface(Surface) ---------------------------------------------- */
#ifdef CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>

static PyObject *
pdf_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  double width_in_points, height_in_points;
  PyObject *file, *writer;
  cairo_surface_t *sfc;

  if (!PyArg_ParseTuple(args, "Odd:PDFSurface.__new__",
			&file, &width_in_points, &height_in_points))
    return NULL;

  if (file == Py_None) {
    Py_BEGIN_ALLOW_THREADS;
    sfc = cairo_pdf_surface_create (NULL, width_in_points, height_in_points);
    Py_END_ALLOW_THREADS;
    return PycairoSurface_FromSurface (sfc, NULL);

  }else if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
    /* filename (str or unicode) argument */
    char *name = NULL;  // the encoded filename

    if (!PyArg_ParseTuple(args, "etdd:PDFSurface.__new__",
			  Py_FileSystemDefaultEncoding,
			  &name, &width_in_points, &height_in_points))
      return NULL;

    Py_BEGIN_ALLOW_THREADS;
    sfc = cairo_pdf_surface_create (name, width_in_points, height_in_points);
    Py_END_ALLOW_THREADS;

    PyMem_Free(name);
    return PycairoSurface_FromSurface (sfc, NULL);
  }

  /* file or file-like object argument */
  writer = PyObject_GetAttrString (file, "write");
  if (writer == NULL || !PyCallable_Check (writer)) {
    Py_XDECREF(writer);
    PyErr_SetString(PyExc_TypeError,
"PDFSurface argument 1 must be\n"
"  None, or\n"
"  a filename (str), or\n"
"  a file object, or\n"
"  an object that has a \"write\" method (like StringIO)."
		    );
    return NULL;
  }
  Py_DECREF(writer);

  Py_BEGIN_ALLOW_THREADS;
  sfc = cairo_pdf_surface_create_for_stream (_write_func, file,
					     width_in_points, height_in_points);
  Py_END_ALLOW_THREADS;
  return PycairoSurface_FromSurface (sfc, file);
}

static PyObject *
pdf_surface_set_size (PycairoPDFSurface *o, PyObject *args) {
  double width_in_points, height_in_points;

  if (!PyArg_ParseTuple(args, "dd:PDFSurface.set_size", &width_in_points,
			&height_in_points))
    return NULL;
  cairo_pdf_surface_set_size (o->surface, width_in_points,
			      height_in_points);
  Py_RETURN_NONE;
}

static PyMethodDef pdf_surface_methods[] = {
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
ps_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  double width_in_points, height_in_points;
  PyObject *file, *writer;
  cairo_surface_t *sfc;

  if (!PyArg_ParseTuple(args, "Odd:PSSurface.__new__",
			&file, &width_in_points, &height_in_points))
    return NULL;

  if (file == Py_None) {
    Py_BEGIN_ALLOW_THREADS;
    sfc = cairo_ps_surface_create (NULL, width_in_points, height_in_points);
    Py_END_ALLOW_THREADS;
    return PycairoSurface_FromSurface (sfc, NULL);

  }else if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
    /* filename (str or unicode) argument */
    char *name = NULL;  // the encoded filename

    if (!PyArg_ParseTuple(args, "etdd:PSSurface.__new__",
			  Py_FileSystemDefaultEncoding,
			  &name, &width_in_points, &height_in_points))
      return NULL;

    Py_BEGIN_ALLOW_THREADS;
    sfc = cairo_ps_surface_create (name, width_in_points, height_in_points);
    Py_END_ALLOW_THREADS;

    PyMem_Free(name);
    return PycairoSurface_FromSurface (sfc, NULL);
  }
  /* else: file or file-like object argument */
  writer = PyObject_GetAttrString (file, "write");
  if (writer == NULL || !PyCallable_Check (writer)) {
    Py_XDECREF(writer);
    PyErr_SetString(PyExc_TypeError,
"PSSurface argument 1 must be\n"
"  None, or\n"
"  a filename (str), or\n"
"  a file object, or\n"
"  an object that has a \"write\" method (like StringIO)."
		    );
    return NULL;
  }
  Py_DECREF(writer);

  Py_BEGIN_ALLOW_THREADS;
  sfc = cairo_ps_surface_create_for_stream (_write_func, file,
					    width_in_points, height_in_points);
  Py_END_ALLOW_THREADS;
  return PycairoSurface_FromSurface (sfc, file);
}

static PyObject *
ps_surface_dsc_begin_page_setup (PycairoPSSurface *o) {
  cairo_ps_surface_dsc_begin_page_setup (o->surface);
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
ps_surface_dsc_begin_setup (PycairoPSSurface *o) {
  cairo_ps_surface_dsc_begin_setup (o->surface);
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
ps_surface_dsc_comment (PycairoPSSurface *o, PyObject *args) {
  const char *comment;
  if (!PyArg_ParseTuple(args, "s:PSSurface.dsc_comment", &comment))
    return NULL;
  cairo_ps_surface_dsc_comment (o->surface, comment);
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
ps_surface_get_eps (PycairoPSSurface *o) {
  PyObject *eps = cairo_ps_surface_get_eps (o->surface) ? Py_True : Py_False;
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_INCREF(eps);
  return eps;
}

/* METH_STATIC */
static PyObject *
ps_surface_ps_level_to_string (PyObject *self, PyObject *args) {
  int level;
  if (!PyArg_ParseTuple(args, "i:ps_level_to_string", &level))
    return NULL;
  const char *s = cairo_ps_level_to_string (level);
  if (s == NULL){
    PyErr_SetString(CairoError, "ps_level_to_string: "
		    "invalid level argument");
    return NULL;
  }
  return PyString_FromString(s);
}

static PyObject *
ps_surface_restrict_to_level (PycairoPSSurface *o, PyObject *args) {
  int level;

  if (!PyArg_ParseTuple(args, "i:PSSurface.restrict_to_level", &level))
    return NULL;
  cairo_ps_surface_restrict_to_level (o->surface, level);
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
ps_surface_set_eps (PycairoPSSurface *o, PyObject *args) {
  PyObject *py_eps;
  if (!PyArg_ParseTuple(args, "O!:PSSurface.set_eps",
			&PyBool_Type, &py_eps))
    return NULL;
  cairo_ps_surface_set_eps (o->surface, (py_eps == Py_True));
  RETURN_NULL_IF_CAIRO_SURFACE_ERROR(o->surface);
  Py_RETURN_NONE;
}

static PyObject *
ps_surface_set_size (PycairoPSSurface *o, PyObject *args) {
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
  {"get_eps", (PyCFunction)ps_surface_get_eps,                 METH_NOARGS },
  /* ps_get_levels - not implemented yet*/
  {"ps_level_to_string", (PyCFunction)ps_surface_ps_level_to_string,
   METH_VARARGS | METH_STATIC},
  {"restrict_to_level", (PyCFunction)ps_surface_restrict_to_level,
   METH_VARARGS },
  {"set_eps", (PyCFunction)ps_surface_set_eps,                METH_VARARGS },
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


/* Class SVGSurface(Surface) ----------------------------------------------- */
#ifdef CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>

static PyObject *
svg_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  double width_in_points, height_in_points;
  PyObject *file, *writer;
  cairo_surface_t *sfc;

  if (!PyArg_ParseTuple(args, "Odd:SVGSurface.__new__",
			&file, &width_in_points, &height_in_points))
    return NULL;

  if (file == Py_None) {
    Py_BEGIN_ALLOW_THREADS;
    sfc = cairo_svg_surface_create (NULL, width_in_points, height_in_points);
    Py_END_ALLOW_THREADS;
    return PycairoSurface_FromSurface (sfc, NULL);

  }else if (PyObject_TypeCheck (file, &PyBaseString_Type)) {
    /* filename (str or unicode) argument */
    char *name = NULL;  // the encoded filename

    if (!PyArg_ParseTuple(args, "etdd:SVGSurface.__new__",
			  Py_FileSystemDefaultEncoding,
			  &name, &width_in_points, &height_in_points))
      return NULL;

    Py_BEGIN_ALLOW_THREADS;
    sfc = cairo_svg_surface_create (name, width_in_points, height_in_points);
    Py_END_ALLOW_THREADS;

    PyMem_Free(name);
    return PycairoSurface_FromSurface (sfc, NULL);
  }
  /* else: file or file-like object argument */
  writer = PyObject_GetAttrString (file, "write");
  if (writer == NULL || !PyCallable_Check (writer)) {
    Py_XDECREF(writer);
    PyErr_SetString(PyExc_TypeError,
"SVGSurface argument 1 must be\n"
"  None, or\n"
"  a filename (str), or\n"
"  a file object, or\n"
"  an object that has a \"write\" method (like StringIO)."
		    );
    return NULL;
  }
  Py_DECREF(writer);

  Py_BEGIN_ALLOW_THREADS;
  sfc = cairo_svg_surface_create_for_stream (_write_func, file,
					     width_in_points, height_in_points);
  Py_END_ALLOW_THREADS;
  return PycairoSurface_FromSurface (sfc, file);
}

static PyMethodDef svg_surface_methods[] = {
  /* TODO
   * cairo_svg_surface_restrict_to_version
   * cairo_svg_get_versions
   * cairo_svg_version_to_string
   */
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
  &PycairoSurface_Type,               /* tp_base */
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


#if CAIRO_HAS_WIN32_SURFACE
#include <cairo-win32.h>

/* Class Win32Surface(Surface) -------------------------------------------- */
static PyObject *
win32_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
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


/* Class Win32PrintingSurface(Surface) ------------------------------------ */
static PyObject *
win32_printing_surface_new (PyTypeObject *type, PyObject *args,
			    PyObject *kwds) {
  int hdc;

  if (!PyArg_ParseTuple(args, "i:Win32PrintingSurface.__new__", &hdc))
    return NULL;
  return PycairoSurface_FromSurface (
		     cairo_win32_printing_surface_create ((HDC)hdc), NULL);
}

static PyMethodDef win32_printing_surface_methods[] = {
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoWin32PrintingSurface_Type = {
  PyObject_HEAD_INIT(NULL)
  0,                                  /* ob_size */
  "cairo.Win32PrintingSurface",       /* tp_name */
  sizeof(PycairoWin32PrintingSurface), /* tp_basicsize */
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
  win32_printing_surface_methods,     /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoSurface_Type,               /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)win32_printing_surface_new,/* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};
#endif  /* CAIRO_HAS_WIN32_SURFACE */


/* Class XCBSurface(Surface) --------------------------------------------- */
#ifdef CAIRO_HAS_XCB_SURFACE
#include <cairo-xcb.h>

#ifdef HAVE_XPYB
/** Convert a Python object from xpyb to a C struct matching libxcb type.
 * The object must be referenced if you want to keep returned data away from
 * garbage collection.
 * @param obj The object to convert.
 * @param len The size of the object read by Python.
 * @return A pointer to that data.
 */
const void *
xpyb2struct(PyObject *obj, Py_ssize_t *len)
{
    const void *data;

    if (PyObject_AsReadBuffer(obj, &data, len) < 0)
        return NULL;

    return data;
}

static int
have_xpyb(void)
{
  static int have_xpyb = -1;
  if(have_xpyb == -1) {
      /* Get type from xpyb */
      xpyb_IMPORT;
      /* Some types are not defined in the CAPI */
      PyObject *xpyb_module = PyImport_ImportModule("xcb.xproto");
      if (xpyb_module) {
          PyObject *dict = PyModule_GetDict(xpyb_module);
          xpybVISUALTYPE_type = PyDict_GetItemString(dict, "VISUALTYPE");
          Py_DECREF(xpyb_module);
          have_xpyb = 1;
      }
      else
          have_xpyb = 0;
  }
  return have_xpyb;
}

#endif

static PyObject *
xcb_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
#ifdef HAVE_XPYB
  int width, height;
  xcb_drawable_t drawable;
  PyObject *visual;
  xpybConn *conn;

  if(!have_xpyb())
      return NULL;

  if (!PyArg_ParseTuple(args, "O!IO!ii:XCBSurface.__new__",
                        xpyb_CAPI->xpybConn_type, &conn,
                        &drawable,
                        xpybVISUALTYPE_type, &visual,
                        &width, &height))
    return NULL;

  /* Convert Python object VISUALTYPE to a xcb_visualtype_t */
  Py_ssize_t length;
  xcb_visualtype_t *visualtype = (xcb_visualtype_t *) xpyb2struct(visual, &length);

  if (length < sizeof(xcb_visualtype_t))
      return NULL;

  return PycairoSurface_FromSurface (
		     cairo_xcb_surface_create (conn->conn, drawable, visualtype,
                                               width, height), NULL);
#else
  PyErr_SetString(PyExc_TypeError,
                  "pycairo was not compiled with xpyb support");
  return NULL;
#endif
}

#ifdef HAVE_XPYB
static PyObject *
xcb_surface_set_size (PycairoXCBSurface *o, PyObject *args) {
  int width, height;

  if (!PyArg_ParseTuple(args, "ii:XCBSurface.set_size", &width, &height))
    return NULL;
  cairo_xcb_surface_set_size (o->surface, width, height);
  Py_RETURN_NONE;
}
#endif

static PyMethodDef xcb_surface_methods[] = {
#ifdef HAVE_XPYB
  {"set_size", (PyCFunction)xcb_surface_set_size,    METH_VARARGS },
#endif
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoXCBSurface_Type = {
  PyObject_HEAD_INIT(NULL)
  0,                                  /* ob_size */
  "cairo.XCBSurface",                 /* tp_name */
  sizeof(PycairoXCBSurface),          /* tp_basicsize */
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
  xcb_surface_methods,                /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoSurface_Type,               /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)xcb_surface_new,           /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};
#endif  /* CAIRO_HAS_XCB_SURFACE */


/* Class XlibSurface(Surface) --------------------------------------------- */
#ifdef CAIRO_HAS_XLIB_SURFACE
#include <cairo-xlib.h>

static PyObject *
xlib_surface_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PyErr_SetString(PyExc_TypeError,
		  "The XlibSurface type cannot be directly instantiated");
  return NULL;
}

static PyObject *
xlib_surface_get_depth (PycairoXlibSurface *o) {
  return PyInt_FromLong (cairo_xlib_surface_get_depth (o->surface));
}

static PyObject *
xlib_surface_get_height (PycairoXlibSurface *o) {
  return PyInt_FromLong (cairo_xlib_surface_get_height (o->surface));
}

static PyObject *
xlib_surface_get_width (PycairoXlibSurface *o) {
  return PyInt_FromLong (cairo_xlib_surface_get_width (o->surface));
}

static PyMethodDef xlib_surface_methods[] = {
  {"get_depth", (PyCFunction)xlib_surface_get_depth,    METH_NOARGS },
  {"get_height",(PyCFunction)xlib_surface_get_height,   METH_NOARGS },
  {"get_width", (PyCFunction)xlib_surface_get_width,    METH_NOARGS },
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
  &PycairoSurface_Type,               /* tp_base */
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
