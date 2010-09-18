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


/* class cairo.FontFace --------------------------------------------------- */

/* PycairoFontFace_FromFontFace
 * Create a new PycairoFontFace from a cairo_font_face_t
 * font_face - a cairo_font_face_t to 'wrap' into a Python object.
 *             it is unreferenced if the PycairoFontFace creation fails
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoFontFace_FromFontFace (cairo_font_face_t *font_face) {
  PyTypeObject *type = NULL;
  PyObject *o;

  assert (font_face != NULL);

  if (Pycairo_Check_Status (cairo_font_face_status (font_face))) {
    cairo_font_face_destroy (font_face);
    return NULL;
  }

  switch (cairo_font_face_get_type (font_face)) {
  case CAIRO_FONT_TYPE_TOY:
    type = &PycairoToyFontFace_Type;
    break;
  default:
    type = &PycairoFontFace_Type;
    break;
  }
  o = type->tp_alloc (type, 0);
  if (o == NULL)
    cairo_font_face_destroy (font_face);
  else
    ((PycairoFontFace *)o)->font_face = font_face;
  return o;
}

static void
font_face_dealloc (PycairoFontFace *o) {
  if (o->font_face) {
    cairo_font_face_destroy (o->font_face);
    o->font_face = NULL;
  }
  o->ob_type->tp_free((PyObject *) o);
}

static PyObject *
font_face_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PyErr_SetString (PyExc_TypeError, "The FontFace type cannot be "
		   "instantiated directly, use Context.get_font_face()");
  return NULL;
}

/*
static PyMethodDef font_face_methods[] = {
     * methods never exposed in a language binding:
     * cairo_font_face_destroy()
     * cairo_font_face_get_user_data()
     * cairo_font_face_get_type()
     * cairo_font_face_reference()
     * cairo_font_face_set_user_data(),
    {NULL, NULL, 0, NULL},
};
*/

PyTypeObject PycairoFontFace_Type = {
  PyObject_HEAD_INIT(NULL)
  0,                                  /* ob_size */
  "cairo.FontFace",                   /* tp_name */
  sizeof(PycairoFontFace),            /* tp_basicsize */
  0,                                  /* tp_itemsize */
  (destructor)font_face_dealloc,      /* tp_dealloc */
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
  0,                                  /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  0,                                  /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)font_face_new,             /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};


/* class cairo.ToyFontFace ------------------------------------------------- */

static PyObject *
toy_font_face_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  const char *utf8;
  cairo_font_slant_t slant   = CAIRO_FONT_SLANT_NORMAL;
  cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;

  if (!PyArg_ParseTuple (args, "et|ii:ToyFontFace.__new__",
			 "utf-8", &utf8, &slant, &weight))
    return NULL;

  PyObject *o = PycairoFontFace_FromFontFace (
		     cairo_toy_font_face_create (utf8, slant, weight));
  PyMem_Free((void *)utf8);
  return o;
}

static PyObject *
toy_font_get_family (PycairoToyFontFace *o) {
  return PyString_FromString (cairo_toy_font_face_get_family (o->font_face));
}

static PyObject *
toy_font_get_slant (PycairoToyFontFace *o) {
  return PyInt_FromLong (cairo_toy_font_face_get_slant (o->font_face));
}

static PyObject *
toy_font_get_weight (PycairoToyFontFace *o) {
  return PyInt_FromLong (cairo_toy_font_face_get_weight (o->font_face));
}

static PyMethodDef toy_font_face_methods[] = {
  {"get_family",       (PyCFunction)toy_font_get_family, METH_NOARGS},
  {"get_slant",        (PyCFunction)toy_font_get_slant,  METH_NOARGS},
  {"get_weight",       (PyCFunction)toy_font_get_weight, METH_NOARGS},
  {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoToyFontFace_Type = {
  PyObject_HEAD_INIT(NULL)
  0,                                  /* ob_size */
  "cairo.ToyFontFace",                /* tp_name */
  sizeof(PycairoToyFontFace),         /* tp_basicsize */
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
  toy_font_face_methods,              /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoFontFace_Type,              /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)toy_font_face_new,         /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};


/* class cairo.ScaledFont ------------------------------------------------- */

/* PycairoScaledFont_FromScaledFont
 * Create a new PycairoScaledFont from a cairo_scaled_font_t
 * scaled_font - a cairo_scaled_font_t to 'wrap' into a Python object.
 *               it is unreferenced if the PycairoScaledFont creation fails
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoScaledFont_FromScaledFont (cairo_scaled_font_t *scaled_font) {
  PyObject *o;

  assert (scaled_font != NULL);

  if (Pycairo_Check_Status (cairo_scaled_font_status (scaled_font))) {
    cairo_scaled_font_destroy (scaled_font);
    return NULL;
  }

  o = PycairoScaledFont_Type.tp_alloc (&PycairoScaledFont_Type, 0);
  if (o == NULL)
    cairo_scaled_font_destroy (scaled_font);
  else
    ((PycairoScaledFont *)o)->scaled_font = scaled_font;
  return o;
}

static void
scaled_font_dealloc(PycairoScaledFont *o) {
  if (o->scaled_font) {
    cairo_scaled_font_destroy (o->scaled_font);
    o->scaled_font = NULL;
  }
  o->ob_type->tp_free((PyObject *) o);
}

static PyObject *
scaled_font_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PycairoFontFace *ff;
  PycairoFontOptions *fo;
  PycairoMatrix *mx1, *mx2;

  if (!PyArg_ParseTuple(args, "O!O!O!O!:ScaledFont.__new__",
			&PycairoFontFace_Type, &ff,
			&PycairoMatrix_Type, &mx1,
			&PycairoMatrix_Type, &mx2,
			&PycairoFontOptions_Type, &fo))
    return NULL;
  return PycairoScaledFont_FromScaledFont (
               cairo_scaled_font_create (ff->font_face, &mx1->matrix,
					 &mx2->matrix, fo->font_options));
}

static PyObject *
scaled_font_extents (PycairoScaledFont *o) {
  cairo_font_extents_t e;

  cairo_scaled_font_extents (o->scaled_font, &e);
  RETURN_NULL_IF_CAIRO_SCALED_FONT_ERROR(o->scaled_font);
  return Py_BuildValue ("(ddddd)", e.ascent, e.descent, e.height,
			e.max_x_advance, e.max_y_advance);
}

static PyObject *
scaled_font_get_font_face (PycairoScaledFont *o) {
  return PycairoFontFace_FromFontFace (
               cairo_font_face_reference (
		   cairo_scaled_font_get_font_face (o->scaled_font)));
}

static PyObject *
scaled_font_get_scale_matrix (PycairoScaledFont *o) {
  cairo_matrix_t matrix;
  cairo_scaled_font_get_scale_matrix (o->scaled_font, &matrix);
  return PycairoMatrix_FromMatrix (&matrix);
}

static PyObject *
scaled_font_text_extents (PycairoScaledFont *o, PyObject *args) {
  const char *utf8;
  cairo_text_extents_t extents;

  if (!PyArg_ParseTuple (args, "et:ScaledFont.text_extents", "utf-8", &utf8))
    return NULL;

  cairo_scaled_font_text_extents (o->scaled_font, utf8, &extents);
  PyMem_Free((void *)utf8);
  RETURN_NULL_IF_CAIRO_SCALED_FONT_ERROR(o->scaled_font);
  return Py_BuildValue("(dddddd)", extents.x_bearing, extents.y_bearing,
		       extents.width, extents.height, extents.x_advance,
		       extents.y_advance);
}

static PyMethodDef scaled_font_methods[] = {
  /* methods never exposed in a language binding:
   * cairo_scaled_font_destroy()
   * cairo_scaled_font_get_type()
   * cairo_scaled_font_reference()
   *
   * TODO if requested:
   * cairo_scaled_font_get_ctm
   * cairo_scaled_font_get_font_matrix
   * cairo_scaled_font_get_font_options
   * cairo_scaled_font_glyph_extents
   * cairo_scaled_font_text_to_glyphs
   */
  {"extents",       (PyCFunction)scaled_font_extents,        METH_NOARGS},
  {"get_font_face", (PyCFunction)scaled_font_get_font_face,  METH_NOARGS},
  {"get_scale_matrix", (PyCFunction)scaled_font_get_scale_matrix, METH_VARARGS},
  {"text_extents",  (PyCFunction)scaled_font_text_extents,   METH_VARARGS},
  {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoScaledFont_Type = {
  PyObject_HEAD_INIT(NULL)
  0,                                  /* ob_size */
  "cairo.ScaledFont",                 /* tp_name */
  sizeof(PycairoScaledFont),          /* tp_basicsize */
  0,                                  /* tp_itemsize */
  (destructor)scaled_font_dealloc,    /* tp_dealloc */
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
  scaled_font_methods,                /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  0,                                  /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)scaled_font_new,           /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};


/* class cairo.FontOptions ------------------------------------------------ */

/* PycairoFontOptions_FromFontOptions
 * Create a new PycairoFontOptions from a cairo_font_options_t
 * font_options - a cairo_font_options_t to 'wrap' into a Python object.
 *                it is unreferenced if the PycairoFontOptions creation fails
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoFontOptions_FromFontOptions (cairo_font_options_t *font_options) {
  PyObject *o;

  assert (font_options != NULL);

  if (Pycairo_Check_Status (cairo_font_options_status (font_options))) {
    cairo_font_options_destroy (font_options);
    return NULL;
  }

  o = PycairoFontOptions_Type.tp_alloc (&PycairoFontOptions_Type, 0);
  if (o == NULL)
    cairo_font_options_destroy (font_options);
  else
    ((PycairoFontOptions *)o)->font_options = font_options;
  return o;
}

static void
font_options_dealloc(PycairoFontOptions *o) {
  if (o->font_options) {
    cairo_font_options_destroy (o->font_options);
    o->font_options = NULL;
  }
  o->ob_type->tp_free((PyObject *) o);
}

static PyObject *
font_options_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  return PycairoFontOptions_FromFontOptions (cairo_font_options_create());
}

static PyObject *
font_options_get_antialias (PycairoFontOptions *o) {
  return PyInt_FromLong (cairo_font_options_get_antialias (o->font_options));
}

static PyObject *
font_options_get_hint_metrics (PycairoFontOptions *o) {
  return PyInt_FromLong (cairo_font_options_get_hint_metrics
			 (o->font_options));
}

static PyObject *
font_options_get_hint_style (PycairoFontOptions *o) {
  return PyInt_FromLong (cairo_font_options_get_hint_style
			 (o->font_options));
}

static PyObject *
font_options_get_subpixel_order (PycairoFontOptions *o) {
  return PyInt_FromLong (cairo_font_options_get_subpixel_order
			 (o->font_options));
}

static PyObject *
font_options_set_antialias (PycairoFontOptions *o, PyObject *args) {
  cairo_antialias_t aa = CAIRO_ANTIALIAS_DEFAULT;

  if (!PyArg_ParseTuple(args, "|i:FontOptions.set_antialias", &aa))
    return NULL;

  cairo_font_options_set_antialias (o->font_options, aa);
  RETURN_NULL_IF_CAIRO_FONT_OPTIONS_ERROR(o->font_options);
  Py_RETURN_NONE;
}

static PyObject *
font_options_set_hint_metrics (PycairoFontOptions *o, PyObject *args) {
  cairo_hint_metrics_t hm = CAIRO_HINT_METRICS_DEFAULT;

  if (!PyArg_ParseTuple(args, "|i:FontOptions.set_hint_metrics", &hm))
    return NULL;

  cairo_font_options_set_hint_metrics (o->font_options, hm);
  RETURN_NULL_IF_CAIRO_FONT_OPTIONS_ERROR(o->font_options);
  Py_RETURN_NONE;
}

static PyObject *
font_options_set_hint_style (PycairoFontOptions *o, PyObject *args) {
  cairo_hint_style_t hs = CAIRO_HINT_STYLE_DEFAULT;

  if (!PyArg_ParseTuple(args, "|i:FontOptions.set_hint_style", &hs))
    return NULL;

  cairo_font_options_set_hint_style (o->font_options, hs);
  RETURN_NULL_IF_CAIRO_FONT_OPTIONS_ERROR(o->font_options);
  Py_RETURN_NONE;
}

static PyObject *
font_options_set_subpixel_order (PycairoFontOptions *o, PyObject *args) {
  cairo_subpixel_order_t so = CAIRO_SUBPIXEL_ORDER_DEFAULT;

  if (!PyArg_ParseTuple(args, "|i:FontOptions.set_subpixel_order", &so))
    return NULL;

  cairo_font_options_set_subpixel_order (o->font_options, so);
  RETURN_NULL_IF_CAIRO_FONT_OPTIONS_ERROR(o->font_options);
  Py_RETURN_NONE;
}


static PyMethodDef font_options_methods[] = {
  /* methods never exposed in a language binding:
   * cairo_font_options_destroy()
   * cairo_font_options_reference()
   */
  /* TODO: */
  /* copy */
  /* hash */
  /* merge */
  /* equal (richcmp?) */
  {"get_antialias",     (PyCFunction)font_options_get_antialias,  METH_NOARGS},
  {"get_hint_metrics",  (PyCFunction)font_options_get_hint_metrics,
   METH_NOARGS},
  {"get_hint_style",    (PyCFunction)font_options_get_hint_style, METH_NOARGS},
  {"get_subpixel_order",(PyCFunction)font_options_get_subpixel_order,
   METH_NOARGS},
  {"set_antialias",     (PyCFunction)font_options_set_antialias,  METH_VARARGS},
  {"set_hint_metrics",  (PyCFunction)font_options_set_hint_metrics,
   METH_VARARGS},
  {"set_hint_style",    (PyCFunction)font_options_set_hint_style, METH_VARARGS},
  {"set_subpixel_order",(PyCFunction)font_options_set_subpixel_order,
   METH_VARARGS},
  {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoFontOptions_Type = {
  PyObject_HEAD_INIT(NULL)
  0,                                  /* ob_size */
  "cairo.FontOptions",                /* tp_name */
  sizeof(PycairoFontOptions),         /* tp_basicsize */
  0,                                  /* tp_itemsize */
  (destructor)font_options_dealloc,   /* tp_dealloc */
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
  font_options_methods,               /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  0,                                  /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)font_options_new,          /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};
