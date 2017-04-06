/* -*- mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo-private.h"

PyObject *
pycairo_surface_new(cairo_surface_t *surface)
{
    PyCairoSurface *self;

    self = PyObject_New(PyCairoSurface, &PyCairoSurface_Type);
    if (!self) {
	cairo_surface_destroy(surface);
	return NULL;
    }

    self->surface = surface;

    return (PyObject *)self;
}

static void
pycairo_surface_dealloc(PyCairoSurface *self)
{
    if (self->surface)
	cairo_surface_destroy(self->surface);
    self->surface = NULL;

    if (self->ob_type->tp_free)
	self->ob_type->tp_free((PyObject *)self);
    else
	PyObject_Del(self);
}

static PyObject *
pycairo_surface_create_similar(PyCairoSurface *self, PyObject *args)
{
    cairo_surface_t *surface;
    cairo_format_t format;
    int width, height;

    if (!PyArg_ParseTuple(args, "iii:Surface.create_similar",
			  &format, &width, &height))
	return NULL;

    surface = cairo_surface_create_similar(self->surface, format,
					   width, height);
    if (!surface)
	return PyErr_NoMemory();
    return pycairo_surface_new(surface);
}

static PyObject *
pycairo_surface_set_repeat(PyCairoSurface *self, PyObject *args)
{
    int repeat;

    if (!PyArg_ParseTuple(args, "i:Surface.set_repeat", &repeat))
	return NULL;

    cairo_surface_set_repeat(self->surface, repeat);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pycairo_surface_set_matrix(PyCairoSurface *self, PyObject *args)
{
    PyCairoMatrix *matrix;

    if (!PyArg_ParseTuple(args, "O!:Surface.set_matrix",
			  &PyCairoMatrix_Type, &matrix))
	return NULL;

    cairo_surface_set_matrix(self->surface, matrix->matrix);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pycairo_surface_get_matrix(PyCairoSurface *self)
{
    cairo_matrix_t *matrix;

    matrix = cairo_matrix_create();
    if (!matrix)
	return PyErr_NoMemory();
    cairo_surface_get_matrix(self->surface, matrix);
    return pycairo_matrix_new(matrix);
}

static PyObject *
pycairo_surface_get_filter(PyCairoSurface *self)
{
    return PyInt_FromLong(cairo_surface_get_filter(self->surface));
}


static PyObject *
pycairo_surface_set_filter(PyCairoSurface *self, PyObject *args)
{
    cairo_filter_t filter;

    if (!PyArg_ParseTuple(args, "i:Surface.set_filter", &filter))
	return NULL;

    cairo_surface_set_filter(self->surface, filter);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef pycairo_surface_methods[] = {
    { "create_similar", (PyCFunction)pycairo_surface_create_similar,
      METH_VARARGS },
    { "set_repeat", (PyCFunction)pycairo_surface_set_repeat, METH_VARARGS },
    { "set_matrix", (PyCFunction)pycairo_surface_set_matrix, METH_VARARGS },
    { "set_filter", (PyCFunction)pycairo_surface_set_filter, METH_VARARGS },
    { NULL, NULL, 0 }
};

static PyGetSetDef pycairo_surface_getsets[] = {
    /* for some reason, there is no cairo_surface_get_repeat */
    { "matrix", (getter)pycairo_surface_get_matrix, (setter)0 },
    { "filter", (getter)pycairo_surface_get_filter, (setter)0 },
    { NULL, }
};

PyTypeObject PyCairoSurface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "cairo.Surface",                    /* tp_name */
    sizeof(PyCairoSurface),             /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)pycairo_surface_dealloc, /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    NULL, /* Documentation string */
    (traverseproc)0,                    /* tp_traverse */
    (inquiry)0,                         /* tp_clear */
    (richcmpfunc)0,                     /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    (getiterfunc)0,                     /* tp_iter */
    (iternextfunc)0,                    /* tp_iternext */
    pycairo_surface_methods,            /* tp_methods */
    0,                                  /* tp_members */
    pycairo_surface_getsets,            /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)0,                        /* tp_init */
    (allocfunc)0,                       /* tp_alloc */
    (newfunc)0,                         /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
