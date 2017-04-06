/* -*- mode: C; c-basic-offset: 4 -*- */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo.h"
#include <Numeric/arrayobject.h>

static PyObject *
surface_create_for_array(PyObject *self, PyObject *args)
{
    PyArrayObject *array;
    cairo_format_t format;
    int width, height, stride;
    char *data;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!:surface_create_for_array",
			  &PyArray_Type, &array))
	return NULL;

    if (array->descr->type_num != PyArray_UBYTE) {
	PyErr_SetString(PyExc_TypeError, "array data must be unsigned bytes");
	return NULL;
    }
    if (array->nd < 2) {
	PyErr_SetString(PyExc_TypeError,
			"array must have at least two dimensions");
	return NULL;
    }
    width = array->dimensions[1];
    height = array->dimensions[0];
    stride = array->strides[0];
    data = (char *)array->data;

    if (array->nd == 2 || (array->nd == 3 && array->dimensions[2] == 1)) {
	if (array->strides[1] != 1) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_A8;
    } else if (array->nd == 3 && array->dimensions[2] == 3) {
	if (array->strides[1] != 3) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_RGB24;
    } else if (array->nd == 3 && array->dimensions[2] == 4) {
	if (array->strides[1] != 4) {
	    PyErr_SetString(PyExc_TypeError, "second axis must be contiguous");
	    return NULL;
	}
	format = CAIRO_FORMAT_ARGB32;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"array must be MxN or MxNxP where P is 1, 3 or 4");
	return NULL;
    }

    surface = cairo_surface_create_for_image(data, format,
					     width, height, stride);
    if (!surface)
	return PyErr_NoMemory();
    /* should get surface to hold reference to array ... */

    return pycairo_surface_new(surface);
}

static PyMethodDef caironumpy_functions[] = {
    { "surface_create_for_array", (PyCFunction)surface_create_for_array,
      METH_VARARGS },
    { NULL, NULL, 0 }
};

DL_EXPORT(void)
initnumpy(void)
{
    PyObject *mod;

    mod = Py_InitModule("cairo.numpy", caironumpy_functions);

    init_pycairo();
    import_array();
}
