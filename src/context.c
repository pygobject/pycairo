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


/* PycairoContext_FromContext
 * Create a new PycairoContext from a cairo_t
 * ctx  - a cairo_t to 'wrap' into a Python object.
 *        It is unreferenced if the PycairoContext creation fails, or if
 *        the cairo_t has an error status.
 * type - a pointer to the type to instantiate.
 *        It can be &PycairoContext_Type, or a PycairoContext_Type subtype.
 *        (cairo.Context or a cairo.Context subclass)
 * base - the base object used to create the context, or NULL.
 *        it is referenced to keep it alive while the cairo_t is being used
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoContext_FromContext(cairo_t *ctx, PyTypeObject *type, PyObject *base) {
  PyObject *o;

  assert (ctx != NULL);

  if (Pycairo_Check_Status (cairo_status (ctx))) {
    cairo_destroy (ctx);
    return NULL;
  }

  o = PycairoContext_Type.tp_alloc (type, 0);
  if (o) {
    ((PycairoContext *)o)->ctx = ctx;
    Py_XINCREF(base);
    ((PycairoContext *)o)->base = base;
  } else {
    cairo_destroy (ctx);
  }
  return o;
}

static void
pycairo_dealloc(PycairoContext *o) {
  if (o->ctx) {
    cairo_destroy(o->ctx);
    o->ctx = NULL;
  }
  Py_CLEAR(o->base);

  o->ob_type->tp_free((PyObject *)o);
}

static PyObject *
pycairo_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PycairoSurface *s;
  if (!PyArg_ParseTuple(args, "O!:Context.__new__",
			&PycairoSurface_Type, &s))
    return NULL;
  return PycairoContext_FromContext (cairo_create (s->surface), type, NULL);
}

static PyObject *
pycairo_append_path (PycairoContext *o, PyObject *args) {
  PycairoPath *p;

  if (!PyArg_ParseTuple(args, "O!:Context.append_path",
			&PycairoPath_Type, &p))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_append_path (o->ctx, p->path);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_arc (PycairoContext *o, PyObject *args) {
  double xc, yc, radius, angle1, angle2;

  if (!PyArg_ParseTuple (args, "ddddd:Context.arc",
			 &xc, &yc, &radius, &angle1, &angle2))
    return NULL;

  cairo_arc (o->ctx, xc, yc, radius, angle1, angle2);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_arc_negative (PycairoContext *o, PyObject *args) {
  double xc, yc, radius, angle1, angle2;

  if (!PyArg_ParseTuple (args, "ddddd:Context.arc_negative",
			 &xc, &yc, &radius, &angle1, &angle2))
    return NULL;

  cairo_arc_negative (o->ctx, xc, yc, radius, angle1, angle2);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_clip (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_clip (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_clip_extents (PycairoContext *o) {
  double x1, y1, x2, y2;
  cairo_clip_extents (o->ctx, &x1, &y1, &x2, &y2);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dddd)", x1, y1, x2, y2);
}

static PyObject *
pycairo_clip_preserve (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_clip_preserve (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_close_path (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_close_path (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_copy_clip_rectangle_list (PycairoContext *o) {
  int i;
  PyObject *rv = NULL;
  cairo_rectangle_t *r;
  cairo_rectangle_list_t *rlist = cairo_copy_clip_rectangle_list (o->ctx);
  if (rlist->status != CAIRO_STATUS_SUCCESS) {
    Pycairo_Check_Status (rlist->status);
    goto exit;
  }

  rv = PyTuple_New(rlist->num_rectangles);
  if (rv == NULL)
    goto exit;

  for (i = 0, r = rlist->rectangles; i < rlist->num_rectangles; i++, r++) {
    PyObject *py_rect = Py_BuildValue("(dddd)", r->x, r->y,
				      r->width, r->height);
    if (py_rect == NULL) {
      Py_CLEAR(rv);
      goto exit;
    }
    PyTuple_SET_ITEM (rv, i, py_rect);
  }
 exit:
  cairo_rectangle_list_destroy(rlist);
  return rv;
}

static PyObject *
pycairo_copy_page (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_copy_page (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_copy_path (PycairoContext *o) {
  cairo_path_t *cp;
  Py_BEGIN_ALLOW_THREADS;
  cp = cairo_copy_path (o->ctx);
  Py_END_ALLOW_THREADS;
  return PycairoPath_FromPath (cp);
}

static PyObject *
pycairo_copy_path_flat (PycairoContext *o) {
  cairo_path_t *cp;
  Py_BEGIN_ALLOW_THREADS;
  cp = cairo_copy_path_flat (o->ctx);
  Py_END_ALLOW_THREADS;
  return PycairoPath_FromPath (cp);
}

static PyObject *
pycairo_curve_to (PycairoContext *o, PyObject *args) {
  double x1, y1, x2, y2, x3, y3;

  if (!PyArg_ParseTuple (args, "dddddd:Context.curve_to",
			 &x1, &y1, &x2, &y2, &x3, &y3))
    return NULL;

  cairo_curve_to (o->ctx, x1, y1, x2, y2, x3, y3);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_device_to_user(PycairoContext *o, PyObject *args) {
  double x, y;

  if (!PyArg_ParseTuple(args, "dd:Context.device_to_user", &x, &y))
    return NULL;

  cairo_device_to_user(o->ctx, &x, &y);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dd)", x, y);
}

static PyObject *
pycairo_device_to_user_distance (PycairoContext *o, PyObject *args) {
  double dx, dy;

  if (!PyArg_ParseTuple (args, "dd:Context.device_to_user_distance",
			 &dx, &dy))
    return NULL;

  cairo_device_to_user_distance (o->ctx, &dx, &dy);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dd)", dx, dy);
}

static PyObject *
pycairo_fill (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_fill (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_fill_extents (PycairoContext *o) {
  double x1, y1, x2, y2;
  cairo_fill_extents (o->ctx, &x1, &y1, &x2, &y2);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dddd)", x1, y1, x2, y2);
}

static PyObject *
pycairo_fill_preserve (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_fill_preserve (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_font_extents (PycairoContext *o) {
  cairo_font_extents_t e;

  cairo_font_extents (o->ctx, &e);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(ddddd)", e.ascent, e.descent, e.height,
		       e.max_x_advance, e.max_y_advance);
}

static PyObject *
pycairo_get_antialias (PycairoContext *o) {
  return PyInt_FromLong (cairo_get_antialias (o->ctx));
}

static PyObject *
pycairo_get_current_point (PycairoContext *o) {
  double x, y;
  cairo_get_current_point (o->ctx, &x, &y);
  return Py_BuildValue("(dd)", x, y);
}

static PyObject *
pycairo_get_dash (PycairoContext *o) {
  double *dashes = NULL, offset;
  int count, i;
  PyObject *py_dashes = NULL, *rv = NULL;

  count = cairo_get_dash_count (o->ctx);
  dashes = PyMem_Malloc (count * sizeof(double));
  if (dashes == NULL)
    return PyErr_NoMemory();

  cairo_get_dash (o->ctx, dashes, &offset);
  py_dashes = PyTuple_New(count);
  if (py_dashes == NULL)
    goto exit;

  for (i = 0; i < count; i++) {
    PyObject *dash = PyFloat_FromDouble(dashes[i]);
    if (dash == NULL)
      goto exit;
    PyTuple_SET_ITEM (py_dashes, i, dash);
  }
  rv = Py_BuildValue("(Od)", py_dashes, offset);

 exit:
  PyMem_Free (dashes);
  Py_XDECREF(py_dashes);
  return rv;
}

static PyObject *
pycairo_get_dash_count (PycairoContext *o) {
  return PyInt_FromLong (cairo_get_dash_count (o->ctx));
}

static PyObject *
pycairo_get_fill_rule (PycairoContext *o) {
  return PyInt_FromLong(cairo_get_fill_rule (o->ctx));
}

static PyObject *
pycairo_get_font_face (PycairoContext *o) {
  return PycairoFontFace_FromFontFace (
       cairo_font_face_reference (cairo_get_font_face (o->ctx)));
}

static PyObject *
pycairo_get_font_matrix (PycairoContext *o) {
  cairo_matrix_t matrix;
  cairo_get_font_matrix (o->ctx, &matrix);
  return PycairoMatrix_FromMatrix (&matrix);
}

static PyObject *
pycairo_get_font_options (PycairoContext *o) {
  cairo_font_options_t *options = cairo_font_options_create();
  cairo_get_font_options (o->ctx, options);
  /* there is no reference fn */
  return PycairoFontOptions_FromFontOptions (options);
}

static PyObject *
pycairo_get_group_target (PycairoContext *o) {
  cairo_surface_t *surface = cairo_get_group_target (o->ctx);
  if (surface != NULL)
    return PycairoSurface_FromSurface (cairo_surface_reference (surface),
				       NULL);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_get_line_cap (PycairoContext *o) {
  return PyInt_FromLong(cairo_get_line_cap (o->ctx));
}

static PyObject *
pycairo_get_line_join (PycairoContext *o) {
  return PyInt_FromLong(cairo_get_line_join (o->ctx));
}

static PyObject *
pycairo_get_line_width (PycairoContext *o) {
  return PyFloat_FromDouble(cairo_get_line_width (o->ctx));
}

static PyObject *
pycairo_get_matrix (PycairoContext *o) {
  cairo_matrix_t matrix;
  cairo_get_matrix (o->ctx, &matrix);
  return PycairoMatrix_FromMatrix (&matrix);
}

static PyObject *
pycairo_get_miter_limit (PycairoContext *o) {
  return PyFloat_FromDouble (cairo_get_miter_limit (o->ctx));
}

static PyObject *
pycairo_get_operator (PycairoContext *o) {
  return PyInt_FromLong (cairo_get_operator (o->ctx));
}

static PyObject *
pycairo_get_scaled_font (PycairoContext *o) {
  return PycairoScaledFont_FromScaledFont (
	   cairo_scaled_font_reference (cairo_get_scaled_font (o->ctx)));
}

static PyObject *
pycairo_get_source (PycairoContext *o) {
  return PycairoPattern_FromPattern (
	       cairo_pattern_reference (cairo_get_source (o->ctx)), NULL);
}

static PyObject *
pycairo_get_target (PycairoContext *o) {
  return PycairoSurface_FromSurface (
	       cairo_surface_reference (cairo_get_target (o->ctx)),
	       NULL);
}

static PyObject *
pycairo_get_tolerance (PycairoContext *o) {
  return PyFloat_FromDouble (cairo_get_tolerance (o->ctx));
}

/* read a Python sequence of (i,x,y) sequences
 * return cairo_glyph_t *
 *        num_glyphs
 *        must call PyMem_Free(glyphs) when finished using the glyphs
 */
static cairo_glyph_t *
_PyGlyphs_AsGlyphs (PyObject *py_object, int *num_glyphs)
{
  int length, i;
  cairo_glyph_t *glyphs = NULL, *glyph;
  PyObject *py_glyphs, *py_seq = NULL;

  py_glyphs = PySequence_Fast (py_object, "glyphs must be a sequence");
  if (py_glyphs == NULL)
    return NULL;

  length = PySequence_Fast_GET_SIZE(py_glyphs);
  if (*num_glyphs < 0 || *num_glyphs > length)
    *num_glyphs = length;

  glyphs = PyMem_Malloc (*num_glyphs * sizeof(cairo_glyph_t));
  if (glyphs == NULL) {
    PyErr_NoMemory();
    goto error;
  }
  for (i = 0, glyph = glyphs; i < *num_glyphs; i++, glyph++) {
    PyObject *py_item = PySequence_Fast_GET_ITEM(py_glyphs, i);
    py_seq = PySequence_Fast (py_item, "glyph items must be a sequence");
    if (py_seq == NULL)
      goto error;
    if (PySequence_Fast_GET_SIZE(py_seq) != 3) {
      PyErr_SetString(PyExc_ValueError,
		      "each glyph item must be an (i,x,y) sequence");
      goto error;
    }
    glyph->index = PyInt_AsLong(PySequence_Fast_GET_ITEM(py_seq, 0));
    glyph->x = PyFloat_AsDouble(PySequence_Fast_GET_ITEM(py_seq, 1));
    glyph->y = PyFloat_AsDouble(PySequence_Fast_GET_ITEM(py_seq, 2));
    if (PyErr_Occurred())
      goto error;
    Py_DECREF(py_seq);
  }
  Py_DECREF(py_glyphs);
  return glyphs;
 error:
  Py_DECREF(py_glyphs);
  Py_XDECREF(py_seq);
  PyMem_Free(glyphs);
  return NULL;
}

static PyObject *
pycairo_glyph_extents (PycairoContext *o, PyObject *args) {
  int num_glyphs = -1;
  cairo_glyph_t *glyphs;
  cairo_text_extents_t extents;
  PyObject *py_object;

  if (!PyArg_ParseTuple (args, "O|i:Context.glyph_extents",
			 &py_object, &num_glyphs))
    return NULL;

  glyphs = _PyGlyphs_AsGlyphs (py_object, &num_glyphs);
  if (glyphs == NULL)
    return NULL;
  cairo_glyph_extents (o->ctx, glyphs, num_glyphs, &extents);
  PyMem_Free (glyphs);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dddddd)", extents.x_bearing, extents.y_bearing,
		       extents.width, extents.height, extents.x_advance,
		       extents.y_advance);
}

static PyObject *
pycairo_glyph_path (PycairoContext *o, PyObject *args) {
  int num_glyphs = -1;
  cairo_glyph_t *glyphs;
  PyObject *py_object;

  if (!PyArg_ParseTuple (args, "O|i:Context.glyph_path",
			 &py_object, &num_glyphs))
    return NULL;

  glyphs = _PyGlyphs_AsGlyphs (py_object, &num_glyphs);
  if (glyphs == NULL)
    return NULL;
  cairo_glyph_path (o->ctx, glyphs, num_glyphs);
  PyMem_Free (glyphs);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_has_current_point (PycairoContext *o) {
  PyObject *b = cairo_has_current_point (o->ctx) ? Py_True : Py_False;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_INCREF(b);
  return b;
}

static PyObject *
pycairo_identity_matrix (PycairoContext *o) {
  cairo_identity_matrix (o->ctx);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_in_fill (PycairoContext *o, PyObject *args) {
  double x, y;
  PyObject *result;

  if (!PyArg_ParseTuple (args, "dd:Context.in_fill", &x, &y))
    return NULL;

  result = cairo_in_fill (o->ctx, x, y) ? Py_True : Py_False;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_INCREF(result);
  return result;
}

static PyObject *
pycairo_in_stroke (PycairoContext *o, PyObject *args) {
  double x, y;
  PyObject *result;

  if (!PyArg_ParseTuple (args, "dd:Context.in_stroke", &x, &y))
    return NULL;

  result = cairo_in_stroke (o->ctx, x, y) ? Py_True : Py_False;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_INCREF(result);
  return result;
}

static PyObject *
pycairo_line_to (PycairoContext *o, PyObject *args) {
  double x, y;

  if (!PyArg_ParseTuple (args, "dd:Context.line_to", &x, &y))
    return NULL;

  cairo_line_to (o->ctx, x, y);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_mask (PycairoContext *o, PyObject *args) {
  PycairoPattern *p;

  if (!PyArg_ParseTuple(args, "O!:Context.mask", &PycairoPattern_Type, &p))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mask (o->ctx, p->pattern);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_mask_surface (PycairoContext *o, PyObject *args) {
  PycairoSurface *s;
  double surface_x = 0.0, surface_y = 0.0;

  if (!PyArg_ParseTuple (args, "O!|dd:Context.mask_surface",
			 &PycairoSurface_Type, &s, &surface_x, &surface_y))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mask_surface (o->ctx, s->surface, surface_x, surface_y);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_move_to (PycairoContext *o, PyObject *args) {
  double x, y;

  if (!PyArg_ParseTuple (args, "dd:Context.move_to", &x, &y))
    return NULL;

  cairo_move_to (o->ctx, x, y);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_new_path (PycairoContext *o) {
  cairo_new_path (o->ctx);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_new_sub_path (PycairoContext *o) {
  cairo_new_sub_path (o->ctx);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_paint (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_paint (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_paint_with_alpha (PycairoContext *o, PyObject *args) {
  double alpha;

  if (!PyArg_ParseTuple (args, "d:Context.paint_with_alpha", &alpha))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_paint_with_alpha (o->ctx, alpha);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_path_extents (PycairoContext *o) {
  double x1, y1, x2, y2;
  cairo_path_extents (o->ctx, &x1, &y1, &x2, &y2);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dddd)", x1, y1, x2, y2);
}

static PyObject *
pycairo_pop_group (PycairoContext *o) {
  return PycairoPattern_FromPattern (cairo_pop_group (o->ctx), NULL);
}

static PyObject *
pycairo_pop_group_to_source (PycairoContext *o) {
  cairo_pop_group_to_source (o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_push_group (PycairoContext *o) {
  cairo_push_group (o->ctx);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_push_group_with_content (PycairoContext *o, PyObject *args) {
  cairo_content_t content;

  if (!PyArg_ParseTuple(args, "i:Context.push_group_with_content",
			&content))
    return NULL;
  cairo_push_group_with_content (o->ctx, content);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_rectangle (PycairoContext *o, PyObject *args) {
  double x, y, width, height;

  if (!PyArg_ParseTuple (args, "dddd:Context.rectangle",
			 &x, &y, &width, &height))
    return NULL;

  cairo_rectangle (o->ctx, x, y, width, height);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_rel_curve_to (PycairoContext *o, PyObject *args) {
  double dx1, dy1, dx2, dy2, dx3, dy3;

  if (!PyArg_ParseTuple (args, "dddddd:Context.rel_curve_to",
			 &dx1, &dy1, &dx2, &dy2, &dx3, &dy3))
    return NULL;

  cairo_rel_curve_to (o->ctx, dx1, dy1, dx2, dy2, dx3, dy3);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_rel_line_to (PycairoContext *o, PyObject *args) {
  double dx, dy;

  if (!PyArg_ParseTuple (args, "dd:Context.rel_line_to", &dx, &dy))
    return NULL;

  cairo_rel_line_to (o->ctx, dx, dy);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_rel_move_to (PycairoContext *o, PyObject *args) {
  double dx, dy;

  if (!PyArg_ParseTuple (args, "dd:Context.rel_move_to", &dx, &dy))
    return NULL;

  cairo_rel_move_to (o->ctx, dx, dy);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_reset_clip (PycairoContext *o) {
  cairo_reset_clip (o->ctx);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_restore (PycairoContext *o) {
  cairo_restore (o->ctx);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_rotate (PycairoContext *o, PyObject *args) {
  double angle;

  if (!PyArg_ParseTuple(args, "d:Context.rotate", &angle))
    return NULL;

  cairo_rotate (o->ctx, angle);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_save (PycairoContext *o) {
  cairo_save (o->ctx);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_scale (PycairoContext *o, PyObject *args) {
  double sx, sy;

  if (!PyArg_ParseTuple (args, "dd:Context.scale", &sx, &sy))
    return NULL;

  cairo_scale (o->ctx, sx, sy);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_select_font_face (PycairoContext *o, PyObject *args) {
  const char *utf8;
  cairo_font_slant_t slant   = CAIRO_FONT_SLANT_NORMAL;
  cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;

  if (!PyArg_ParseTuple (args, "et|ii:Context.select_font_face",
			 "utf-8", &utf8, &slant, &weight))
    return NULL;

  cairo_select_font_face (o->ctx, utf8, slant, weight);
  PyMem_Free((void *)utf8);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_antialias (PycairoContext *o, PyObject *args) {
  cairo_antialias_t antialias = CAIRO_ANTIALIAS_DEFAULT;

  if (!PyArg_ParseTuple(args, "|i:Context.set_antialias", &antialias))
    return NULL;

  cairo_set_antialias (o->ctx, antialias);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_dash (PycairoContext *o, PyObject *args) {
  double *dashes, offset = 0;
  int num_dashes, i;
  PyObject *py_dashes;

  if (!PyArg_ParseTuple (args, "O|d:Context.set_dash", &py_dashes, &offset))
    return NULL;

  py_dashes = PySequence_Fast (py_dashes,
			       "first argument must be a sequence");
  if (py_dashes == NULL)
    return NULL;

  num_dashes = PySequence_Fast_GET_SIZE(py_dashes);
  dashes = PyMem_Malloc (num_dashes * sizeof(double));
  if (dashes == NULL) {
    Py_DECREF(py_dashes);
    return PyErr_NoMemory();
  }

  for (i = 0; i < num_dashes; i++) {
    dashes[i] = PyFloat_AsDouble(PySequence_Fast_GET_ITEM(py_dashes, i));
    if (PyErr_Occurred()) {
      PyMem_Free (dashes);
      Py_DECREF(py_dashes);
      return NULL;
    }
  }
  cairo_set_dash (o->ctx, dashes, num_dashes, offset);
  PyMem_Free (dashes);
  Py_DECREF(py_dashes);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_fill_rule (PycairoContext *o, PyObject *args) {
  cairo_fill_rule_t fill_rule;

  if (!PyArg_ParseTuple (args, "i:Context.set_fill_rule", &fill_rule))
    return NULL;

  cairo_set_fill_rule (o->ctx, fill_rule);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_font_face (PycairoContext *o, PyObject *obj) {
  if (PyObject_TypeCheck(obj, &PycairoFontFace_Type))
    cairo_set_font_face (o->ctx, ((PycairoFontFace *)obj)->font_face);
  else if (obj == Py_None)
    cairo_set_font_face (o->ctx, NULL);
  else {
    PyErr_SetString(PyExc_TypeError,
		    "Context.set_font_face() argument must be "
		    "cairo.FontFace or None");
    return NULL;
  }
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_font_matrix (PycairoContext *o, PyObject *args) {
  PycairoMatrix *matrix;

  if (!PyArg_ParseTuple (args, "O!:Context.set_font_matrix",
			 &PycairoMatrix_Type, &matrix))
    return NULL;

  cairo_set_font_matrix (o->ctx, &matrix->matrix);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_font_options (PycairoContext *o, PyObject *args) {
  PycairoFontOptions *options;

  if (!PyArg_ParseTuple (args, "O!:Context.set_font_options",
			 &PycairoFontOptions_Type, &options))
    return NULL;

  cairo_set_font_options (o->ctx, options->font_options);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_font_size (PycairoContext *o, PyObject *args) {
  double size;

  if (!PyArg_ParseTuple (args, "d:Context.set_font_size", &size))
    return NULL;

  cairo_set_font_size (o->ctx, size);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_line_cap (PycairoContext *o, PyObject *args) {
  cairo_line_cap_t line_cap;

  if (!PyArg_ParseTuple (args, "i:Context.set_line_cap", &line_cap))
    return NULL;

  cairo_set_line_cap (o->ctx, line_cap);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_line_join (PycairoContext *o, PyObject *args) {
  cairo_line_join_t line_join;

  if (!PyArg_ParseTuple (args, "i:Context.set_line_join", &line_join))
    return NULL;

  cairo_set_line_join (o->ctx, line_join);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_line_width (PycairoContext *o, PyObject *args) {
  double width;

  if (!PyArg_ParseTuple (args, "d:Context.set_line_width", &width))
    return NULL;

  cairo_set_line_width (o->ctx, width);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_matrix (PycairoContext *o, PyObject *args) {
  PycairoMatrix *matrix;

  if (!PyArg_ParseTuple (args, "O!:Context.set_matrix",
			 &PycairoMatrix_Type, &matrix))
    return NULL;

  cairo_set_matrix (o->ctx, &matrix->matrix);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_miter_limit (PycairoContext *o, PyObject *args) {
  double limit;

  if (!PyArg_ParseTuple (args, "d:Context.set_miter_limit", &limit))
    return NULL;

  cairo_set_miter_limit (o->ctx, limit);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_operator(PycairoContext *o, PyObject *args) {
  cairo_operator_t op;

  if (!PyArg_ParseTuple(args, "i:Context.set_operator", &op))
    return NULL;

  cairo_set_operator(o->ctx, op);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_scaled_font(PycairoContext *o, PyObject *args) {
  PycairoScaledFont *f;
  if (!PyArg_ParseTuple (args, "O!:Context.set_scaled_font",
			 &PycairoScaledFont_Type, &f))
    return NULL;

  cairo_set_scaled_font(o->ctx, f->scaled_font);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_source (PycairoContext *o, PyObject *args) {
  PycairoPattern *p;

  if (!PyArg_ParseTuple (args, "O!:Context.set_source",
			 &PycairoPattern_Type, &p))
    return NULL;

  cairo_set_source (o->ctx, p->pattern);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_source_rgb (PycairoContext *o, PyObject *args) {
  double red, green, blue;

  if (!PyArg_ParseTuple (args, "ddd:Context.set_source_rgb",
			 &red, &green, &blue))
    return NULL;

  cairo_set_source_rgb (o->ctx, red, green, blue);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_source_rgba (PycairoContext *o, PyObject *args) {
  double red, green, blue;
  double alpha = 1.0;

  if (!PyArg_ParseTuple (args, "ddd|d:Context.set_source_rgba",
			 &red, &green, &blue, &alpha))
    return NULL;

  cairo_set_source_rgba (o->ctx, red, green, blue, alpha);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_source_surface (PycairoContext *o, PyObject *args) {
  PycairoSurface *surface;
  double x = 0.0, y = 0.0;

  if (!PyArg_ParseTuple (args, "O!|dd:Context.set_source_surface",
			 &PycairoSurface_Type, &surface, &x, &y))
    return NULL;

  cairo_set_source_surface (o->ctx, surface->surface, x, y);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_set_tolerance (PycairoContext *o, PyObject *args) {
  double tolerance;
  if (!PyArg_ParseTuple (args, "d:Context.set_tolerance", &tolerance))
    return NULL;
  cairo_set_tolerance (o->ctx, tolerance);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_show_glyphs (PycairoContext *o, PyObject *args) {
  int num_glyphs = -1;
  cairo_glyph_t *glyphs;
  PyObject *py_object;

  if (!PyArg_ParseTuple (args, "O|i:Context.show_glyphs",
			 &py_object, &num_glyphs))
    return NULL;

  glyphs = _PyGlyphs_AsGlyphs (py_object, &num_glyphs);
  if (glyphs == NULL)
    return NULL;
  Py_BEGIN_ALLOW_THREADS;
  cairo_show_glyphs (o->ctx, glyphs, num_glyphs);
  Py_END_ALLOW_THREADS;
  PyMem_Free (glyphs);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_show_page (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_show_page (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_show_text (PycairoContext *o, PyObject *args) {
  const char *utf8;

  if (!PyArg_ParseTuple (args, "et:Context.show_text", "utf-8", &utf8))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_show_text (o->ctx, utf8);
  Py_END_ALLOW_THREADS;

  PyMem_Free((void *)utf8);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_stroke (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_stroke (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_stroke_extents (PycairoContext *o) {
  double x1, y1, x2, y2;
  cairo_stroke_extents (o->ctx, &x1, &y1, &x2, &y2);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dddd)", x1, y1, x2, y2);
}

static PyObject *
pycairo_stroke_preserve (PycairoContext *o) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_stroke_preserve (o->ctx);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_text_extents (PycairoContext *o, PyObject *args) {
  cairo_text_extents_t extents;
  const char *utf8;

  if (!PyArg_ParseTuple (args, "et:Context.text_extents", "utf-8", &utf8))
    return NULL;

  cairo_text_extents (o->ctx, utf8, &extents);
  PyMem_Free((void *)utf8);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dddddd)", extents.x_bearing, extents.y_bearing,
		       extents.width, extents.height, extents.x_advance,
		       extents.y_advance);
}

static PyObject *
pycairo_text_path (PycairoContext *o, PyObject *args) {
  const char *utf8;

  if (!PyArg_ParseTuple (args, "et:Context.text_path", "utf-8", &utf8))
    return NULL;

  cairo_text_path (o->ctx, utf8);
  PyMem_Free((void *)utf8);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_transform (PycairoContext *o, PyObject *args) {
  PycairoMatrix *matrix;

  if (!PyArg_ParseTuple (args, "O!:Context.transform",
			 &PycairoMatrix_Type, &matrix))
    return NULL;

  cairo_transform (o->ctx, &matrix->matrix);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_translate (PycairoContext *o, PyObject *args) {
  double tx, ty;

  if (!PyArg_ParseTuple (args, "dd:Context.translate", &tx, &ty))
    return NULL;

  cairo_translate (o->ctx, tx, ty);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  Py_RETURN_NONE;
}

static PyObject *
pycairo_user_to_device (PycairoContext *o, PyObject *args) {
  double x, y;

  if (!PyArg_ParseTuple (args, "dd:Context.user_to_device", &x, &y))
    return NULL;

  cairo_user_to_device (o->ctx, &x, &y);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dd)", x, y);
}

static PyObject *
pycairo_user_to_device_distance (PycairoContext *o, PyObject *args) {
  double dx, dy;

  if (!PyArg_ParseTuple (args, "dd:Context.user_to_device_distance",
			 &dx, &dy))
    return NULL;

  cairo_user_to_device_distance (o->ctx, &dx, &dy);
  RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(o->ctx);
  return Py_BuildValue("(dd)", dx, dy);
}


static PyMethodDef pycairo_methods[] = {
  /* methods never exposed in a language binding:
   * cairo_destroy()
   * cairo_reference()
   * cairo_rectangle_list_destroy()
   *
   * cairo_status()
   * cairo_status_string()
   * - not needed since Pycairo calls Pycairo_Check_Status() to check
   *   for errors and raise exceptions
   */
  {"append_path",     (PyCFunction)pycairo_append_path,      METH_VARARGS},
  {"arc",             (PyCFunction)pycairo_arc,              METH_VARARGS},
  {"arc_negative",    (PyCFunction)pycairo_arc_negative,     METH_VARARGS},
  {"clip",            (PyCFunction)pycairo_clip,             METH_NOARGS},
  {"clip_extents",    (PyCFunction)pycairo_clip_extents,     METH_NOARGS},
  {"clip_preserve",   (PyCFunction)pycairo_clip_preserve,    METH_NOARGS},
  {"close_path",      (PyCFunction)pycairo_close_path,       METH_NOARGS},
  {"copy_clip_rectangle_list", (PyCFunction)pycairo_copy_clip_rectangle_list,
   METH_NOARGS},
  {"copy_page",       (PyCFunction)pycairo_copy_page,        METH_NOARGS},
  {"copy_path",       (PyCFunction)pycairo_copy_path,        METH_NOARGS},
  {"copy_path_flat",  (PyCFunction)pycairo_copy_path_flat,   METH_NOARGS},
  {"curve_to",        (PyCFunction)pycairo_curve_to,         METH_VARARGS},
  {"device_to_user",  (PyCFunction)pycairo_device_to_user,   METH_VARARGS},
  {"device_to_user_distance", (PyCFunction)pycairo_device_to_user_distance,
   METH_VARARGS},
  {"fill",            (PyCFunction)pycairo_fill,             METH_NOARGS},
  {"fill_extents",    (PyCFunction)pycairo_fill_extents,     METH_NOARGS},
  {"fill_preserve",   (PyCFunction)pycairo_fill_preserve,    METH_NOARGS},
  {"font_extents",    (PyCFunction)pycairo_font_extents,     METH_NOARGS},
  {"get_antialias",   (PyCFunction)pycairo_get_antialias,    METH_NOARGS},
  {"get_current_point",(PyCFunction)pycairo_get_current_point,METH_NOARGS},
  {"get_dash",        (PyCFunction)pycairo_get_dash,         METH_NOARGS},
  {"get_dash_count",  (PyCFunction)pycairo_get_dash_count,   METH_NOARGS},
  {"get_fill_rule",   (PyCFunction)pycairo_get_fill_rule,    METH_NOARGS},
  {"get_font_face",   (PyCFunction)pycairo_get_font_face,    METH_NOARGS},
  {"get_font_matrix", (PyCFunction)pycairo_get_font_matrix,  METH_NOARGS},
  {"get_font_options",(PyCFunction)pycairo_get_font_options, METH_NOARGS},
  {"get_group_target",(PyCFunction)pycairo_get_group_target, METH_NOARGS},
  {"get_line_cap",    (PyCFunction)pycairo_get_line_cap,     METH_NOARGS},
  {"get_line_join",   (PyCFunction)pycairo_get_line_join,    METH_NOARGS},
  {"get_line_width",  (PyCFunction)pycairo_get_line_width,   METH_NOARGS},
  {"get_matrix",      (PyCFunction)pycairo_get_matrix,       METH_NOARGS},
  {"get_miter_limit", (PyCFunction)pycairo_get_miter_limit,  METH_NOARGS},
  {"get_operator",    (PyCFunction)pycairo_get_operator,     METH_NOARGS},
  {"get_scaled_font", (PyCFunction)pycairo_get_scaled_font,  METH_NOARGS},
  {"get_source",      (PyCFunction)pycairo_get_source,       METH_NOARGS},
  {"get_target",      (PyCFunction)pycairo_get_target,       METH_NOARGS},
  {"get_tolerance",   (PyCFunction)pycairo_get_tolerance,    METH_NOARGS},
  {"glyph_extents",   (PyCFunction)pycairo_glyph_extents,    METH_VARARGS},
  {"glyph_path",      (PyCFunction)pycairo_glyph_path,       METH_VARARGS},
  {"has_current_point",(PyCFunction)pycairo_has_current_point, METH_NOARGS},
  {"identity_matrix", (PyCFunction)pycairo_identity_matrix,  METH_NOARGS},
  {"in_fill",         (PyCFunction)pycairo_in_fill,          METH_VARARGS},
  {"in_stroke",       (PyCFunction)pycairo_in_stroke,        METH_VARARGS},
  {"line_to",         (PyCFunction)pycairo_line_to,          METH_VARARGS},
  {"mask",            (PyCFunction)pycairo_mask,             METH_VARARGS},
  {"mask_surface",    (PyCFunction)pycairo_mask_surface,     METH_VARARGS},
  {"move_to",         (PyCFunction)pycairo_move_to,          METH_VARARGS},
  {"new_path",        (PyCFunction)pycairo_new_path,         METH_NOARGS},
  {"new_sub_path",    (PyCFunction)pycairo_new_sub_path,     METH_NOARGS},
  {"paint",           (PyCFunction)pycairo_paint,            METH_NOARGS},
  {"paint_with_alpha",(PyCFunction)pycairo_paint_with_alpha, METH_VARARGS},
  {"path_extents",    (PyCFunction)pycairo_path_extents,     METH_NOARGS},
  {"pop_group",       (PyCFunction)pycairo_pop_group,        METH_NOARGS},
  {"pop_group_to_source", (PyCFunction)pycairo_pop_group_to_source,
   METH_NOARGS},
  {"push_group",      (PyCFunction)pycairo_push_group,       METH_NOARGS},
  {"push_group_with_content", (PyCFunction)pycairo_push_group_with_content,
   METH_VARARGS},
  {"rectangle",       (PyCFunction)pycairo_rectangle,        METH_VARARGS},
  {"rel_curve_to",    (PyCFunction)pycairo_rel_curve_to,     METH_VARARGS},
  {"rel_line_to",     (PyCFunction)pycairo_rel_line_to,      METH_VARARGS},
  {"rel_move_to",     (PyCFunction)pycairo_rel_move_to,      METH_VARARGS},
  {"reset_clip",      (PyCFunction)pycairo_reset_clip,       METH_NOARGS},
  {"restore",         (PyCFunction)pycairo_restore,          METH_NOARGS},
  {"rotate",          (PyCFunction)pycairo_rotate,           METH_VARARGS},
  {"save",            (PyCFunction)pycairo_save,             METH_NOARGS},
  {"scale",           (PyCFunction)pycairo_scale,            METH_VARARGS},
  {"select_font_face",(PyCFunction)pycairo_select_font_face, METH_VARARGS},
  {"set_antialias",   (PyCFunction)pycairo_set_antialias,    METH_VARARGS},
  {"set_dash",        (PyCFunction)pycairo_set_dash,         METH_VARARGS},
  {"set_fill_rule",   (PyCFunction)pycairo_set_fill_rule,    METH_VARARGS},
  {"set_font_face",   (PyCFunction)pycairo_set_font_face,    METH_O},
  {"set_font_matrix", (PyCFunction)pycairo_set_font_matrix,  METH_VARARGS},
  {"set_font_options",(PyCFunction)pycairo_set_font_options, METH_VARARGS},
  {"set_font_size",   (PyCFunction)pycairo_set_font_size,    METH_VARARGS},
  {"set_line_cap",    (PyCFunction)pycairo_set_line_cap,     METH_VARARGS},
  {"set_line_join",   (PyCFunction)pycairo_set_line_join,    METH_VARARGS},
  {"set_line_width",  (PyCFunction)pycairo_set_line_width,   METH_VARARGS},
  {"set_matrix",      (PyCFunction)pycairo_set_matrix,       METH_VARARGS},
  {"set_miter_limit", (PyCFunction)pycairo_set_miter_limit,  METH_VARARGS},
  {"set_operator",    (PyCFunction)pycairo_set_operator,     METH_VARARGS},
  {"set_scaled_font", (PyCFunction)pycairo_set_scaled_font,  METH_VARARGS},
  {"set_source",      (PyCFunction)pycairo_set_source,       METH_VARARGS},
  {"set_source_rgb",  (PyCFunction)pycairo_set_source_rgb,   METH_VARARGS},
  {"set_source_rgba", (PyCFunction)pycairo_set_source_rgba,  METH_VARARGS},
  {"set_source_surface",(PyCFunction)pycairo_set_source_surface, METH_VARARGS},
  {"set_tolerance",   (PyCFunction)pycairo_set_tolerance,    METH_VARARGS},
  {"show_glyphs",     (PyCFunction)pycairo_show_glyphs,      METH_VARARGS},
  {"show_page",       (PyCFunction)pycairo_show_page,        METH_NOARGS},
  {"show_text",       (PyCFunction)pycairo_show_text,        METH_VARARGS},
  {"stroke",          (PyCFunction)pycairo_stroke,           METH_NOARGS},
  {"stroke_extents",  (PyCFunction)pycairo_stroke_extents,   METH_NOARGS},
  {"stroke_preserve", (PyCFunction)pycairo_stroke_preserve,  METH_NOARGS},
  {"text_extents",    (PyCFunction)pycairo_text_extents,     METH_VARARGS},
  {"text_path",       (PyCFunction)pycairo_text_path,        METH_VARARGS},
  {"transform",       (PyCFunction)pycairo_transform,        METH_VARARGS},
  {"translate",       (PyCFunction)pycairo_translate,        METH_VARARGS},
  {"user_to_device",  (PyCFunction)pycairo_user_to_device,   METH_VARARGS},
  {"user_to_device_distance",(PyCFunction)pycairo_user_to_device_distance,
   METH_VARARGS},
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoContext_Type = {
  PyObject_HEAD_INIT(NULL)
  0,                                  /* ob_size */
  "cairo.Context",                    /* tp_name */
  sizeof(PycairoContext),             /* tp_basicsize */
  0,                                  /* tp_itemsize */
  (destructor)pycairo_dealloc,        /* tp_dealloc */
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
  pycairo_methods,                    /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  0,                                  /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)pycairo_new,               /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};
