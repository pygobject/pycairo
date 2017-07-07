/* Pycairo - Python bindings for cairo
 *
 * Copyright © 2017 Christoph Reiter
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


typedef struct {
    PyObject_HEAD
    cairo_device_t *device;
} PycairoDevice;

static void
device_dealloc(PycairoDevice *obj) {
    if (obj->device) {
        cairo_device_destroy(obj->device);
        obj->device = NULL;
    }
    Py_TYPE(obj)->tp_free(obj);
}

static PyObject *
device_finish (PycairoDevice *obj) {
    cairo_device_finish (obj->device);
    RETURN_NULL_IF_CAIRO_DEVICE_ERROR(obj->device);
    Py_RETURN_NONE;
}

static PyObject *
device_flush (PycairoDevice *obj) {
    cairo_device_flush (obj->device);
    RETURN_NULL_IF_CAIRO_DEVICE_ERROR(obj->device);
    Py_RETURN_NONE;
}

static PyObject *
device_acquire (PycairoDevice *obj) {
    cairo_status_t status;

    Py_BEGIN_ALLOW_THREADS;
    status = cairo_device_acquire (obj->device);
    Py_END_ALLOW_THREADS;

    RETURN_NULL_IF_CAIRO_ERROR (status);
    Py_RETURN_NONE;
}

static PyObject *
device_release (PycairoDevice *obj) {
    cairo_device_release (obj->device);
    Py_RETURN_NONE;
}

static PyMethodDef device_methods[] = {
    {"finish",         (PyCFunction)device_finish,             METH_NOARGS},
    {"flush",          (PyCFunction)device_flush,              METH_NOARGS},
    {"acquire",        (PyCFunction)device_acquire,            METH_NOARGS},
    {"release",        (PyCFunction)device_release,            METH_NOARGS},
    {NULL, NULL, 0, NULL},
};

static PyObject *
device_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyErr_SetString(PyExc_TypeError,
                    "The Device type cannot be instantiated");
    return NULL;
}

PyTypeObject PycairoDevice_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cairo.Device",                     /* tp_name */
    sizeof(PycairoDevice),              /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)device_dealloc,         /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT                 ,/* tp_flags */
    0,                                  /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    device_methods,                    /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    (newfunc)device_new,                /* tp_new */
};

PyObject *
PycairoDevice_FromDevice (cairo_device_t *device) {
    PyObject *obj;

    assert (device != NULL);

    if (Pycairo_Check_Status (cairo_device_status (device))) {
        cairo_device_destroy (device);
        return NULL;
    }

    obj = PycairoDevice_Type.tp_alloc (&PycairoDevice_Type, 0);
    if (obj) {
        ((PycairoDevice *)obj)->device = device;
    } else {
        cairo_device_destroy (device);
    }
    return obj;
}
