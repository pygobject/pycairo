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
#include "pycairo-private.h"

PyObject *
pycairo_matrix_wrap(cairo_matrix_t *matrix)
{
    PyCairoMatrix *self;

    self = PyObject_New(PyCairoMatrix, &PyCairoMatrix_Type);
    if (!self) {
	cairo_matrix_destroy(matrix);
	return NULL;
    }

    self->matrix = matrix;

    return (PyObject *)self;
}

static int
pycairo_matrix_init(PyCairoMatrix *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "a", "b", "c", "d", "tx", "ty", NULL };
    double a = 1.0, b = 0.0, c = 0.0, d = 1.0, tx = 0.0, ty = 0.0;

    self->matrix = cairo_matrix_create();
    if (!self->matrix) {
	PyErr_SetString(PyExc_RuntimeError, "could not create matrix");
	return -1;
    }

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "|dddddd:Matrix.__init__", kwlist,
				     &a, &b, &c, &d, &tx, &ty))
	return -1;

    cairo_matrix_set_affine(self->matrix, a, b, c, d, tx, ty);

    return 0;
}

static void
pycairo_matrix_dealloc(PyCairoMatrix *self)
{
    if (self->matrix)
	cairo_matrix_destroy(self->matrix);
    self->matrix = NULL;

    if (self->ob_type->tp_free)
	self->ob_type->tp_free((PyObject *)self);
    else
	PyObject_Del(self);
}

static PyObject *
pycairo_matrix_repr(PyCairoMatrix *self)
{
    char buf[256];
    double a, b, c, d, tx, ty;

    cairo_matrix_get_affine(self->matrix, &a, &b, &c, &d, &tx, &ty);
    PyOS_snprintf(buf, sizeof(buf), "cairo.Matrix(%g, %g, %g, %g, %g, %g)",
		  a, b, c, d, tx, ty);
    return PyString_FromString(buf);
}

static PyObject *
pycairo_matrix_richcmp(PyCairoMatrix *self, PyCairoMatrix *other, int op)
{
    double a1, b1, c1, d1, tx1, ty1;
    double a2, b2, c2, d2, tx2, ty2;
    int equal;
    PyObject *ret;

    if (!PyObject_TypeCheck(other, &PyCairoMatrix_Type) ||
	!(op == Py_EQ || op == Py_NE)) {
	Py_INCREF(Py_NotImplemented);
	return Py_NotImplemented;
    }

    cairo_matrix_get_affine(self->matrix, &a1, &b1, &c1, &d1, &tx1, &ty1);
    cairo_matrix_get_affine(other->matrix, &a2, &b2, &c2, &d2, &tx2, &ty2);
    equal = a1 == a2 &&	b1 == b2 && c1 == c2 && d1 == d2 &&
	tx1 == tx2 && ty1 == ty2;

    if (op == Py_EQ)
	ret = equal ? Py_True : Py_False;
    else
	ret = equal ? Py_False : Py_True;
    Py_INCREF(ret);
    return ret;
}

static PyObject *
pycairo_matrix_multiply(PyCairoMatrix *self, PyCairoMatrix *other)
{
    cairo_matrix_t *result;

    result = cairo_matrix_create();
    if (!result)
	return PyErr_NoMemory();

    cairo_matrix_multiply(result, self->matrix, other->matrix);
    return pycairo_matrix_wrap(result);
}

static PyNumberMethods pycairo_matrix_as_number = {
  (binaryfunc)0,
  (binaryfunc)0,
  (binaryfunc)pycairo_matrix_multiply,
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
pycairo_matrix_translate(PyCairoMatrix *self, PyObject *args)
{
    double tx, ty;
    cairo_matrix_t *other;

    if (!PyArg_ParseTuple(args, "dd:Matrix.translate", &tx, &ty))
	return NULL;
    
    other = cairo_matrix_create();
    if (!other)
	return PyErr_NoMemory();

    cairo_matrix_copy(other, self->matrix);
    cairo_matrix_translate(other, tx, ty);
    return pycairo_matrix_wrap(other);
}

static PyObject *
pycairo_matrix_scale(PyCairoMatrix *self, PyObject *args)
{
    double sx, sy;
    cairo_matrix_t *other;

    if (!PyArg_ParseTuple(args, "dd:Matrix.scale", &sx, &sy))
	return NULL;
    
    other = cairo_matrix_create();
    if (!other)
	return PyErr_NoMemory();

    cairo_matrix_copy(other, self->matrix);
    cairo_matrix_scale(other, sx, sy);
    return pycairo_matrix_wrap(other);
}

static PyObject *
pycairo_matrix_rotate(PyCairoMatrix *self, PyObject *args)
{
    double radians;
    cairo_matrix_t *other;

    if (!PyArg_ParseTuple(args, "d:Matrix.rotate", &radians))
	return NULL;
    
    other = cairo_matrix_create();
    if (!other)
	return PyErr_NoMemory();

    cairo_matrix_copy(other, self->matrix);
    cairo_matrix_rotate(other, radians);
    return pycairo_matrix_wrap(other);
}

static PyObject *
pycairo_matrix_invert(PyCairoMatrix *self)
{
    cairo_matrix_t *other;

    other = cairo_matrix_create();
    if (!other)
	return PyErr_NoMemory();

    cairo_matrix_copy(other, self->matrix);
    if (pycairo_check_status(cairo_matrix_invert(other))) {
	cairo_matrix_destroy(other);
	return NULL;
    }
    return pycairo_matrix_wrap(other);
}

static PyObject *
pycairo_matrix_transform_distance(PyCairoMatrix *self, PyObject *args)
{
    double dx, dy;

    if (!PyArg_ParseTuple(args, "dd:Matrix.transform_distance", &dx, &dy))
	return NULL;

    cairo_matrix_transform_distance(self->matrix, &dx, &dy);
    return Py_BuildValue("(dd)", dx, dy);
}

static PyObject *
pycairo_matrix_transform_point(PyCairoMatrix *self, PyObject *args)
{
    double x, y;

    if (!PyArg_ParseTuple(args, "dd:Matrix.transform_point", &x, &y))
	return NULL;

    cairo_matrix_transform_point(self->matrix, &x, &y);
    return Py_BuildValue("(dd)", x, y);
}

static PyMethodDef pycairo_matrix_methods[] = {
    { "translate", (PyCFunction)pycairo_matrix_translate, METH_VARARGS },
    { "scale", (PyCFunction)pycairo_matrix_scale, METH_VARARGS },
    { "rotate", (PyCFunction)pycairo_matrix_rotate, METH_VARARGS },
    { "invert", (PyCFunction)pycairo_matrix_invert, METH_VARARGS },
    { "transform_distance", (PyCFunction)pycairo_matrix_transform_distance,
      METH_VARARGS },
    { "transform_point", (PyCFunction)pycairo_matrix_transform_point,
      METH_VARARGS },
    { NULL, NULL, 0 }
};

PyTypeObject PyCairoMatrix_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Matrix",                     /* tp_name */
    sizeof(PyCairoMatrix),              /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)pycairo_matrix_dealloc, /* tp_dealloc */
    (printfunc)0,                       /* tp_print */
    (getattrfunc)0,                     /* tp_getattr */
    (setattrfunc)0,                     /* tp_setattr */
    (cmpfunc)0,                         /* tp_compare */
    (reprfunc)pycairo_matrix_repr,      /* tp_repr */
    &pycairo_matrix_as_number,          /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    (hashfunc)0,                        /* tp_hash */
    (ternaryfunc)0,                     /* tp_call */
    (reprfunc)0,                        /* tp_str */
    (getattrofunc)0,                    /* tp_getattro */
    (setattrofunc)0,                    /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    NULL, /* Documentation string */
    (traverseproc)0,                    /* tp_traverse */
    (inquiry)0,                         /* tp_clear */
    (richcmpfunc)pycairo_matrix_richcmp, /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    (getiterfunc)0,                     /* tp_iter */
    (iternextfunc)0,                    /* tp_iternext */
    pycairo_matrix_methods,             /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)pycairo_matrix_init,      /* tp_init */
    (allocfunc)0,                       /* tp_alloc */
    (newfunc)0,                         /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
