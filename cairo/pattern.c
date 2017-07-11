/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2004-2006 Steve Chaplin
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


/* Class Pattern ---------------------------------------------------------- */

/* PycairoPattern_FromPattern
 * Create a new
 *   PycairoSolidPattern,
 *   PycairoSurfacePattern,
 *   PycairoLinearGradient, or
 *   PycairoRadialGradient from a cairo_pattern_t.
 * pattern - a cairo_pattern_t to 'wrap' into a Python object.
 *   It is unreferenced if the PycairoPattern creation fails, or if the
 *   pattern has an error status.
 * base - the base object used to create the pattern, or NULL.
 *   It is referenced to keep it alive while the cairo_pattern_t is being used.
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoPattern_FromPattern (cairo_pattern_t *pattern, PyObject *base) {
  PyTypeObject *type = NULL;
  PyObject *o;

  assert (pattern != NULL);

  if (Pycairo_Check_Status (cairo_pattern_status (pattern))) {
    cairo_pattern_destroy (pattern);
    return NULL;
  }

  switch (cairo_pattern_get_type (pattern)) {
  case CAIRO_PATTERN_TYPE_SOLID:
    type = &PycairoSolidPattern_Type;
    break;
  case CAIRO_PATTERN_TYPE_SURFACE:
    type = &PycairoSurfacePattern_Type;
    break;
  case CAIRO_PATTERN_TYPE_LINEAR:
    type = &PycairoLinearGradient_Type;
    break;
  case CAIRO_PATTERN_TYPE_RADIAL:
    type = &PycairoRadialGradient_Type;
    break;
  case CAIRO_PATTERN_TYPE_MESH:
    type = &PycairoMeshPattern_Type;
    break;
  default:
    type = &PycairoPattern_Type;
    break;
  }

  o = type->tp_alloc(type, 0);
  if (o == NULL) {
    cairo_pattern_destroy (pattern);
  } else {
    ((PycairoPattern *)o)->pattern = pattern;
    Py_XINCREF(base);
    ((PycairoPattern *)o)->base = base;
  }
  return o;
}

static void
pattern_dealloc (PycairoPattern *o) {
  if (o->pattern) {
    cairo_pattern_destroy (o->pattern);
    o->pattern = NULL;
  }
  Py_CLEAR(o->base);

  Py_TYPE(o)->tp_free(o);
}

static PyObject *
pattern_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PyErr_SetString(PyExc_TypeError,
		  "The Pattern type cannot be instantiated");
  return NULL;
}

static PyObject *
pattern_get_extend (PycairoPattern *o) {
  RETURN_INT_ENUM (Extend, cairo_pattern_get_extend (o->pattern));
}

static PyObject *
pattern_get_matrix (PycairoPattern *o) {
  cairo_matrix_t matrix;
  cairo_pattern_get_matrix (o->pattern, &matrix);
  return PycairoMatrix_FromMatrix (&matrix);
}

static PyObject *
pattern_set_extend (PycairoPattern *o, PyObject *args) {
  int extend;

  if (!PyArg_ParseTuple(args, "i:Pattern.set_extend", &extend))
    return NULL;

  cairo_pattern_set_extend (o->pattern, extend);
  Py_RETURN_NONE;
}

static PyObject *
pattern_set_matrix (PycairoPattern *o, PyObject *args) {
  PycairoMatrix *m;

  if (!PyArg_ParseTuple (args, "O!:Pattern.set_matrix",
			 &PycairoMatrix_Type, &m))
    return NULL;

  cairo_pattern_set_matrix (o->pattern, &m->matrix);
  Py_RETURN_NONE;
}

static PyObject *
pattern_get_filter (PycairoPattern *o) {
  cairo_filter_t filter;

  Py_BEGIN_ALLOW_THREADS;
  filter = cairo_pattern_get_filter (o->pattern);
  Py_END_ALLOW_THREADS;

  RETURN_INT_ENUM (Filter, filter);
}

static PyObject *
pattern_set_filter (PycairoPattern *o, PyObject *args) {
  cairo_filter_t filter;

  if (!PyArg_ParseTuple (args, "i:Pattern.set_filter", &filter))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_pattern_set_filter (o->pattern, filter);
  Py_END_ALLOW_THREADS;

  Py_RETURN_NONE;
}

static PyMethodDef pattern_methods[] = {
  /* methods never exposed in a language binding:
   * cairo_pattern_destroy()
   * cairo_pattern_get_type()
   * cairo_pattern_reference()
   *
   * cairo_pattern_status()
   * - not needed since Pycairo handles status checking
   */
  {"get_extend", (PyCFunction)pattern_get_extend,          METH_NOARGS },
  {"get_matrix", (PyCFunction)pattern_get_matrix,          METH_NOARGS },
  {"set_extend", (PyCFunction)pattern_set_extend,          METH_VARARGS },
  {"set_matrix", (PyCFunction)pattern_set_matrix,          METH_VARARGS },
  {"get_filter", (PyCFunction)pattern_get_filter,          METH_NOARGS },
  {"set_filter", (PyCFunction)pattern_set_filter,          METH_VARARGS },
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoPattern_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.Pattern",                    /* tp_name */
  sizeof(PycairoPattern),             /* tp_basicsize */
  0,                                  /* tp_itemsize */
  (destructor)pattern_dealloc,        /* tp_dealloc */
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
  pattern_methods,                    /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  0,                                  /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)pattern_new,               /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};


/* Class SolidPattern ----------------------------------------------------- */

static PyObject *
solid_pattern_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  double r, g, b, a = 1.0;
  if (!PyArg_ParseTuple (args, "ddd|d:SolidPattern.__new__", &r, &g, &b, &a))
    return NULL;
  return PycairoPattern_FromPattern (cairo_pattern_create_rgba (r, g, b, a),
				     NULL);
}

static PyObject *
solid_pattern_get_rgba (PycairoSolidPattern *o) {
  double red, green, blue, alpha;
  cairo_pattern_get_rgba (o->pattern, &red, &green, &blue, &alpha);
  return Py_BuildValue("(dddd)", red, green, blue, alpha);
}

static PyMethodDef solid_pattern_methods[] = {
  {"get_rgba",       (PyCFunction)solid_pattern_get_rgba,      METH_NOARGS },
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoSolidPattern_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.SolidPattern",               /* tp_name */
  sizeof(PycairoSolidPattern),        /* tp_basicsize */
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
  solid_pattern_methods,              /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoPattern_Type,               /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)solid_pattern_new,         /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};

/* Class SurfacePattern --------------------------------------------------- */

static PyObject *
surface_pattern_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PycairoSurface *s;
  if (!PyArg_ParseTuple (args, "O!:SurfacePattern.__new__",
			 &PycairoSurface_Type, &s))
    return NULL;
  return PycairoPattern_FromPattern (
	     cairo_pattern_create_for_surface (s->surface), NULL);
}

static PyObject *
surface_pattern_get_surface (PycairoSurfacePattern *o) {
  cairo_surface_t *surface;
  RETURN_NULL_IF_CAIRO_ERROR(cairo_pattern_get_surface (o->pattern, &surface));
  return PycairoSurface_FromSurface(cairo_surface_reference (surface), NULL);
}

static PyMethodDef surface_pattern_methods[] = {
  {"get_surface",   (PyCFunction)surface_pattern_get_surface, METH_NOARGS },
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoSurfacePattern_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.SurfacePattern",             /* tp_name */
  sizeof(PycairoSurfacePattern),      /* tp_basicsize */
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
  surface_pattern_methods,            /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoPattern_Type,               /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)surface_pattern_new,       /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};

/* Class Gradient --------------------------------------------------------- */

static PyObject *
gradient_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PyErr_SetString(PyExc_TypeError,
		  "The Gradient type cannot be instantiated");
  return NULL;
}

static PyObject *
gradient_add_color_stop_rgb (PycairoGradient *o, PyObject *args) {
  double offset, red, green, blue;
  if (!PyArg_ParseTuple(args, "dddd:Gradient.add_color_stop_rgb",
			&offset, &red, &green, &blue))
    return NULL;
  cairo_pattern_add_color_stop_rgb (o->pattern, offset, red, green, blue);
  RETURN_NULL_IF_CAIRO_PATTERN_ERROR(o->pattern);
  Py_RETURN_NONE;
}

static PyObject *
gradient_add_color_stop_rgba (PycairoGradient *o, PyObject *args) {
  double offset, red, green, blue, alpha;
  if (!PyArg_ParseTuple(args, "ddddd:Gradient.add_color_stop_rgba",
			&offset, &red, &green, &blue, &alpha))
    return NULL;
  cairo_pattern_add_color_stop_rgba (o->pattern, offset, red,
				     green, blue, alpha);
  RETURN_NULL_IF_CAIRO_PATTERN_ERROR(o->pattern);
  Py_RETURN_NONE;
}

static PyObject *
gradient_get_color_stops_rgba (PycairoGradient *obj) {
  cairo_status_t status;
  double offset, red, green, blue, alpha;
  int count;
  PyObject *list, *tuple;
  int result, i;

  status = cairo_pattern_get_color_stop_count (obj->pattern, &count);
  RETURN_NULL_IF_CAIRO_ERROR (status);

  list = PyList_New (0);
  if (list == NULL)
    return NULL;

  for (i = 0; i < count; i++) {
    status = cairo_pattern_get_color_stop_rgba (
      obj->pattern, i, &offset, &red, &green, &blue, &alpha);
    if (status != CAIRO_STATUS_SUCCESS)
      goto error;

    tuple = Py_BuildValue("(ddddd)", offset, red, green, blue, alpha);
    if (tuple == NULL)
      goto error;

    result = PyList_Append (list, tuple);
    Py_DECREF (tuple);
    if (result == -1)
      goto error;
  }

  return list;
error:
  Py_DECREF (list);
  if (status != CAIRO_STATUS_SUCCESS)
    Pycairo_Check_Status (status);
  return NULL;
}

static PyMethodDef gradient_methods[] = {
  {"add_color_stop_rgb",(PyCFunction)gradient_add_color_stop_rgb,
   METH_VARARGS },
  {"add_color_stop_rgba",(PyCFunction)gradient_add_color_stop_rgba,
   METH_VARARGS },
  {"get_color_stops_rgba",(PyCFunction)gradient_get_color_stops_rgba,
   METH_NOARGS },
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoGradient_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.Gradient",                   /* tp_name */
  sizeof(PycairoGradient),            /* tp_basicsize */
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
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,/* tp_flags */
  0,                                  /* tp_doc */
  0,                                  /* tp_traverse */
  0,                                  /* tp_clear */
  0,                                  /* tp_richcompare */
  0,                                  /* tp_weaklistoffset */
  0,                                  /* tp_iter */
  0,                                  /* tp_iternext */
  gradient_methods,                   /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoPattern_Type,               /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)gradient_new,              /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};

/* Class LinearGradient --------------------------------------------------- */

static PyObject *
linear_gradient_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  double x0, y0, x1, y1;
  if (!PyArg_ParseTuple(args, "dddd:LinearGradient.__new__",
			&x0, &y0, &x1, &y1))
    return NULL;
  return PycairoPattern_FromPattern (
	       cairo_pattern_create_linear (x0, y0, x1, y1), NULL);
}

static PyObject *
linear_gradient_get_linear_points (PycairoLinearGradient *o) {
  double x0, y0, x1, y1;
  cairo_pattern_get_linear_points (o->pattern, &x0, &y0, &x1, &y1);
  return Py_BuildValue("(dddd)", x0, y0, x1, y1);
}

static PyMethodDef linear_gradient_methods[] = {
  {"get_linear_points", (PyCFunction)linear_gradient_get_linear_points,
   METH_NOARGS },
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoLinearGradient_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.LinearGradient",             /* tp_name */
  sizeof(PycairoLinearGradient),      /* tp_basicsize */
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
  linear_gradient_methods,            /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoGradient_Type,              /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)linear_gradient_new,       /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};

/* Class RadialGradient --------------------------------------------------- */

static PyObject *
radial_gradient_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  double cx0, cy0, radius0, cx1, cy1, radius1;
  if (!PyArg_ParseTuple(args, "dddddd:RadialGradient.__new__",
			&cx0, &cy0, &radius0, &cx1, &cy1, &radius1))
    return NULL;
  return PycairoPattern_FromPattern (
	    cairo_pattern_create_radial (cx0, cy0, radius0, cx1, cy1, radius1),
	    NULL);
}

static PyObject *
radial_gradient_get_radial_circles (PycairoRadialGradient *o) {
  double x0, y0, r0, x1, y1, r1;
  cairo_pattern_get_radial_circles (o->pattern, &x0, &y0, &r0,
				    &x1, &y1, &r1);
  return Py_BuildValue("(dddddd)", x0, y0, r0, x1, y1, r1);
}

static PyMethodDef radial_gradient_methods[] = {
  {"get_radial_circles", (PyCFunction)radial_gradient_get_radial_circles,
   METH_NOARGS },
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoRadialGradient_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.RadialGradient",             /* tp_name */
  sizeof(PycairoRadialGradient),      /* tp_basicsize */
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
  radial_gradient_methods,            /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoGradient_Type,              /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)radial_gradient_new,       /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};

static PyObject *
mesh_pattern_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
  if (!PyArg_ParseTuple (args, ":Mesh.__new__"))
    return NULL;

  return PycairoPattern_FromPattern (cairo_pattern_create_mesh (), NULL);
}

static PyObject *
mesh_pattern_begin_patch (PycairoMeshPattern *obj) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_begin_patch (obj->pattern);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyObject *
mesh_pattern_end_patch (PycairoMeshPattern *obj) {
  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_end_patch (obj->pattern);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyObject *
mesh_pattern_curve_to (PycairoMeshPattern *obj, PyObject *args) {
  double x1, y1, x2, y2, x3, y3;

  if (!PyArg_ParseTuple(args, "dddddd:MeshPattern.curve_to",
      &x1, &y1, &x2, &y2, &x3, &y3))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_curve_to (obj->pattern, x1, y1, x2, y2, x3, y3);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyObject *
mesh_pattern_get_control_point (PycairoMeshPattern *obj, PyObject *args) {
  double x, y;
  unsigned int patch_num, point_num;
  cairo_status_t status;

  if (!PyArg_ParseTuple(args, "II:MeshPattern.get_control_point",
      &patch_num, &point_num))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  status = cairo_mesh_pattern_get_control_point (
    obj->pattern, patch_num, point_num, &x, &y);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_ERROR (status);

  return Py_BuildValue("(dd)", x, y);
}

static PyObject *
mesh_pattern_get_corner_color_rgba (PycairoMeshPattern *obj, PyObject *args) {
  double red, green, blue, alpha;
  unsigned int patch_num, corner_num;
  cairo_status_t status;

  if (!PyArg_ParseTuple(args, "II:MeshPattern.get_corner_color_rgba",
      &patch_num, &corner_num))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  status = cairo_mesh_pattern_get_corner_color_rgba (
    obj->pattern, patch_num, corner_num, &red, &green, &blue, &alpha);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_ERROR (status);

  return Py_BuildValue("(dddd)", red, green, blue, alpha);
}

static PyObject *
mesh_pattern_get_patch_count (PycairoMeshPattern *obj) {
  unsigned int count;
  cairo_status_t status;

  Py_BEGIN_ALLOW_THREADS;
  status = cairo_mesh_pattern_get_patch_count (obj->pattern, &count);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_ERROR (status);

  return PYCAIRO_PyLong_FromLong (count);
}

static PyObject *
mesh_pattern_get_path (PycairoMeshPattern *obj, PyObject *args) {
  unsigned int patch_num;
  cairo_path_t *path;

  if (!PyArg_ParseTuple(args, "I:MeshPattern.get_path", &patch_num))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  path = cairo_mesh_pattern_get_path (obj->pattern, patch_num);
  Py_END_ALLOW_THREADS;

  return PycairoPath_FromPath (path);
}

static PyObject *
mesh_pattern_line_to (PycairoMeshPattern *obj, PyObject *args) {
  double x, y;

  if (!PyArg_ParseTuple(args, "dd:MeshPattern.line_to", &x, &y))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_line_to (obj->pattern, x, y);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyObject *
mesh_pattern_move_to (PycairoMeshPattern *obj, PyObject *args) {
  double x, y;

  if (!PyArg_ParseTuple(args, "dd:MeshPattern.move_to", &x, &y))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_move_to (obj->pattern, x, y);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyObject *
mesh_pattern_set_control_point (PycairoMeshPattern *obj, PyObject *args) {
  double x, y;
  unsigned int point_num;

  if (!PyArg_ParseTuple(args, "Idd:MeshPattern.set_control_point",
      &point_num, &x, &y))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_set_control_point (obj->pattern, point_num, x, y);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyObject *
mesh_pattern_set_corner_color_rgb (PycairoMeshPattern *obj, PyObject *args) {
  double red, green, blue;
  unsigned int corner_num;

  if (!PyArg_ParseTuple(args, "Iddd:MeshPattern.set_corner_color_rgb",
      &corner_num, &red, &green, &blue))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_set_corner_color_rgb (
    obj->pattern, corner_num, red, green, blue);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyObject *
mesh_pattern_set_corner_color_rgba (PycairoMeshPattern *obj, PyObject *args) {
  double red, green, blue, alpha;
  unsigned int corner_num;

  if (!PyArg_ParseTuple(args, "Idddd:MeshPattern.set_corner_color_rgba",
      &corner_num, &red, &green, &blue, &alpha))
    return NULL;

  Py_BEGIN_ALLOW_THREADS;
  cairo_mesh_pattern_set_corner_color_rgba (
    obj->pattern, corner_num, red, green, blue, alpha);
  Py_END_ALLOW_THREADS;

  RETURN_NULL_IF_CAIRO_PATTERN_ERROR (obj->pattern);

  Py_RETURN_NONE;
}

static PyMethodDef mesh_pattern_methods[] = {
  {"begin_patch",       (PyCFunction)mesh_pattern_begin_patch, METH_NOARGS},
  {"end_patch",         (PyCFunction)mesh_pattern_end_patch,   METH_NOARGS},
  {"curve_to",          (PyCFunction)mesh_pattern_curve_to,    METH_VARARGS},
  {"get_control_point",
   (PyCFunction)mesh_pattern_get_control_point, METH_VARARGS},
  {"get_corner_color_rgba",
   (PyCFunction)mesh_pattern_get_corner_color_rgba, METH_VARARGS},
  {"get_patch_count",
   (PyCFunction)mesh_pattern_get_patch_count, METH_NOARGS},
  {"get_path",          (PyCFunction)mesh_pattern_get_path,    METH_VARARGS},
  {"line_to",           (PyCFunction)mesh_pattern_line_to,     METH_VARARGS},
  {"move_to",           (PyCFunction)mesh_pattern_move_to,     METH_VARARGS},
  {"set_control_point",
   (PyCFunction)mesh_pattern_set_control_point, METH_VARARGS},
  {"set_corner_color_rgb",
   (PyCFunction)mesh_pattern_set_corner_color_rgb, METH_VARARGS},
  {"set_corner_color_rgba",
   (PyCFunction)mesh_pattern_set_corner_color_rgba, METH_VARARGS},
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoMeshPattern_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.MeshPattern",                /* tp_name */
  sizeof(PycairoMeshPattern),         /* tp_basicsize */
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
  mesh_pattern_methods,               /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoPattern_Type,               /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)mesh_pattern_new,          /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};
