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

#include "private.h"

static void
device_dealloc(PycairoDevice *obj) {
    if (obj->device) {
        cairo_device_destroy(obj->device);
        obj->device = NULL;
    }
    Py_TYPE(obj)->tp_free(obj);
}

static PyObject *
device_finish (PycairoDevice *obj, PyObject *ignored) {
    cairo_device_finish (obj->device);
    RETURN_NULL_IF_CAIRO_DEVICE_ERROR(obj->device);
    Py_RETURN_NONE;
}

static PyObject *
device_flush (PycairoDevice *obj, PyObject *ignored) {
    cairo_device_flush (obj->device);
    RETURN_NULL_IF_CAIRO_DEVICE_ERROR(obj->device);
    Py_RETURN_NONE;
}

static PyObject *
device_acquire (PycairoDevice *obj, PyObject *ignored) {
    cairo_status_t status;

    Py_BEGIN_ALLOW_THREADS;
    status = cairo_device_acquire (obj->device);
    Py_END_ALLOW_THREADS;

    RETURN_NULL_IF_CAIRO_ERROR (status);
    Py_RETURN_NONE;
}

static PyObject *
device_release (PycairoDevice *obj, PyObject *ignored) {
    cairo_device_release (obj->device);
    Py_RETURN_NONE;
}

static PyObject *
device_ctx_enter (PyObject *obj, PyObject *ignored) {
    Py_INCREF (obj);
    return obj;
}

static PyObject *
device_ctx_exit (PycairoDevice *obj, PyObject *args) {
    Py_BEGIN_ALLOW_THREADS;
    cairo_device_finish (obj->device);
    Py_END_ALLOW_THREADS;
    Py_RETURN_NONE;
}

static PyMethodDef device_methods[] = {
    {"__enter__",      (PyCFunction)device_ctx_enter,          METH_NOARGS},
    {"__exit__",       (PyCFunction)device_ctx_exit,           METH_VARARGS},
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

static PyObject*
device_richcompare (PyObject *self, PyObject *other, int op)
{
  if (Py_TYPE(self) == Py_TYPE(other))
    return Pycairo_richcompare (
      ((PycairoDevice *)self)->device,
      ((PycairoDevice *)other)->device,
      op);
  else {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
}

static PYCAIRO_Py_hash_t
device_hash (PyObject *self)
{
  return PYCAIRO_Py_hash_t_FromVoidPtr (((PycairoDevice *)self)->device);
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
    device_hash,                        /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,            /* tp_flags */
    0,                                  /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    device_richcompare,                 /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    device_methods,                     /* tp_methods */
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

#ifdef CAIRO_HAS_SCRIPT_SURFACE
#include <cairo-script.h>

static const cairo_user_data_key_t device_base_object_key;

static void
_decref_destroy_func(void *user_data) {
  PyGILState_STATE gstate = PyGILState_Ensure();
  Py_DECREF(user_data);
  PyGILState_Release(gstate);
}

static PyObject *
_device_create_with_object(cairo_device_t *device, PyObject *base) {
    PyObject *pydevice;
    cairo_status_t status;

    pydevice = PycairoDevice_FromDevice(device);
    if (pydevice == NULL)
        return NULL;

    if (base != NULL) {
        status = cairo_device_set_user_data(
            device, &device_base_object_key, base, _decref_destroy_func);
        if (status != CAIRO_STATUS_SUCCESS)
            Py_DECREF(pydevice);
        RETURN_NULL_IF_CAIRO_ERROR(status);
        Py_INCREF(base);
    }

    return pydevice;
}

static cairo_status_t
_write_func (void *closure, const unsigned char *data, unsigned int length) {
    PyGILState_STATE gstate = PyGILState_Ensure();
    PyObject *res = PyObject_CallMethod (
        (PyObject *)closure, "write", "(" PYCAIRO_DATA_FORMAT "#)",
        data, (Py_ssize_t)length);

    if (res == NULL) {
        PyErr_Clear();
        /* an exception has occurred, it will be picked up later by
         * Pycairo_Check_Status()
         */
        PyGILState_Release(gstate);
        return CAIRO_STATUS_WRITE_ERROR;
    }

    Py_DECREF(res);
    PyGILState_Release(gstate);
    return CAIRO_STATUS_SUCCESS;
}

static PyObject *
script_device_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    char *name = NULL;
    PyObject *file;
    cairo_device_t *device;

  if (!PyArg_ParseTuple (args, "O:ScriptDevice.__new__", &file))
    return NULL;

  if (Pycairo_is_fspath (file)) {
    if (!PyArg_ParseTuple (args, "O&:ScriptDevice.__new__",
                           Pycairo_fspath_converter, &name))
      return NULL;

    Py_BEGIN_ALLOW_THREADS;
    device = cairo_script_create (name);
    Py_END_ALLOW_THREADS;
    PyMem_Free (name);
    return PycairoDevice_FromDevice (device);
  } else {
    if (PyArg_ParseTuple (args, "O&:ScriptDevice.__new__",
                          Pycairo_writer_converter, &file)) {
        Py_BEGIN_ALLOW_THREADS;
        device = cairo_script_create_for_stream (_write_func, file);
        Py_END_ALLOW_THREADS;
        return _device_create_with_object (device, file);
    } else {
        PyErr_Clear ();
        PyErr_SetString (PyExc_TypeError,
                         "ScriptDevice takes one argument which must be "
                         "a filename, file object, or a file-like object "
                         "which has a \"write\" method (like StringIO)");
        return NULL;
    }
  }
}

static PyObject *
script_device_get_mode (PycairoDevice *obj, PyObject *ignored) {
    RETURN_INT_ENUM (ScriptMode, cairo_script_get_mode (obj->device));
}

static PyObject *
script_device_set_mode (PycairoDevice *obj, PyObject *args) {
    cairo_script_mode_t mode;
    int mode_arg;

    if (!PyArg_ParseTuple (args, "i:ScriptDevice.set_mode", &mode_arg))
        return NULL;

    mode = (cairo_script_mode_t)mode_arg;

    Py_BEGIN_ALLOW_THREADS;
    cairo_script_set_mode (obj->device, mode);
    Py_END_ALLOW_THREADS;

    RETURN_NULL_IF_CAIRO_DEVICE_ERROR (obj->device);
    Py_RETURN_NONE;
}

static PyObject *
script_device_write_comment (PycairoDevice *obj, PyObject *args) {
    const char *comment;

    if (!PyArg_ParseTuple(args, "s:ScriptDevice.write_comment", &comment))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    cairo_script_write_comment (obj->device, comment, -1);
    Py_END_ALLOW_THREADS;

    RETURN_NULL_IF_CAIRO_DEVICE_ERROR(obj->device);
    Py_RETURN_NONE;
}

#ifdef CAIRO_HAS_RECORDING_SURFACE
static PyObject *
script_device_from_recording_surface (PycairoDevice *obj, PyObject *args) {
    PyObject *pysurface;
    cairo_status_t status;

    if (!PyArg_ParseTuple(args, "O!:ScriptDevice.from_recording_surface",
                          &PycairoRecordingSurface_Type, &pysurface))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    status = cairo_script_from_recording_surface (obj->device, ((PycairoRecordingSurface*)pysurface)->surface);
    Py_END_ALLOW_THREADS;

    RETURN_NULL_IF_CAIRO_ERROR(status);
    Py_RETURN_NONE;
}
#endif

static PyMethodDef script_device_methods[] = {
    {"get_mode",      (PyCFunction)script_device_get_mode,       METH_NOARGS},
    {"set_mode",      (PyCFunction)script_device_set_mode,       METH_VARARGS},
    {"write_comment", (PyCFunction)script_device_write_comment,  METH_VARARGS},
#ifdef CAIRO_HAS_RECORDING_SURFACE
    {"from_recording_surface",
        (PyCFunction)script_device_from_recording_surface,  METH_VARARGS},
#endif
    {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoScriptDevice_Type = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "cairo.ScriptDevice",               /* tp_name */
  sizeof(PycairoScriptDevice),        /* tp_basicsize */
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
  script_device_methods,              /* tp_methods */
  0,                                  /* tp_members */
  0,                                  /* tp_getset */
  &PycairoDevice_Type,                /* tp_base */
  0,                                  /* tp_dict */
  0,                                  /* tp_descr_get */
  0,                                  /* tp_descr_set */
  0,                                  /* tp_dictoffset */
  0,                                  /* tp_init */
  0,                                  /* tp_alloc */
  (newfunc)script_device_new,         /* tp_new */
  0,                                  /* tp_free */
  0,                                  /* tp_is_gc */
  0,                                  /* tp_bases */
};

#endif

PyObject *
PycairoDevice_FromDevice (cairo_device_t *device) {
    PyObject *obj;
    PyTypeObject *type;

    assert (device != NULL);

    if (Pycairo_Check_Status (cairo_device_status (device))) {
        cairo_device_destroy (device);
        return NULL;
    }

    switch (cairo_device_get_type (device)) {
#ifdef CAIRO_HAS_SCRIPT_SURFACE
        case CAIRO_DEVICE_TYPE_SCRIPT:
            type = &PycairoScriptDevice_Type;
            break;
#endif
        default:
            type = &PycairoDevice_Type;
            break;
    }

    obj = type->tp_alloc (type, 0);
    if (obj) {
        ((PycairoDevice *)obj)->device = device;
    } else {
        cairo_device_destroy (device);
    }
    return obj;
}
