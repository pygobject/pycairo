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
 *                 Steve Chaplin
 *                 Maarten Breddels
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo-private.h"
#include "pycairo-misc.h"

PyObject *
pycairo_context_wrap(cairo_t *ctx)
{
    PyCairoContext *self;

    self = PyObject_New(PyCairoContext, &PyCairoContext_Type);
    if (self)
	self->ctx = ctx;
    else
	cairo_destroy(ctx);

    return (PyObject *)self;
}

static void
pycairo_dealloc(PyCairoContext *self)
{
    if (self->ctx)
	cairo_destroy(self->ctx);
    self->ctx = NULL;

    if (self->ob_type->tp_free)
	self->ob_type->tp_free((PyObject *)self);
    else
	PyObject_Del(self);
}

static PyObject *
pycairo_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyCairoContext *self = (PyCairoContext *)type->tp_alloc(type, 0);

    if (self) {
	self->ctx = cairo_create();
	if (!self->ctx) {
	    PyErr_SetString(PyExc_RuntimeError, "could not create context");
	    Py_DECREF(self);
	    return NULL;
	}
	if (pycairo_check_status(cairo_status(self->ctx))) {
	    Py_DECREF(self);
	    return NULL;
	}
    }
    return (PyObject *)self;
}

/* pycairo_init - not used */

static PyObject *
pycairo_copy(PyCairoContext *self, PyObject *args)
{
    PyCairoContext *src;

    if (!PyArg_ParseTuple(args, "O!:Context.copy",
			  &PyCairoContext_Type, &src))
	return NULL;

    cairo_copy(self->ctx, src->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_save(PyCairoContext *self)
{
    cairo_save(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_restore(PyCairoContext *self)
{
    cairo_restore(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_target_surface(PyCairoContext *self, PyObject *args)
{
    PyCairoSurface *surface;

    if (!PyArg_ParseTuple(args, "O!:Context.set_target_surface",
			  &PyCairoSurface_Type, &surface))
	return NULL;

    cairo_set_target_surface(self->ctx, surface->surface);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

#ifdef CAIRO_HAS_PS_SURFACE
static PyObject *
pycairo_set_target_ps(PyCairoContext *self, PyObject *args)
{
    PyObject *file_object;
    double width_inches, height_inches;
    double x_pixels_per_inch, y_pixels_per_inch;

    if (!PyArg_ParseTuple(args, "O!dddd:Context.set_target_ps",
			  &PyFile_Type, &file_object,
			  &width_inches, &height_inches,
			  &x_pixels_per_inch, &y_pixels_per_inch))
	return NULL;

    cairo_set_target_ps(self->ctx, PyFile_AsFile(file_object),
			width_inches, height_inches,
			x_pixels_per_inch, y_pixels_per_inch);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}
#endif  /* CAIRO_HAS_PS_SURFACE */

#ifdef CAIRO_HAS_PDF_SURFACE
static PyObject *
pycairo_set_target_pdf(PyCairoContext *self, PyObject *args)
{
    PyObject *file_object;
    double width_inches, height_inches;
    double x_pixels_per_inch, y_pixels_per_inch;

    if (!PyArg_ParseTuple(args, "O!dddd:Context.set_target_pdf",
			  &PyFile_Type, &file_object,
			  &width_inches, &height_inches,
			  &x_pixels_per_inch, &y_pixels_per_inch))
	return NULL;

    cairo_set_target_pdf(self->ctx, PyFile_AsFile(file_object),
			width_inches, height_inches,
			x_pixels_per_inch, y_pixels_per_inch);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}
#endif  /* CAIRO_HAS_PDF_SURFACE */

#ifdef CAIRO_HAS_PNG_SURFACE
static PyObject *
pycairo_set_target_png(PyCairoContext *self, PyObject *args)
{
    PyObject *file_object;
    cairo_format_t format;
    int width, height;

    if (!PyArg_ParseTuple(args, "O!iii:Context.set_target_png",
			  &PyFile_Type, &file_object, &format, &width,
			  &height))
	return NULL;

    cairo_set_target_png(self->ctx, PyFile_AsFile(file_object), format,
			 width, height);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}
#endif /* CAIRO_HAS_PNG_SURFACE */

static PyObject *
pycairo_set_operator(PyCairoContext *self, PyObject *args)
{
    cairo_operator_t op;

    if (!PyArg_ParseTuple(args, "i:Context.set_operator", &op))
	return NULL;

    cairo_set_operator(self->ctx, op);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_in_stroke(PyCairoContext *self, PyObject *args)
{
    double x, y;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "dd:Context.in_stroke", &x, &y))
	return NULL;

    result = cairo_in_stroke(self->ctx, x, y) ? Py_True : Py_False;
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_INCREF(result);
    return result;
}

static PyObject *
pycairo_in_fill(PyCairoContext *self, PyObject *args)
{
    double x, y;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "dd:Context.in_fill", &x, &y))
	return NULL;

    result = cairo_in_fill(self->ctx, x, y) ? Py_True : Py_False;
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_INCREF(result);
    return result;
}


static PyObject *
pycairo_stroke_extents(PyCairoContext *self, PyObject *args)
{
    double x1, y1, x2, y2;
    cairo_stroke_extents(self->ctx, &x1, &y1, &x2, &y2);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(dddd)", x1, y1, x2, y2);
}

static PyObject *
pycairo_fill_extents(PyCairoContext *self)
{
    double x1, y1, x2, y2;
    cairo_fill_extents(self->ctx, &x1, &y1, &x2, &y2);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(dddd)", x1, y1, x2, y2);
}

static PyObject *
pycairo_init_clip(PyCairoContext *self)
{
    cairo_init_clip(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_text_path(PyCairoContext *self, PyObject *args)
{
    const unsigned char *utf8;

    if (!PyArg_ParseTuple(args, "s:Context.text_path", &utf8))
	return NULL;

    cairo_text_path(self->ctx, utf8);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_rgb_color(PyCairoContext *self, PyObject *args)
{
    double red, green, blue;

    if (!PyArg_ParseTuple(args, "ddd:Context.set_rgb_color",
			  &red, &green, &blue))
	return NULL;

    cairo_set_rgb_color(self->ctx, red, green, blue);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_alpha(PyCairoContext *self, PyObject *args)
{
    double alpha;

    if (!PyArg_ParseTuple(args, "d:Context.set_alpha", &alpha))
	return NULL;

    cairo_set_alpha(self->ctx, alpha);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_pattern(PyCairoContext *self, PyObject *args)
{
    PyCairoPattern *pattern;

    if (!PyArg_ParseTuple(args, "O!:Context.set_pattern",
			  &PyCairoPattern_Type, &pattern))
	return NULL;

    cairo_set_pattern(self->ctx, pattern->pattern);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_tolerance(PyCairoContext *self, PyObject *args)
{
    double tolerance;

    if (!PyArg_ParseTuple(args, "d:Context.set_tolerance", &tolerance))
	return NULL;

    cairo_set_tolerance(self->ctx, tolerance);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_fill_rule(PyCairoContext *self, PyObject *args)
{
    cairo_fill_rule_t fill_rule;

    if (!PyArg_ParseTuple(args, "i:Context.set_fill_rule", &fill_rule))
	return NULL;

    cairo_set_fill_rule(self->ctx, fill_rule);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_line_width(PyCairoContext *self, PyObject *args)
{
    double width;

    if (!PyArg_ParseTuple(args, "d:Context.set_line_width", &width))
	return NULL;

    cairo_set_line_width(self->ctx, width);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_line_cap(PyCairoContext *self, PyObject *args)
{
    cairo_line_cap_t line_cap;

    if (!PyArg_ParseTuple(args, "i:Context.set_line_cap", &line_cap))
	return NULL;

    cairo_set_line_cap(self->ctx, line_cap);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_line_join(PyCairoContext *self, PyObject *args)
{
    cairo_line_join_t line_join;

    if (!PyArg_ParseTuple(args, "i:Context.set_line_join", &line_join))
	return NULL;

    cairo_set_line_join(self->ctx, line_join);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_dash(PyCairoContext *self, PyObject *args)
{
    double *dashes, offset = 0;
    int ndash, i;
    PyObject *py_dashes;

    if (!PyArg_ParseTuple(args, "O|d:Context.set_dash", &py_dashes, &offset))
	return NULL;
    if (!(py_dashes = PySequence_Fast(py_dashes, "first argument must be a sequence")))
	return NULL;

    ndash = PySequence_Fast_GET_SIZE(py_dashes);
    dashes = malloc(ndash * sizeof(double));
    for (i = 0; i < ndash; i++) {
	PyObject *item = PySequence_Fast_GET_ITEM(py_dashes, i);

	dashes[i] = PyFloat_AsDouble(item);
	if (PyErr_Occurred()) {
	    free(dashes);
	    Py_DECREF(py_dashes);
	    return NULL;
	}
    }
    Py_DECREF(py_dashes);

    cairo_set_dash(self->ctx, dashes, ndash, offset);
    free(dashes);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_miter_limit(PyCairoContext *self, PyObject *args)
{
    double limit;

    if (!PyArg_ParseTuple(args, "d:Context.set_miter_limit", &limit))
	return NULL;

    cairo_set_miter_limit(self->ctx, limit);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_translate(PyCairoContext *self, PyObject *args)
{
    double tx, ty;

    if (!PyArg_ParseTuple(args, "dd:Context.translate", &tx, &ty))
	return NULL;

    cairo_translate(self->ctx, tx, ty);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_scale(PyCairoContext *self, PyObject *args)
{
    double sx, sy;

    if (!PyArg_ParseTuple(args, "dd:Context.scale", &sx, &sy))
	return NULL;

    cairo_scale(self->ctx, sx, sy);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_rotate(PyCairoContext *self, PyObject *args)
{
    double angle;

    if (!PyArg_ParseTuple(args, "d:Context.rotate", &angle))
	return NULL;

    cairo_rotate(self->ctx, angle);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_concat_matrix(PyCairoContext *self, PyObject *args)
{
    PyCairoMatrix *matrix;

    if (!PyArg_ParseTuple(args, "O!:Context.concat_matrix",
			  &PyCairoMatrix_Type, &matrix))
	return NULL;

    cairo_concat_matrix(self->ctx, matrix->matrix);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_matrix(PyCairoContext *self, PyObject *args)
{
    PyCairoMatrix *matrix;

    if (!PyArg_ParseTuple(args, "O!:Context.set_matrix",
			   &PyCairoMatrix_Type, &matrix))
	return NULL;

    cairo_set_matrix(self->ctx, matrix->matrix);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_default_matrix(PyCairoContext *self)
{
    cairo_default_matrix(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_identity_matrix(PyCairoContext *self)
{
    cairo_identity_matrix(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_transform_distance(PyCairoContext *self, PyObject *args)
{
    double dx, dy;

    if (!PyArg_ParseTuple(args, "dd:Context.transform_distance", &dx, &dy))
	return NULL;

    cairo_transform_distance(self->ctx, &dx, &dy);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(dd)", dx, dy);
}

static PyObject *
pycairo_transform_point(PyCairoContext *self, PyObject *args)
{
    double x, y;

    if (!PyArg_ParseTuple(args, "dd:Context.transform_point", &x, &y))
	return NULL;

    cairo_transform_point(self->ctx, &x, &y);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(dd)", x, y);
}

static PyObject *
pycairo_inverse_transform_distance(PyCairoContext *self, PyObject *args)
{
    double dx, dy;

    if (!PyArg_ParseTuple(args, "dd:Context.inverse_transform_distance",
			  &dx, &dy))
	return NULL;

    cairo_inverse_transform_distance(self->ctx, &dx, &dy);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(dd)", dx, dy);
}

static PyObject *
pycairo_inverse_transform_point(PyCairoContext *self, PyObject *args)
{
    double x, y;

    if (!PyArg_ParseTuple(args, "dd:Context.inverse_transform_point", &x, &y))
	return NULL;

    cairo_inverse_transform_point(self->ctx, &x, &y);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(dd)", x, y);
}

static PyObject *
pycairo_new_path(PyCairoContext *self)
{
    cairo_new_path(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_move_to(PyCairoContext *self, PyObject *args)
{
    double x, y;

    if (!PyArg_ParseTuple(args, "dd:Context.move_to", &x, &y))
	return NULL;

    cairo_move_to(self->ctx, x, y);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_line_to(PyCairoContext *self, PyObject *args)
{
    double x, y;

    if (!PyArg_ParseTuple(args, "dd:Context.line_to", &x, &y))
	return NULL;

    cairo_line_to(self->ctx, x, y);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_curve_to(PyCairoContext *self, PyObject *args)
{
    double x1, y1, x2, y2, x3, y3;

    if (!PyArg_ParseTuple(args, "dddddd:Context.curve_to",
			  &x1, &y1, &x2, &y2, &x3, &y3))
	return NULL;

    cairo_curve_to(self->ctx, x1, y1, x2, y2, x3, y3);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_arc(PyCairoContext *self, PyObject *args)
{
    double xc, yc, radius, angle1, angle2;

    if (!PyArg_ParseTuple(args, "ddddd:Context.arc",
			  &xc, &yc, &radius, &angle1, &angle2))
	return NULL;

    cairo_arc(self->ctx, xc, yc, radius, angle1, angle2);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_arc_negative(PyCairoContext *self, PyObject *args)
{
    double xc, yc, radius, angle1, angle2;

    if (!PyArg_ParseTuple(args, "ddddd:Context.arc_negative",
			  &xc, &yc, &radius, &angle1, &angle2))
	return NULL;

    cairo_arc_negative(self->ctx, xc, yc, radius, angle1, angle2);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_rel_move_to(PyCairoContext *self, PyObject *args)
{
    double dx, dy;

    if (!PyArg_ParseTuple(args, "dd:Context.rel_move_to", &dx, &dy))
	return NULL;

    cairo_rel_move_to(self->ctx, dx, dy);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_rel_line_to(PyCairoContext *self, PyObject *args)
{
    double dx, dy;

    if (!PyArg_ParseTuple(args, "dd:Context.rel_line_to", &dx, &dy))
	return NULL;

    cairo_rel_line_to(self->ctx, dx, dy);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_rel_curve_to(PyCairoContext *self, PyObject *args)
{
    double dx1, dy1, dx2, dy2, dx3, dy3;

    if (!PyArg_ParseTuple(args, "dddddd:Context.rel_curve_to",
			  &dx1, &dy1, &dx2, &dy2, &dx3, &dy3))
	return NULL;

    cairo_rel_curve_to(self->ctx, dx1, dy1, dx2, dy2, dx3, dy3);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_rectangle(PyCairoContext *self, PyObject *args)
{
    double x, y, width, height;

    if (!PyArg_ParseTuple(args, "dddd:Context.rectangle",
			  &x, &y, &width, &height))
	return NULL;

    cairo_rectangle(self->ctx, x, y, width, height);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_close_path(PyCairoContext *self)
{
    cairo_close_path(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_stroke(PyCairoContext *self)
{
    cairo_stroke(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_fill(PyCairoContext *self)
{
    cairo_fill(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_copy_page(PyCairoContext *self)
{
    cairo_copy_page(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_show_page(PyCairoContext *self)
{
    cairo_show_page(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_clip(PyCairoContext *self)
{
    cairo_clip(self->ctx);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_select_font(PyCairoContext *self, PyObject *args)
{
    const char *family;
    cairo_font_slant_t slant = CAIRO_FONT_SLANT_NORMAL;
    cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;

    if (!PyArg_ParseTuple(args, "s|ii:Context.select_font",
			  &family, &slant, &weight))
	return NULL;

    cairo_select_font(self->ctx, family, slant, weight);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_scale_font(PyCairoContext *self, PyObject *args)
{
    double scale;

    if (!PyArg_ParseTuple(args, "d:Context.scale_font", &scale))
	return NULL;

    cairo_scale_font(self->ctx, scale);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_transform_font(PyCairoContext *self, PyObject *args)
{
    PyCairoMatrix *matrix;

    if (!PyArg_ParseTuple(args, "O!:Context.transform_font",
			  &PyCairoMatrix_Type, &matrix))
	return NULL;

    cairo_transform_font(self->ctx, matrix->matrix);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_show_text(PyCairoContext *self, PyObject *args)
{
    const unsigned char *utf8;

    if (!PyArg_ParseTuple(args, "s:Context.show_text", &utf8))
	return NULL;

    cairo_show_text(self->ctx, utf8);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_set_font(PyCairoContext *self, PyObject *args)
{
    PyCairoFont *font;

    if (!PyArg_ParseTuple(args, "O!:Context.set_font",
			  &PyCairoFont_Type, &font))
	return NULL;

    cairo_set_font(self->ctx, font->font);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_text_extents(PyCairoContext *self, PyObject *args)
{
    const unsigned char *utf8;
    cairo_text_extents_t extents;

    if (!PyArg_ParseTuple(args, "s:Context.text_extents", &utf8))
	return NULL;

    cairo_text_extents(self->ctx, utf8, &extents);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(dddddd)", extents.x_bearing, extents.y_bearing, extents.width, extents.height, extents.x_advance, extents.y_advance);
}

static PyObject *
pycairo_current_font_extents(PyCairoContext *self)
{
    cairo_font_extents_t extents;

    cairo_current_font_extents(self->ctx, &extents);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    return Py_BuildValue("(ddddd)", extents.ascent, extents.descent, extents.height, extents.max_x_advance, extents.max_y_advance);
}

static PyObject *
pycairo_show_surface(PyCairoContext *self, PyObject *args)
{
    PyCairoSurface *surface;
    int width, height;

    if (!PyArg_ParseTuple(args, "O!ii:Context.show_surface",
			  &PyCairoSurface_Type, &surface, &width, &height))
	return NULL;

    cairo_show_surface(self->ctx, surface->surface, width, height);
    if (pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_current_font(PyCairoContext *self)
{
    cairo_font_t *font;

    font = cairo_current_font(self->ctx);
    if (!font){
	pycairo_check_status(cairo_status(self->ctx));
	return NULL;
    }
    cairo_font_reference(font);
    return pycairo_font_wrap(font);
}

static PyObject *
pycairo_current_operator(PyCairoContext *self)
{
    return PyInt_FromLong(cairo_current_operator(self->ctx));
}

static PyObject *
pycairo_current_rgb_color(PyCairoContext *self)
{
    double red, green, blue;

    cairo_current_rgb_color(self->ctx, &red, &green, &blue);
    return Py_BuildValue("(ddd)", red, green, blue);
}

static PyObject *
pycairo_current_alpha(PyCairoContext *self)
{
    return PyFloat_FromDouble(cairo_current_alpha(self->ctx));
}

static PyObject *
pycairo_current_tolerance(PyCairoContext *self)
{
    return PyFloat_FromDouble(cairo_current_tolerance(self->ctx));
}

static PyObject *
pycairo_current_point(PyCairoContext *self)
{
    double x, y;

    cairo_current_point(self->ctx, &x, &y);
    return Py_BuildValue("(dd)", x, y);
}

static PyObject *
pycairo_current_fill_rule(PyCairoContext *self)
{
    return PyInt_FromLong(cairo_current_fill_rule(self->ctx));
}

static PyObject *
pycairo_current_line_width(PyCairoContext *self)
{
    return PyFloat_FromDouble(cairo_current_line_width(self->ctx));
}

static PyObject *
pycairo_current_line_cap(PyCairoContext *self)
{
    return PyInt_FromLong(cairo_current_line_cap(self->ctx));
}

static PyObject *
pycairo_current_line_join(PyCairoContext *self)
{
    return PyInt_FromLong(cairo_current_line_join(self->ctx));
}

static PyObject *
pycairo_current_miter_limit(PyCairoContext *self)
{
    return PyFloat_FromDouble(cairo_current_miter_limit(self->ctx));
}

static PyObject *
pycairo_current_matrix(PyCairoContext *self)
{
    cairo_matrix_t *matrix;

    matrix = cairo_matrix_create();
    if (!matrix)
	return PyErr_NoMemory();
    cairo_current_matrix(self->ctx, matrix);
    return pycairo_matrix_wrap(matrix);
}

static PyObject *
pycairo_current_target_surface(PyCairoContext *self)
{
    cairo_surface_t *surface;

    surface = cairo_current_target_surface(self->ctx);
    if (!surface)
	Py_RETURN_NONE;

    cairo_surface_reference(surface);
    return pycairo_surface_wrap(surface);
}

static PyObject *
pycairo_current_pattern(PyCairoContext *self)
{
    cairo_pattern_t *pattern;

    pattern = cairo_current_pattern(self->ctx);
    if (pattern)
	return pycairo_pattern_wrap(pattern);
    Py_RETURN_NONE;
}

/* struct and wrappers for cairo_current_path() */ 
typedef struct {
	PyObject* move_to;
	PyObject* line_to;
	PyObject* curve_to;
	PyObject* close_path;
} py_path_callbacks;

static void 
py_wrapper_move_to(void *closure, double x, double y)
{
    if(!PyErr_Occurred()) {
	py_path_callbacks *callbacks = (py_path_callbacks *)closure;
	PyObject *arglist, *result = NULL;
	arglist = Py_BuildValue("(dd)", x, y);
	result = PyEval_CallObject(callbacks->move_to, arglist);
	Py_DECREF(arglist);
	Py_XDECREF(result);
    }
}

static void 
py_wrapper_line_to(void *closure, double x, double y)
{
    if(!PyErr_Occurred()) {
	py_path_callbacks *callbacks = (py_path_callbacks *)closure;
	PyObject *arglist, *result = NULL;
	arglist = Py_BuildValue("(dd)", x, y);
	result = PyEval_CallObject(callbacks->line_to, arglist);
	Py_DECREF(arglist);
	Py_XDECREF(result);
    }
}

static void 
py_wrapper_curve_to(void *closure, double x1, double y1, double x2, double y2, double x3, double y3)
{
    if(!PyErr_Occurred()) {
	py_path_callbacks *callbacks = (py_path_callbacks *)closure;
	PyObject *arglist, *result = NULL;
	arglist = Py_BuildValue("(dddddd)", x1, y1, x2, y2, x3, y3);
	result = PyEval_CallObject(callbacks->curve_to, arglist);
	Py_DECREF(arglist);
	Py_XDECREF(result);
    }
}

static void 
py_wrapper_close_path(void *closure)
{
    if(!PyErr_Occurred()) {
	py_path_callbacks *callbacks = (py_path_callbacks *)closure;
	PyObject *arglist, *result = NULL;
    	arglist = Py_BuildValue("()");
	result = PyEval_CallObject(callbacks->close_path, arglist);
	Py_DECREF(arglist);
	Py_XDECREF(result);
    }
}

static PyObject *
pycairo_current_path(PyCairoContext *self, PyObject *args)
{
    py_path_callbacks callbacks;

    if(!PyArg_ParseTuple(args, "OOOO:Context.current_path_flat",
			  &callbacks.move_to, &callbacks.line_to, &callbacks.curve_to, &callbacks.close_path))
	return NULL;

    if(!PyCallable_Check(callbacks.move_to)) {
	PyErr_SetString(PyExc_TypeError, "move_to must be callable");
	return NULL;
    }
    if(!PyCallable_Check(callbacks.line_to)) {
	PyErr_SetString(PyExc_TypeError, "line_to must be callable");
	return NULL;
    }
    if(!PyCallable_Check(callbacks.curve_to)) {
	PyErr_SetString(PyExc_TypeError, "curve_to must be callable");
	return NULL;
    }
    if(!PyCallable_Check(callbacks.close_path)) {
	PyErr_SetString(PyExc_TypeError, "close_path must be callable");
	return NULL;
    }

    cairo_current_path(self->ctx, py_wrapper_move_to, py_wrapper_line_to, py_wrapper_curve_to, py_wrapper_close_path, &callbacks);
    if(PyErr_Occurred() || pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
pycairo_current_path_flat(PyCairoContext *self, PyObject *args)
{
    py_path_callbacks callbacks;

    if(!PyArg_ParseTuple(args, "OOO:Context.current_path_flat",
			  &callbacks.move_to, &callbacks.line_to, &callbacks.close_path))
	return NULL;

    if(!PyCallable_Check(callbacks.move_to)) {
	PyErr_SetString(PyExc_TypeError, "move_to must be callable");
	return NULL;
    }
    if(!PyCallable_Check(callbacks.line_to)) {
	PyErr_SetString(PyExc_TypeError, "line_to must be callable");
	return NULL;
    }
    if(!PyCallable_Check(callbacks.close_path)) {
	PyErr_SetString(PyExc_TypeError, "close_path must be callable");
	return NULL;
    }

    cairo_current_path_flat(self->ctx, py_wrapper_move_to, py_wrapper_line_to, py_wrapper_close_path, &callbacks);
    if(PyErr_Occurred() || pycairo_check_status(cairo_status(self->ctx)))
	return NULL;
    Py_RETURN_NONE;
}

static PyMethodDef pycairo_methods[] = {
    { "arc", (PyCFunction)pycairo_arc, METH_VARARGS },
    { "arc_negative", (PyCFunction)pycairo_arc_negative, METH_VARARGS },
    { "clip", (PyCFunction)pycairo_clip, METH_NOARGS },
    { "close_path", (PyCFunction)pycairo_close_path, METH_NOARGS },
    { "concat_matrix", (PyCFunction)pycairo_concat_matrix, METH_VARARGS },
    { "copy", (PyCFunction)pycairo_copy, METH_VARARGS },
    { "copy_page", (PyCFunction)pycairo_copy_page, METH_NOARGS },
    { "current_path", (PyCFunction)pycairo_current_path, METH_VARARGS },
    { "current_path_flat", (PyCFunction)pycairo_current_path_flat, METH_VARARGS },
    { "curve_to", (PyCFunction)pycairo_curve_to, METH_VARARGS },
    { "default_matrix", (PyCFunction)pycairo_default_matrix, METH_NOARGS },
    { "fill", (PyCFunction)pycairo_fill, METH_NOARGS },
    /* TODO: { "glyph_extents", (PyCFunction)pycairo_glyph_extents, METH_NOARGS }, */
    /* TODO: { "glyph_path", (PyCFunction)pycairo_glyph_path, METH_VARARGS }, */
    { "identity_matrix", (PyCFunction)pycairo_identity_matrix, METH_NOARGS },
    { "in_fill", (PyCFunction)pycairo_in_fill, METH_VARARGS },
    { "in_stroke", (PyCFunction)pycairo_in_stroke, METH_VARARGS },
    { "init_clip", (PyCFunction)pycairo_init_clip, METH_NOARGS },
    { "inverse_transform_distance",
      (PyCFunction)pycairo_inverse_transform_distance, METH_VARARGS },
    { "inverse_transform_point", (PyCFunction)pycairo_inverse_transform_point,
      METH_VARARGS },
    { "line_to", (PyCFunction)pycairo_line_to, METH_VARARGS },
    { "move_to", (PyCFunction)pycairo_move_to, METH_VARARGS },
    { "new_path", (PyCFunction)pycairo_new_path, METH_NOARGS },
    { "rectangle", (PyCFunction)pycairo_rectangle, METH_VARARGS },
    { "rel_curve_to", (PyCFunction)pycairo_rel_curve_to, METH_VARARGS },
    { "rel_line_to", (PyCFunction)pycairo_rel_line_to, METH_VARARGS },
    { "rel_move_to", (PyCFunction)pycairo_rel_move_to, METH_VARARGS },
    { "restore", (PyCFunction)pycairo_restore, METH_NOARGS },
    { "rotate", (PyCFunction)pycairo_rotate, METH_VARARGS },
    { "save", (PyCFunction)pycairo_save, METH_NOARGS },
    { "scale", (PyCFunction)pycairo_scale, METH_VARARGS },
    { "scale_font", (PyCFunction)pycairo_scale_font, METH_VARARGS },
    { "select_font", (PyCFunction)pycairo_select_font, METH_VARARGS },
    { "set_alpha", (PyCFunction)pycairo_set_alpha, METH_VARARGS },
    { "set_dash", (PyCFunction)pycairo_set_dash, METH_VARARGS },
    { "set_fill_rule", (PyCFunction)pycairo_set_fill_rule, METH_VARARGS },
    { "set_font", (PyCFunction)pycairo_set_font, METH_VARARGS },
    { "set_line_cap", (PyCFunction)pycairo_set_line_cap, METH_VARARGS },
    { "set_line_join", (PyCFunction)pycairo_set_line_join, METH_VARARGS },
    { "set_line_width", (PyCFunction)pycairo_set_line_width, METH_VARARGS },
    { "set_matrix", (PyCFunction)pycairo_set_matrix, METH_VARARGS },
    { "set_miter_limit", (PyCFunction)pycairo_set_miter_limit, METH_VARARGS },
    { "set_operator", (PyCFunction)pycairo_set_operator, METH_VARARGS },
    { "set_pattern", (PyCFunction)pycairo_set_pattern, METH_VARARGS },
    { "set_rgb_color", (PyCFunction)pycairo_set_rgb_color, METH_VARARGS },
#ifdef CAIRO_HAS_PDF_SURFACE
    { "set_target_pdf", (PyCFunction)pycairo_set_target_pdf, METH_VARARGS},
#endif
#ifdef CAIRO_HAS_PS_SURFACE
    { "set_target_ps", (PyCFunction)pycairo_set_target_ps, METH_VARARGS},
#endif
#ifdef CAIRO_HAS_PNG_SURFACE
    { "set_target_png", (PyCFunction)pycairo_set_target_png, METH_VARARGS },
#endif
    { "set_target_surface", (PyCFunction)pycairo_set_target_surface,
      METH_VARARGS },
    { "set_tolerance", (PyCFunction)pycairo_set_tolerance, METH_VARARGS },
    /* TODO: { "show_glyphs", (PyCFunction)pycairo_show_glyphs, METH_NOARGS }, */
    { "show_page", (PyCFunction)pycairo_show_page, METH_NOARGS },
    { "show_surface", (PyCFunction)pycairo_show_surface, METH_VARARGS },
    { "show_text", (PyCFunction)pycairo_show_text, METH_VARARGS },
    { "stroke", (PyCFunction)pycairo_stroke, METH_NOARGS },
    { "text_extents", (PyCFunction)pycairo_text_extents, METH_VARARGS },
    { "text_path", (PyCFunction)pycairo_text_path, METH_VARARGS },
    { "transform_font", (PyCFunction)pycairo_transform_font, METH_VARARGS },
    { "transform_point", (PyCFunction)pycairo_transform_point, METH_VARARGS },
    { "transform_distance", (PyCFunction)pycairo_transform_distance,
      METH_VARARGS },
    { "translate", (PyCFunction)pycairo_translate, METH_VARARGS },
    { NULL, NULL, 0 }
};

static PyGetSetDef pycairo_getsets[] = {
    { "alpha",          (getter)pycairo_current_alpha,        (setter)0 },
    { "fill_extents",   (getter)pycairo_fill_extents,         (setter)0 },
    { "fill_rule",      (getter)pycairo_current_fill_rule,    (setter)0 },
    { "font",           (getter)pycairo_current_font,         (setter)0 },
    { "font_extents",   (getter)pycairo_current_font_extents, (setter)0 },
    { "line_cap",       (getter)pycairo_current_line_cap,     (setter)0 },
    { "line_join",      (getter)pycairo_current_line_join,    (setter)0 },
    { "line_width",     (getter)pycairo_current_line_width,   (setter)0 },
    { "matrix",         (getter)pycairo_current_matrix,       (setter)0 },
    { "miter_limit",    (getter)pycairo_current_miter_limit,  (setter)0 },
    { "operator",       (getter)pycairo_current_operator,     (setter)0 },
    { "pattern",        (getter)pycairo_current_pattern,      (setter)0 },
    { "point",          (getter)pycairo_current_point,        (setter)0 },
    { "rgb_color",      (getter)pycairo_current_rgb_color,    (setter)0 },
    { "stroke_extents", (getter)pycairo_stroke_extents,       (setter)0 },
    { "target_surface", (getter)pycairo_current_target_surface, (setter)0 },
    { "tolerance",      (getter)pycairo_current_tolerance,    (setter)0 },
    { NULL, (getter)0, (setter)0 }
};

PyTypeObject PyCairoContext_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Context",                    /* tp_name */
    sizeof(PyCairoContext),             /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)pycairo_dealloc,        /* tp_dealloc */
    (printfunc)0,                       /* tp_print */
    (getattrfunc)0,                     /* tp_getattr */
    (setattrfunc)0,                     /* tp_setattr */
    (cmpfunc)0,                         /* tp_compare */
    (reprfunc)0,                        /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    (hashfunc)0,                        /* tp_hash */
    (ternaryfunc)0,                     /* tp_call */
    (reprfunc)0,                        /* tp_str */
    (getattrofunc)0,                    /* tp_getattro */
    (setattrofunc)0,                    /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    NULL,                               /* tp_doc */
    (traverseproc)0,                    /* tp_traverse */
    (inquiry)0,                         /* tp_clear */
    (richcmpfunc)0,                     /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    (getiterfunc)0,                     /* tp_iter */
    (iternextfunc)0,                    /* tp_iternext */
    pycairo_methods,                    /* tp_methods */
    0,                                  /* tp_members */
    pycairo_getsets,                    /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)0,                        /* tp_init */
    (allocfunc)0,                       /* tp_alloc */
    (newfunc)pycairo_new,               /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
