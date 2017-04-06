/* -*- mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo-private.h"


/* wrap an existing cairo_pattern_t in a PyCairoPattern object */
PyObject *
pycairo_pattern_wrap(cairo_pattern_t *pattern)
{
    PyCairoPattern *self;

    self = PyObject_New(PyCairoPattern, &PyCairoPattern_Type);
    if (self) {
	cairo_pattern_reference(pattern);
	self->pattern = pattern;
    }
    return (PyObject *)self;
}

static void
pycairo_pattern_dealloc(PyCairoPattern *self)
{
    if (self->pattern)
	cairo_pattern_destroy(self->pattern);
    self->pattern = NULL;

    if (self->ob_type->tp_free)
	self->ob_type->tp_free((PyObject *)self);
    else
	PyObject_Del(self);
}

static PyObject *
pycairo_pattern_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyCairoPattern *self = (PyCairoPattern *)type->tp_alloc(type, 0);

    if (self)
      self->pattern = NULL;
    return (PyObject *)self;
}

static int
pycairo_pattern_init(PyCairoPattern *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "x0", "y0", "x1", "y1",
			      "cx0", "cy0", "radius0", "cx1", "cy1", "radius1",
			      "surface",
			      NULL };
    double x0 = -1.0, y0 = -1.0, x1 = -1.0, y1 = -1.0;
    double cx0 =-1.0, cy0 =-1.0, radius0 =-1.0, cx1 =-1.0, cy1 =-1.0, radius1 =-1.0;
    PyCairoSurface *surface = NULL;
    cairo_pattern_t *pattern = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "|ddddddddddO!:Pattern.__init__", kwlist,
				     &x0, &y0, &x1, &y1,
				     &cx0, &cy0, &radius0,
				     &cx1, &cy1, &radius1,
				     &PyCairoSurface_Type, &surface
				     )) /* change to keywords only? */
	return -1;

    /* three alternative constructors */
    if (x0 != -1.0 && y0 != -1.0 && x1 != -1.0 && y1 != -1.0) {
	pattern = cairo_pattern_create_linear (x0, y0, x1, y1);

    } else if (cx0 != -1.0 && cy0 != -1.0 && radius0 != -1.0 &&
	       cx1 != -1.0 && cy1 != -1.0 && radius1 != -1.0) {
	pattern = cairo_pattern_create_radial (cx0, cy0, radius0,
					       cx1, cy1, radius1);

    } else if (surface != NULL) {
	pattern = cairo_pattern_create_for_surface (surface->surface);

    } else {
	PyErr_SetString(PyExc_ValueError, "incorrect arguments for pattern");
	return -1;
    }

    if (!pattern) {
	PyErr_SetString(PyExc_RuntimeError, "could not create pattern");
	return -1;
    }

    /*cairo_pattern_reference(pattern);*/
    self->pattern = pattern;

    return 0;
}

static PyObject *
pycairo_pattern_add_color_stop(PyCairoPattern *self, PyObject *args)
{
    double offset, red, green, blue, alpha;
    cairo_status_t status;

    if (!PyArg_ParseTuple(args, "ddddd:Pattern.add_color_stop",
			  &offset, &red, &green, &blue, &alpha))
	return NULL;

    status = cairo_pattern_add_color_stop (self->pattern, offset, red, green, 
					   blue, alpha);
    if (pycairo_check_status(status))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pycairo_pattern_set_matrix(PyCairoPattern *self, PyObject *args)
{
    PyCairoMatrix *matrix;

    if (!PyArg_ParseTuple(args, "O!:Pattern.set_matrix",
			  &PyCairoMatrix_Type, &matrix))
	return NULL;

    /* always returns status = success */
    cairo_pattern_set_matrix(self->pattern, matrix->matrix);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pycairo_pattern_set_extend(PyCairoPattern *self, PyObject *args)
{
    int extend;

    if (!PyArg_ParseTuple(args, "i:Pattern.set_extend", &extend))
 	return NULL;
 
    /* always returns status = success */
    cairo_pattern_set_extend(self->pattern, extend);
    Py_INCREF(Py_None);
    return Py_None;
}
 
static PyObject *
pycairo_pattern_set_filter(PyCairoPattern *self, PyObject *args)
{
    int filter;

    if (!PyArg_ParseTuple(args, "i:Pattern.set_filter", &filter))
	return NULL;

    /* always returns status = success */
    cairo_pattern_set_filter(self->pattern, filter);
    Py_INCREF(Py_None);
    return Py_None;
}
 
static PyObject *
pycairo_pattern_get_matrix(PyCairoPattern *self)
{
    cairo_matrix_t *matrix;

    matrix = cairo_matrix_create();
    if (!matrix)
	return PyErr_NoMemory();

    /* always returns status = success */
    cairo_pattern_get_matrix(self->pattern, matrix);
    return pycairo_matrix_new(matrix);
}

static PyObject *
pycairo_pattern_get_extend(PyCairoPattern *self)
{
    return PyInt_FromLong(cairo_pattern_get_extend(self->pattern));
}

static PyObject *
pycairo_pattern_get_filter(PyCairoPattern *self)
{
    return PyInt_FromLong(cairo_pattern_get_filter(self->pattern));
}

 
static PyMethodDef pycairo_pattern_methods[] = {
    { "add_color_stop", (PyCFunction)pycairo_pattern_add_color_stop, 
      METH_VARARGS },
    { "set_matrix", (PyCFunction)pycairo_pattern_set_matrix, METH_VARARGS },
    { "set_extend", (PyCFunction)pycairo_pattern_set_extend, METH_VARARGS },
    { "set_filter", (PyCFunction)pycairo_pattern_set_filter, METH_VARARGS },
    { NULL, NULL, 0 }
};

static PyGetSetDef pycairo_pattern_getsets[] = {
    { "matrix", (getter)pycairo_pattern_get_matrix, (setter)0 },
    { "extend", (getter)pycairo_pattern_get_extend, (setter)0 },
    { "filter", (getter)pycairo_pattern_get_filter, (setter)0 },
    { NULL, (getter)0, (setter)0 }
};

PyTypeObject PyCairoPattern_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Pattern",                    /* tp_name */
    sizeof(PyCairoPattern),             /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)pycairo_pattern_dealloc, /* tp_dealloc */
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
    "Pattern objects",                  /* tp_doc */
    (traverseproc)0,                    /* tp_traverse */
    (inquiry)0,                         /* tp_clear */
    (richcmpfunc)0,                     /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    (getiterfunc)0,                     /* tp_iter */
    (iternextfunc)0,                    /* tp_iternext */
    pycairo_pattern_methods,            /* tp_methods */
    0,                                  /* tp_members */
    pycairo_pattern_getsets,            /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)pycairo_pattern_init,     /* tp_init */
    (allocfunc)0,                       /* tp_alloc */
    pycairo_pattern_new,                /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
