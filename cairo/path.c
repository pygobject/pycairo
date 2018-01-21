/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2005 Steve Chaplin
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

#include "private.h"


/* PycairoPath iterator object
 * modelled on Python-2.4/Objects/rangeobject.c and tupleobject.c
 */

PyObject *
PycairoPath_FromPath (cairo_path_t *path) {
  PyObject *o;

  assert (path != NULL);

  if (Pycairo_Check_Status (path->status)) {
    cairo_path_destroy (path);
    return NULL;
  }

  o = PycairoPath_Type.tp_alloc (&PycairoPath_Type, 0);
  if (o)
    ((PycairoPath *)o)->path = path;
  else
    cairo_path_destroy (path);
  return o;
}

static void
path_dealloc(PycairoPath *p) {
  if (p->path) {
    cairo_path_destroy(p->path);
    p->path = NULL;
  }
  Py_TYPE(p)->tp_free(p);
}

static PyObject *
path_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  return type->tp_alloc(type, 0);
  /* initializes memory to zeros */
}

static int
path_init(PycairoPath *p, PyObject *args, PyObject *kwds) {
  PyErr_SetString(PyExc_TypeError, "The Path type cannot be instantiated, "
		  "use Context.copy_path()");
  return -1;
}

static PyObject *
path_str(PycairoPath *p) {
  PyObject *s, *pieces = NULL, *result = NULL;
  cairo_path_t *path = p->path;
  cairo_path_data_t *data;
  int i, ret;
  char buf[80];

  pieces = PyList_New(0);
  if (pieces == NULL)
    goto Done;

  /* loop reading elements */
  for (i=0; i < path->num_data; i += path->data[i].header.length) {
    data = &path->data[i];
    switch (data->header.type) {

    case CAIRO_PATH_MOVE_TO:
      PyOS_snprintf(buf, sizeof(buf), "move_to %f %f",
		    data[1].point.x, data[1].point.y);
      s = PYCAIRO_PyUnicode_FromString(buf);
      if (!s)
	goto Done;
      ret = PyList_Append(pieces, s);
      Py_DECREF(s);
      if (ret < 0)
	goto Done;
      break;

    case CAIRO_PATH_LINE_TO:
      PyOS_snprintf(buf, sizeof(buf), "line_to %f %f",
		    data[1].point.x, data[1].point.y);
      s = PYCAIRO_PyUnicode_FromString(buf);
      if (!s)
	goto Done;
      ret = PyList_Append(pieces, s);
      Py_DECREF(s);
      if (ret < 0)
	goto Done;
      break;

    case CAIRO_PATH_CURVE_TO:
      PyOS_snprintf(buf, sizeof(buf), "curve_to %f %f %f %f %f %f",
		    data[1].point.x, data[1].point.y,
		    data[2].point.x, data[2].point.y,
		    data[3].point.x, data[3].point.y);
      s = PYCAIRO_PyUnicode_FromString(buf);
      if (!s)
	goto Done;
      ret = PyList_Append(pieces, s);
      Py_DECREF(s);
      if (ret < 0)
	goto Done;
      break;

    case CAIRO_PATH_CLOSE_PATH:
      s = PYCAIRO_PyUnicode_FromString("close path");
      if (!s)
	goto Done;
      ret = PyList_Append(pieces, s);
      Py_DECREF(s);
      if (ret < 0)
	goto Done;
      break;
    default:
      PyErr_SetString(PyExc_RuntimeError, "unknown CAIRO_PATH type");
      goto Done;
      break;
    }
  }
  /* result = "\n".join(pieces) */
  s = PYCAIRO_PyUnicode_FromString("\n");
  if (s == NULL)
    goto Done;
  result = PYCAIRO_PyUnicode_Join(s, pieces);
  Py_DECREF(s);

Done:
  Py_XDECREF(pieces);
  return result;
}

static PyObject * path_iter(PyObject *seq); /* forward declaration */


static PyObject*
path_richcompare (PyObject *self, PyObject *other, int op)
{
  if (Py_TYPE(self) == Py_TYPE(other))
    return Pycairo_richcompare (
      ((PycairoPath *)self)->path,
      ((PycairoPath *)other)->path,
      op);
  else {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
}

static PYCAIRO_Py_hash_t
path_hash (PyObject *self)
{
  return PYCAIRO_Py_hash_t_FromVoidPtr (((PycairoPath *)self)->path);
}

PyTypeObject PycairoPath_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.Path",			/* tp_name */
  sizeof(PycairoPath),		/* tp_basicsize */
  0,					/* tp_itemsize */
  (destructor)path_dealloc,		/* tp_dealloc */
  0,					/* tp_print */
  0,					/* tp_getattr */
  0,					/* tp_setattr */
  0,					/* tp_compare */
  0,		                	/* tp_repr */
  0,					/* tp_as_number */
  0,              			/* tp_as_sequence */
  0,					/* tp_as_mapping */
  path_hash,				/* tp_hash */
  0,					/* tp_call */
  (reprfunc)path_str,			/* tp_str */
  0,	                        	/* tp_getattro */
  0,					/* tp_setattro */
  0,					/* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,			/* tp_flags */
  0,      				/* tp_doc */
  0,					/* tp_traverse */
  0,					/* tp_clear */
  path_richcompare,			/* tp_richcompare */
  0,					/* tp_weaklistoffset */
  (getiterfunc)path_iter,   		/* tp_iter */
  0,					/* tp_iternext */
  0,			        	/* tp_methods */
  0,					/* tp_members */
  0,					/* tp_getset */
  0,                                    /* tp_base */
  0,					/* tp_dict */
  0,					/* tp_descr_get */
  0,					/* tp_descr_set */
  0,					/* tp_dictoffset */
  (initproc)path_init,		/* tp_init */
  0,					/* tp_alloc */
  (newfunc)path_new,      		/* tp_new */
};

/*********************** PycairoPath Iterator **************************/

typedef struct {
  PyObject_HEAD
  int index;           /* position within PycairoPath */
  PycairoPath *pypath; /* Set to NULL when iterator is exhausted */
} PycairoPathiter;

PyTypeObject PycairoPathiter_Type;


static void
pathiter_dealloc(PycairoPathiter *it) {
  Py_XDECREF(it->pypath);
  PyObject_Del(it);
}

static PyObject *
path_iter(PyObject *pypath) {
  PycairoPathiter *it;

  if (!PyObject_TypeCheck (pypath, &PycairoPath_Type)) {
    PyErr_BadInternalCall();
    return NULL;
  }
  it = PyObject_New(PycairoPathiter, &PycairoPathiter_Type);
  if (it == NULL)
    return NULL;

  it->index = 0;
  Py_INCREF(pypath);
  it->pypath = (PycairoPath *)pypath;
  return (PyObject *) it;
}

static PyObject *
pathiter_next(PycairoPathiter *it) {
  PycairoPath *pypath;
  cairo_path_t *path;

  assert(it != NULL);
  pypath = it->pypath;
  if (pypath == NULL)
    return NULL;
  assert (PyObject_TypeCheck (pypath, &PycairoPath_Type));
  path = pypath->path;

  /* return the next path element, advance index */
  if (it->index < path->num_data) {
    cairo_path_data_t *data = &path->data[it->index];
    int type = data->header.type;

    it->index += data[0].header.length;

    switch (type) {
    case CAIRO_PATH_MOVE_TO:
    case CAIRO_PATH_LINE_TO:
      return Py_BuildValue("(i(dd))", type,
			   data[1].point.x, data[1].point.y);
    case CAIRO_PATH_CURVE_TO:
      return Py_BuildValue("(i(dddddd))", type,
			   data[1].point.x, data[1].point.y,
			   data[2].point.x, data[2].point.y,
			   data[3].point.x, data[3].point.y);
    case CAIRO_PATH_CLOSE_PATH:
      return Py_BuildValue("i()", type);
    default:
      PyErr_SetString(PyExc_RuntimeError, "unknown CAIRO_PATH type");
      return NULL;
    }
  }

  /* iterator has no remaining items */
  Py_DECREF(pypath);
  it->pypath = NULL;
  return NULL;
}

PyTypeObject PycairoPathiter_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.Pathiter",                   /* tp_name */
  sizeof(PycairoPathiter),            /* tp_basicsize */
  0,                                  /* tp_itemsize */
  (destructor)pathiter_dealloc,	/* tp_dealloc */
  0,                                  /* tp_print */
  0,                                  /* tp_getattr */
  0,                                  /* tp_setattr */
  0,                                  /* tp_compare */
  0,                                  /* tp_repr */
  0,                                  /* tp_as_number */
  0,                 			/* tp_as_sequence */
  0,                                  /* tp_as_mapping */
  0,                                  /* tp_hash */
  0,                                  /* tp_call */
  0,                                  /* tp_str */
  0,                                  /* tp_getattro */
  0,                                  /* tp_setattro */
  0,                                  /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,			/* tp_flags */
  0,                                  /* tp_doc */
  0,					/* tp_traverse */
  0,                                  /* tp_clear */
  0,                                  /* tp_richcompare */
  0,                                  /* tp_weaklistoffset */
  0, /* PyObject_SelfIter, */		/* tp_iter */
  (iternextfunc)pathiter_next,	/* tp_iternext */
  0,					/* tp_methods */
};
