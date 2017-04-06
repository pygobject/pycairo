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


/* PycairoMatrix_FromMatrix
 * Create a new PycairoMatrix from a cairo_matrix_t
 * matrix - a cairo_matrix_t to 'wrap' into a Python object
 * Return value: New reference or NULL on failure
 *
 * takes a copy of cairo_matrix_t
 */
PyObject *
PycairoMatrix_FromMatrix (const cairo_matrix_t *matrix)
{
    PyObject *o;

    assert (matrix != NULL);
    o = PycairoMatrix_Type.tp_alloc (&PycairoMatrix_Type, 0);
    if (o)
	((PycairoMatrix *)o)->matrix = *matrix;
    return o;
}

static void
matrix_dealloc (PycairoMatrix *o)
{
    o->ob_type->tp_free((PyObject *)o);
}

static PyObject *
matrix_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *o;
    static char *kwlist[] = { "xx", "yx", "xy", "yy", "x0", "y0", NULL };
    double xx = 1.0, yx = 0.0, xy = 0.0, yy = 1.0, x0 = 0.0, y0 = 0.0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds,
				     "|dddddd:Matrix.__init__", kwlist,
				     &xx, &yx, &xy, &yy, &x0, &y0))
	return NULL;

    o = type->tp_alloc(type, 0);
    if (o)
	cairo_matrix_init (&((PycairoMatrix *)o)->matrix,
			   xx, yx, xy, yy, x0, y0);
    return o;
}

static PyObject *
matrix_init_rotate (PyTypeObject *type, PyObject *args)
{
    cairo_matrix_t matrix;
    double radians;

    if (!PyArg_ParseTuple(args, "d:Matrix.init_rotate", &radians))
	return NULL;

    cairo_matrix_init_rotate (&matrix, radians);
    return PycairoMatrix_FromMatrix (&matrix);
}

static PyObject *
matrix_get_xx (PycairoMatrix *o)
{
    return Py_BuildValue("d", o->matrix.xx);
}

static PyObject *
matrix_get_yx (PycairoMatrix *o)
{
    return Py_BuildValue("d", o->matrix.yx);
}

static PyObject *
matrix_get_xy (PycairoMatrix *o)
{
    return Py_BuildValue("d", o->matrix.xy);
}

static PyObject *
matrix_get_yy (PycairoMatrix *o)
{
    return Py_BuildValue("d", o->matrix.yy);
}

static PyObject *
matrix_get_x0 (PycairoMatrix *o)
{
    return Py_BuildValue("d", o->matrix.x0);
}

static PyObject *
matrix_get_y0 (PycairoMatrix *o)
{
    return Py_BuildValue("d", o->matrix.y0);
}

/* return cairo_matrix_t data as a 6-tuple */
static PyObject *
matrix_get_value (PycairoMatrix *o)
{
    return Py_BuildValue("(dddddd)",
			 o->matrix.xx, o->matrix.yx,
			 o->matrix.xy, o->matrix.yy,
			 o->matrix.x0, o->matrix.y0);
}

static PyObject *
matrix_invert (PycairoMatrix *o)
{
    if (Pycairo_Check_Status (cairo_matrix_invert (&o->matrix)))
	return NULL;
    Py_RETURN_NONE;
}

static PyObject *
matrix_multiply (PycairoMatrix *o, PycairoMatrix *o2)
{
    cairo_matrix_t result;
    cairo_matrix_multiply (&result, &o->matrix, &o2->matrix);
    return PycairoMatrix_FromMatrix (&result);
}

static PyNumberMethods matrix_as_number = {
  (binaryfunc)0,
  (binaryfunc)0,
  (binaryfunc)matrix_multiply,
  (binaryfunc)0,
  (binaryfunc)0,
  (binaryfunc)0,
  (ternaryfunc)0,
  (unaryfunc)0,
  (unaryfunc)0,
  (unaryfunc)0,
  (inquiry)0,
  (unaryfunc)0,
  (binaryfunc)0,
  (binaryfunc)0,
  (binaryfunc)0,
  (binaryfunc)0,
  (binaryfunc)0,
  (coercion)0,
  (unaryfunc)0,
  (unaryfunc)0,
  (unaryfunc)0,
  (unaryfunc)0,
  (unaryfunc)0
};

static PyObject *
matrix_repr (PycairoMatrix *o)
{
    char buf[256];

    PyOS_snprintf(buf, sizeof(buf), "cairo.Matrix(%g, %g, %g, %g, %g, %g)",
		  o->matrix.xx, o->matrix.yx,
		  o->matrix.xy, o->matrix.yy,
		  o->matrix.x0, o->matrix.y0);
    return PyString_FromString(buf);
}

static PyObject *
matrix_richcmp (PycairoMatrix *m1, PycairoMatrix *m2, int op)
{
    int equal;
    PyObject *ret;
    cairo_matrix_t *mx1 = &m1->matrix;
    cairo_matrix_t *mx2 = &m2->matrix;

    if (!PyObject_TypeCheck(m2, &PycairoMatrix_Type) ||
	!(op == Py_EQ || op == Py_NE)) {
	Py_INCREF(Py_NotImplemented);
	return Py_NotImplemented;
    }

    equal = mx1->xx == mx2->xx && mx1->yx == mx2->yx &&
	mx1->xy == mx2->xy && mx1->yy == mx2->yy &&
	mx1->x0 == mx2->x0 && mx1->y0 == mx2->y0;

    if (op == Py_EQ)
	ret = equal ? Py_True : Py_False;
    else
	ret = equal ? Py_False : Py_True;
    Py_INCREF(ret);
    return ret;
}

static PyObject *
matrix_rotate (PycairoMatrix *o, PyObject *args)
{
    double radians;

    if (!PyArg_ParseTuple(args, "d:Matrix.rotate", &radians))
	return NULL;

    cairo_matrix_rotate (&o->matrix, radians);
    Py_RETURN_NONE;
}

static PyObject *
matrix_scale (PycairoMatrix *o, PyObject *args)
{
    double sx, sy;

    if (!PyArg_ParseTuple(args, "dd:Matrix.scale", &sx, &sy))
	return NULL;

    cairo_matrix_scale (&o->matrix, sx, sy);
    Py_RETURN_NONE;
}

static PyObject *
matrix_translate (PycairoMatrix *o, PyObject *args)
{
    double tx, ty;

    if (!PyArg_ParseTuple(args, "dd:Matrix.translate", &tx, &ty))
	return NULL;

    cairo_matrix_translate (&o->matrix, tx, ty);
    Py_RETURN_NONE;
}

static PyObject *
matrix_transform_distance (PycairoMatrix *o, PyObject *args)
{
    double dx, dy;

    if (!PyArg_ParseTuple(args, "dd:Matrix.transform_distance", &dx, &dy))
	return NULL;

    cairo_matrix_transform_distance (&o->matrix, &dx, &dy);
    return Py_BuildValue("(dd)", dx, dy);
}

static PyObject *
matrix_transform_point (PycairoMatrix *o, PyObject *args)
{
    double x, y;

    if (!PyArg_ParseTuple(args, "dd:Matrix.transform_point", &x, &y))
	return NULL;

    cairo_matrix_transform_point (&o->matrix, &x, &y);
    return Py_BuildValue("(dd)", x, y);
}


static PyMethodDef matrix_methods[] = {
    /* Do not need to wrap all cairo_matrix_init_*() functions
     * C API Matrix constructors       Python equivalents
     * cairo_matrix_init()             cairo.Matrix(xx,yx,xy,yy,x0,y0)
     * cairo_matrix_init_identity()    cairo.Matrix()
     * cairo_matrix_init_translate()   cairo.Matrix(x0=x0,y0=y0)
     * cairo_matrix_init_scale()       cairo.Matrix(xx=xx,yy=yy)
     * cairo_matrix_init_rotate()      cairo.Matrix.init_rotate(radians)
     */
    {"init_rotate", (PyCFunction)matrix_init_rotate,
                                                   METH_VARARGS | METH_CLASS },
    {"invert",      (PyCFunction)matrix_invert,                METH_NOARGS },
    {"rotate",      (PyCFunction)matrix_rotate,                METH_VARARGS },
    {"scale",       (PyCFunction)matrix_scale,                 METH_VARARGS },
    {"transform_distance",(PyCFunction)matrix_transform_distance,
                                                               METH_VARARGS },
    {"transform_point", (PyCFunction)matrix_transform_point,   METH_VARARGS },
    {"translate",   (PyCFunction)matrix_translate,             METH_VARARGS },
    {NULL, NULL, 0, NULL},
};

static PyGetSetDef matrix_getsets[] = {
    {"xx",   (getter)matrix_get_xx},
    {"yx",   (getter)matrix_get_yx},
    {"xy",   (getter)matrix_get_xy},
    {"yy",   (getter)matrix_get_yy},
    {"x0",   (getter)matrix_get_x0},
    {"y0",   (getter)matrix_get_y0},
    {"value",(getter)matrix_get_value},
    {NULL, (getter)0, (setter)0, NULL, NULL},
};

PyTypeObject PycairoMatrix_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Matrix",                     /* tp_name */
    sizeof(PycairoMatrix),              /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)matrix_dealloc,         /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    (reprfunc)matrix_repr,              /* tp_repr */
    &matrix_as_number,                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    NULL,                               /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    (richcmpfunc)matrix_richcmp,        /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    matrix_methods,                     /* tp_methods */
    0,                                  /* tp_members */
    matrix_getsets,                     /* tp_getset */
    0, /* &PyBaseObject_Type, */        /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)matrix_new,                /* tp_new */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
};
