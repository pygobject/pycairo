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


/* class cairo.FontFace --------------------------------------------------- */

/* PycairoFontFace_FromFontFace
 * Create a new PycairoFontFace from a cairo_font_face_t
 * font_face - a cairo_font_face_t to 'wrap' into a Python object.
 *             it is unreferenced if the PycairoFontFace creation fails
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoFontFace_FromFontFace (cairo_font_face_t *font_face)
{
    PyObject *o;

    assert (font_face != NULL);	

    if (Pycairo_Check_Status (cairo_font_face_status (font_face))) {
	cairo_font_face_destroy (font_face);
	return NULL;
    }

    o = PycairoFontFace_Type.tp_alloc (&PycairoFontFace_Type, 0);
    if (o)
	((PycairoFontFace *)o)->font_face = font_face;
    else
	cairo_font_face_destroy (font_face);
    return o;
}

static void
font_face_dealloc (PycairoFontFace *o)
{
#ifdef DEBUG
    printf("font_face_dealloc start\n");
#endif
    if (o->font_face) {
	cairo_font_face_destroy (o->font_face);
	o->font_face = NULL;
    }
    o->ob_type->tp_free((PyObject *) o);
#ifdef DEBUG
    printf("font_face_dealloc end\n");
#endif
}

static PyObject *
font_face_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyErr_SetString (PyExc_TypeError, "The FontFace type cannot be "
		     "instantiated directly, use Context.get_font_face()");
    return NULL;
}

/*
static PyMethodDef font_face_methods[] = {
     * methods never exposed in a language binding:
     * cairo_font_face_destroy()  
     * cairo_font_face_reference()
     * cairo_font_face_get_user_data()
     * cairo_font_face_set_user_data(),
     *
    {NULL, NULL, 0, NULL},
};
*/

PyTypeObject PycairoFontFace_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    &PyBaseObject_Type,                 /* tp_base */
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


/* class cairo.ScaledFont ------------------------------------------------- */

/* PycairoScaledFont_FromScaledFont
 * Create a new PycairoScaledFont from a cairo_scaled_font_t
 * scaled_font - a cairo_scaled_font_t to 'wrap' into a Python object.
 *               it is unreferenced if the PycairoScaledFont creation fails
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoScaledFont_FromScaledFont (cairo_scaled_font_t *scaled_font)
{
    PyObject *o;

    assert (scaled_font != NULL);	

    if (Pycairo_Check_Status (cairo_scaled_font_status (scaled_font))) {
	cairo_scaled_font_destroy (scaled_font);
	return NULL;
    }

    o = PycairoScaledFont_Type.tp_alloc (&PycairoScaledFont_Type, 0);
    if (o)
	((PycairoScaledFont *)o)->scaled_font = scaled_font;
    else
	cairo_scaled_font_destroy (scaled_font);
    return o;
}

static void
scaled_font_dealloc(PycairoScaledFont *o)
{
#ifdef DEBUG
    printf("scaled_font_dealloc start\n");
#endif
    if (o->scaled_font) {
	cairo_scaled_font_destroy (o->scaled_font);
	o->scaled_font = NULL;
    }
    o->ob_type->tp_free((PyObject *) o);
#ifdef DEBUG
    printf("scaled_font_dealloc end\n");
#endif
}

static PyObject *
scaled_font_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PycairoFontFace *ff;
    PycairoFontOptions *fo;
    PycairoMatrix *mx1, *mx2;

    if (!PyArg_ParseTuple(args, "O!O!O!O!:ScaledFont.__new__",
			  &PycairoFontFace_Type, &ff,
			  &PycairoMatrix_Type, &mx1,
			  &PycairoMatrix_Type, &mx2,
			  &PycairoFontOptions_Type, &fo))
	return NULL;

    PyObject *o = type->tp_alloc(type, 0);
    if (o) {
	cairo_scaled_font_t *scaled_font = cairo_scaled_font_create 
	    (ff->font_face, &mx1->matrix, &mx2->matrix, fo->font_options);

	if (Pycairo_Check_Status (cairo_scaled_font_status (scaled_font))) {
	    cairo_scaled_font_destroy (scaled_font);
	    Py_DECREF(o);
	    return NULL;
	}
	((PycairoScaledFont *)o)->scaled_font = scaled_font;
    }
    return o;
}

static PyObject *
scaled_font_extents (PycairoScaledFont *o)
{
    cairo_font_extents_t e;

    cairo_scaled_font_extents (o->scaled_font, &e);
    if (Pycairo_Check_Status (cairo_scaled_font_status(o->scaled_font)))
	return NULL;
    return Py_BuildValue ("(ddddd)", e.ascent, e.descent, e.height, 
			  e.max_x_advance, e.max_y_advance);
}

static PyMethodDef scaled_font_methods[] = {
    /* methods never exposed in a language binding:
     * cairo_scaled_font_destroy()  
     * cairo_scaled_font_reference()
     */
    /* glyph_extents - undocumented */
    {"extents", (PyCFunction)scaled_font_extents, METH_NOARGS},
    {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoScaledFont_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    &PyBaseObject_Type,                 /* tp_base */
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
PycairoFontOptions_FromFontOptions (cairo_font_options_t *font_options)
{
    PyObject *o;

    assert (font_options != NULL);	

    if (Pycairo_Check_Status (cairo_font_options_status (font_options))) {
	cairo_font_options_destroy (font_options);
	return NULL;
    }

    o = PycairoFontOptions_Type.tp_alloc (&PycairoFontOptions_Type, 0);
    if (o)
	((PycairoFontOptions *)o)->font_options = font_options;
    else
	cairo_font_options_destroy (font_options);
    return o;
}

static void
font_options_dealloc(PycairoFontOptions *o)
{
#ifdef DEBUG
    printf("font_options_dealloc start\n");
#endif
    if (o->font_options) {
	cairo_font_options_destroy (o->font_options);
	o->font_options = NULL;
    }
    o->ob_type->tp_free((PyObject *) o);
#ifdef DEBUG
    printf("font_options_dealloc end\n");
#endif
}

static PyObject *
font_options_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *o = type->tp_alloc(type, 0);
    if (o) {
	cairo_font_options_t *font_options = cairo_font_options_create();

	if (Pycairo_Check_Status (cairo_font_options_status (font_options))) {
	    cairo_font_options_destroy (font_options);
	    Py_DECREF(o);
	    return NULL;
	}
	((PycairoFontOptions *)o)->font_options = font_options;
    }
    return o;
}


static PyMethodDef font_options_methods[] = {
    /* methods never exposed in a language binding:
     * cairo_font_options_destroy()  
     * cairo_font_options_reference()
     */
    /* TODO:
     * copy
     * merge
     * equal (richcmp?)
     * set/get_antialias
     * set/get_subpixel_order
     * set/get_hint_style
     * set/get_hint_metrics
     */
    {NULL, NULL, 0, NULL},
};


PyTypeObject PycairoFontOptions_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
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
    &PyBaseObject_Type,                 /* tp_base */
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
