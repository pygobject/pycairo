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
#include "structmember.h"

#include "private.h"


PyObject *
PycairoMatrix_FromMatrix (const cairo_matrix_t *matrix) {
  PyObject *o;
  assert (matrix != NULL);
  o = PycairoMatrix_Type.tp_alloc (&PycairoMatrix_Type, 0);
  if (o != NULL)
    ((PycairoMatrix *)o)->matrix = *matrix; /* copy struct */
  return o;
}

static void
matrix_dealloc (PycairoMatrix *o) {
  Py_TYPE(o)->tp_free(o);
}

static PyObject *
matrix_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  cairo_matrix_t mx;
  static char *kwlist[] = { "xx", "yx", "xy", "yy", "x0", "y0", NULL };
  double xx = 1.0, yx = 0.0, xy = 0.0, yy = 1.0, x0 = 0.0, y0 = 0.0;

  if (!PyArg_ParseTupleAndKeywords(args, kwds,
				   "|dddddd:Matrix.__init__", kwlist,
				   &xx, &yx, &xy, &yy, &x0, &y0))
    return NULL;

  cairo_matrix_init (&mx, xx, yx, xy, yy, x0, y0);
  return PycairoMatrix_FromMatrix (&mx);
}

static PyObject *
matrix_init_rotate (PyTypeObject *type, PyObject *args) {
  cairo_matrix_t matrix;
  double radians;

  if (!PyArg_ParseTuple(args, "d:Matrix.init_rotate", &radians))
    return NULL;

  cairo_matrix_init_rotate (&matrix, radians);
  return PycairoMatrix_FromMatrix (&matrix);
}

static PyObject *
matrix_invert (PycairoMatrix *o, PyObject *ignored) {
  if (Pycairo_Check_Status (cairo_matrix_invert (&o->matrix)))
    return NULL;
  Py_RETURN_NONE;
}

/* cairo_matrix_multiply */
static PyObject *
matrix_multiply (PycairoMatrix *o, PyObject *args) {
  PycairoMatrix *mx2;
  cairo_matrix_t result;

  if (!PyArg_ParseTuple(args, "O!:Matrix.multiply",
			&PycairoMatrix_Type, &mx2))
    return NULL;

  cairo_matrix_multiply (&result, &o->matrix, &mx2->matrix);
  return PycairoMatrix_FromMatrix (&result);
}

/* standard matrix multiply, for use by '*' operator */
static PyObject *
matrix_operator_multiply (PycairoMatrix *o, PycairoMatrix *o2) {
  cairo_matrix_t result;
  if (PyObject_IsInstance((PyObject *)o2, (PyObject *)&PycairoMatrix_Type) <= 0) {
    PyErr_SetString(PyExc_TypeError, "matrix can only multiply another matrix");
    return NULL;
  }
  cairo_matrix_multiply (&result, &o->matrix, &o2->matrix);
  return PycairoMatrix_FromMatrix (&result);
}

static PyObject *
matrix_repr (PycairoMatrix *o) {
  char buf[256];

  PyOS_snprintf(buf, sizeof(buf), "cairo.Matrix(%g, %g, %g, %g, %g, %g)",
		o->matrix.xx, o->matrix.yx,
		o->matrix.xy, o->matrix.yy,
		o->matrix.x0, o->matrix.y0);
  return PYCAIRO_PyUnicode_FromString(buf);
}

static PyObject *
matrix_richcmp (PycairoMatrix *m1, PycairoMatrix *m2, int op) {
  int equal;
  PyObject *ret;
  cairo_matrix_t *mx1 = &m1->matrix;
  cairo_matrix_t *mx2 = &m2->matrix;

  if (op != Py_EQ && op != Py_NE) {
    PyErr_SetString(PyExc_TypeError, "Only support testing for == or !=");
    return NULL;
  }

  if (!PyObject_TypeCheck(m2, &PycairoMatrix_Type)) {
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
matrix_rotate (PycairoMatrix *o, PyObject *args) {
  double radians;

  if (!PyArg_ParseTuple(args, "d:Matrix.rotate", &radians))
    return NULL;

  cairo_matrix_rotate (&o->matrix, radians);
  Py_RETURN_NONE;
}

static PyObject *
matrix_scale (PycairoMatrix *o, PyObject *args) {
  double sx, sy;

  if (!PyArg_ParseTuple(args, "dd:Matrix.scale", &sx, &sy))
    return NULL;

  cairo_matrix_scale (&o->matrix, sx, sy);
  Py_RETURN_NONE;
}

static PyObject *
matrix_transform_distance (PycairoMatrix *o, PyObject *args) {
  double dx, dy;

  if (!PyArg_ParseTuple(args, "dd:Matrix.transform_distance", &dx, &dy))
    return NULL;

  cairo_matrix_transform_distance (&o->matrix, &dx, &dy);
  return Py_BuildValue("(dd)", dx, dy);
}

static PyObject *
matrix_transform_point (PycairoMatrix *o, PyObject *args) {
  double x, y;

  if (!PyArg_ParseTuple(args, "dd:Matrix.transform_point", &x, &y))
    return NULL;

  cairo_matrix_transform_point (&o->matrix, &x, &y);
  return Py_BuildValue("(dd)", x, y);
}

static PyObject *
matrix_translate (PycairoMatrix *o, PyObject *args) {
  double tx, ty;

  if (!PyArg_ParseTuple(args, "dd:Matrix.translate", &tx, &ty))
    return NULL;

  cairo_matrix_translate (&o->matrix, tx, ty);
  Py_RETURN_NONE;
}

static PyObject *
matrix_item (PycairoMatrix *o, Py_ssize_t i) {
  switch (i) {
  case 0:
    return Py_BuildValue("d", o->matrix.xx);
  case 1:
    return Py_BuildValue("d", o->matrix.yx);
  case 2:
    return Py_BuildValue("d", o->matrix.xy);
  case 3:
    return Py_BuildValue("d", o->matrix.yy);
  case 4:
    return Py_BuildValue("d", o->matrix.x0);
  case 5:
    return Py_BuildValue("d", o->matrix.y0);
  default:
    PyErr_SetString(PyExc_IndexError, "Matrix index out of range");
    return NULL;
  }
}

static PyNumberMethods matrix_as_number = {
  (binaryfunc)0,   /*nb_add*/
  (binaryfunc)0,   /*nb_subtract*/
  (binaryfunc)matrix_operator_multiply,  /*nb_multiply*/
#if PY_MAJOR_VERSION < 3
  (binaryfunc)0,   /*nb_divide*/
#endif
  (binaryfunc)0,   /*nb_remainder*/
  (binaryfunc)0,   /*nb_divmod*/
  (ternaryfunc)0,  /*nb_power*/
  (unaryfunc)0,    /*nb_negative*/
  (unaryfunc)0,    /*nb_positive*/
  (unaryfunc)0,    /*nb_absolute*/
  (inquiry)0,      /*py2:nb_nonzero/py3:nb_bool*/
  (unaryfunc)0,    /*nb_invert*/
  (binaryfunc)0,   /*nb_lshift*/
  (binaryfunc)0,   /*nb_rshift*/
  (binaryfunc)0,   /*nb_and*/
  (binaryfunc)0,   /*nb_xor*/
  (binaryfunc)0,   /*nb_or*/
#if PY_MAJOR_VERSION < 3
  (coercion)0,     /*nb_coerce*/
#endif
  (unaryfunc)0,    /*nb_int*/
  0,               /*py2:nb_long/py3:nb_reserved*/
  (unaryfunc)0,    /*nb_float*/
#if PY_MAJOR_VERSION < 3
  (unaryfunc)0,    /*nb_oct*/
  (unaryfunc)0,    /*nb_hex*/
#endif
  0,		   /*nb_inplace_add*/
  0,		   /*nb_inplace_subtract*/
  0,		   /*nb_inplace_multiply*/
#if PY_MAJOR_VERSION < 3
  0,		   /*nb_inplace_divide*/
#endif
  0,		   /*nb_inplace_remainder*/
  0,		   /*nb_inplace_power*/
  0,		   /*nb_inplace_lshift*/
  0,		   /*nb_inplace_rshift*/
  0,		   /*nb_inplace_and*/
  0,		   /*nb_inplace_xor*/
  0,		   /*nb_inplace_or*/
  (binaryfunc)0,   /* nb_floor_divide */
  0,	           /* nb_true_divide */
  0,		   /* nb_inplace_floor_divide */
  0,		   /* nb_inplace_true_divide */
  (unaryfunc)0,	   /* nb_index */
};

static PySequenceMethods matrix_as_sequence = {
  0,                  		/* sq_length */
  0,                  		/* sq_concat */
  0,                  		/* sq_repeat */
  (ssizeargfunc)matrix_item,	/* sq_item */
  0,                     	/* sq_slice */
  0,				/* sq_ass_item */
  0,				/* sq_ass_slice */
  0,		                /* sq_contains */
};

static PyMethodDef matrix_methods[] = {
  /* Do not need to wrap all cairo_matrix_init_*() functions
   * C API Matrix constructors       Python equivalents
   * cairo_matrix_init()             cairo.Matrix(xx,yx,xy,yy,x0,y0)
   * cairo_matrix_init_rotate()      cairo.Matrix.init_rotate(radians)

   * cairo_matrix_init_identity()    cairo.Matrix()
   * cairo_matrix_init_translate()   cairo.Matrix(x0=x0,y0=y0)
   * cairo_matrix_init_scale()       cairo.Matrix(xx=xx,yy=yy)
   */
  {"init_rotate", (PyCFunction)matrix_init_rotate, METH_VARARGS | METH_CLASS },
  {"invert",      (PyCFunction)matrix_invert,                METH_NOARGS },
  {"multiply",    (PyCFunction)matrix_multiply,              METH_VARARGS },
  {"rotate",      (PyCFunction)matrix_rotate,                METH_VARARGS },
  {"scale",       (PyCFunction)matrix_scale,                 METH_VARARGS },
  {"transform_distance",(PyCFunction)matrix_transform_distance, METH_VARARGS },
  {"transform_point", (PyCFunction)matrix_transform_point,   METH_VARARGS },
  {"translate",   (PyCFunction)matrix_translate,             METH_VARARGS },
  {NULL, NULL, 0, NULL},
};

static PyMemberDef matrix_tp_members[] = {
  {"xx", T_DOUBLE, sizeof(PyObject) + sizeof(double) * 0, 0,
   "xx component of the affine transformation"},
  {"yx", T_DOUBLE, sizeof(PyObject) + sizeof(double) * 1, 0,
   "yx component of the affine transformation"},
  {"xy", T_DOUBLE, sizeof(PyObject) + sizeof(double) * 2, 0,
   "xy component of the affine transformation"},
  {"yy", T_DOUBLE, sizeof(PyObject) + sizeof(double) * 3, 0,
   "yy component of the affine transformation"},
  {"x0", T_DOUBLE, sizeof(PyObject) + sizeof(double) * 4, 0,
   "X translation component of the affine transformation"},
  {"y0", T_DOUBLE, sizeof(PyObject) + sizeof(double) * 5, 0,
   "Y translation component of the affine transformation"},
  {NULL}
};

PyTypeObject PycairoMatrix_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
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
  &matrix_as_sequence,                /* tp_as_sequence */
  0,                                  /* tp_as_mapping */
  0,                                  /* tp_hash */
  0,                                  /* tp_call */
  0,                                  /* tp_str */
  0,                                  /* tp_getattro */
  0,                                  /* tp_setattro */
  0,                                  /* tp_as_buffer */
#if PY_MAJOR_VERSION < 3
  Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_CHECKTYPES,            /* tp_flags */
#else
  Py_TPFLAGS_DEFAULT,                 /* tp_flags */
#endif
  NULL,                               /* tp_doc */
  0,                                  /* tp_traverse */
  0,                                  /* tp_clear */
  (richcmpfunc)matrix_richcmp,        /* tp_richcompare */
  0,                                  /* tp_weaklistoffset */
  0,                                  /* tp_iter */
  0,                                  /* tp_iternext */
  matrix_methods,                     /* tp_methods */
  matrix_tp_members,                  /* tp_members */
  0,                                  /* tp_getset */
  0,                                  /* tp_base */
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
