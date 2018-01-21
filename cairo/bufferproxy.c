/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
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

#if PY_MAJOR_VERSION >= 3

typedef struct {
    PyObject_HEAD
    PyObject *exporter;
    void *buf;
    Py_ssize_t len;
    int readonly;
} Pycairo_BufferProxy;

static PyTypeObject Pycairo_BufferProxyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cairo._BufferProxy",
    sizeof(Pycairo_BufferProxy),
};

static int
buffer_proxy_getbuffer(PyObject *exporter, Py_buffer *view, int flags)
{
    Pycairo_BufferProxy *self = (Pycairo_BufferProxy *)exporter;

    return PyBuffer_FillInfo (view, exporter, self->buf, self->len,
                              self->readonly, flags);
}

static PyBufferProcs Pycairo_BufferProxy_as_buffer = {
    (getbufferproc)buffer_proxy_getbuffer,
    (releasebufferproc)0,
};

PyObject *
buffer_proxy_create_view(PyObject *exporter, void *buf, Py_ssize_t len,
                         int readonly) {
    PyObject *memoryview;
    PyObject *obj;
    Pycairo_BufferProxy *self;

    obj = (PyObject *)PyObject_GC_New(Pycairo_BufferProxy,
                                      &Pycairo_BufferProxyType);
    if (obj == NULL)
        return NULL;

    self = (Pycairo_BufferProxy *)obj;
    Py_INCREF(exporter);
    self->exporter = exporter;
    self->buf = buf;
    self->len = len;
    self->readonly = readonly;
    PyObject_GC_Track(obj);

    memoryview = PyMemoryView_FromObject (obj);
    Py_DECREF(obj);

    return memoryview;
}

static int
buffer_proxy_traverse(PyObject* obj, visitproc visit, void *arg)
{
    Pycairo_BufferProxy *self = (Pycairo_BufferProxy *)obj;

    Py_VISIT(self->exporter);
    return 0;
}

static int
buffer_proxy_clear(PyObject *obj)
{
    Pycairo_BufferProxy *self = (Pycairo_BufferProxy *)obj;

    Py_CLEAR(self->exporter);
    return 0;
}

static void
buffer_proxy_dealloc(PyObject* obj)
{
    Pycairo_BufferProxy *self = (Pycairo_BufferProxy *)obj;

    PyObject_GC_UnTrack(obj);

    self->buf = NULL;
    self->len = 0;
    self->readonly = 0;
    buffer_proxy_clear(obj);

    Py_TYPE(obj)->tp_free(obj);
}
#endif

int
init_buffer_proxy (void) {
#if PY_MAJOR_VERSION >= 3
    Pycairo_BufferProxyType.tp_as_buffer = &Pycairo_BufferProxy_as_buffer;
    Pycairo_BufferProxyType.tp_dealloc = &buffer_proxy_dealloc;
    Pycairo_BufferProxyType.tp_traverse = &buffer_proxy_traverse;
    Pycairo_BufferProxyType.tp_clear = &buffer_proxy_clear;
    Pycairo_BufferProxyType.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC;

    if (PyType_Ready(&Pycairo_BufferProxyType) < 0)
        return -1;
#endif
    return 0;
}
