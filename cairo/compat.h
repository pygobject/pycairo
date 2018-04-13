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

#ifndef __PYCAIRO_COMPAT_H__
#define __PYCAIRO_COMPAT_H__

#ifndef PyVarObject_HEAD_INIT
    #define PyVarObject_HEAD_INIT(type, size) \
        PyObject_HEAD_INIT(type) size,
#endif

#ifdef __GNUC__
#define PYCAIRO_MODINIT_FUNC __attribute__((visibility("default"))) PyMODINIT_FUNC
#else
#define PYCAIRO_MODINIT_FUNC PyMODINIT_FUNC
#endif

#if PY_MAJOR_VERSION < 3

#define PYCAIRO_MOD_ERROR_VAL
#define PYCAIRO_MOD_SUCCESS_VAL(val)
#define PYCAIRO_MOD_INIT(name) PYCAIRO_MODINIT_FUNC init##name(void)

#define PYCAIRO_PyUnicode_FromString PyString_FromString
#define PYCAIRO_PyUnicode_Format PyString_Format
#define PYCAIRO_PyUnicode_Join _PyString_Join
#define PYCAIRO_PyUnicode_InternFromString PyString_InternFromString
#define PYCAIRO_PyUnicode_Astring PyString_AsString
#define PYCAIRO_PyUnicode_FromFormat PyString_FromFormat

#define PYCAIRO_PyNumber_Long PyNumber_Int

#define PYCAIRO_PyBytes_AsStringAndSize PyString_AsStringAndSize

#define PYCAIRO_PyLong_Type PyInt_Type
#define PYCAIRO_PyLongObject PyIntObject
#define PYCAIRO_PyLong_FromLong PyInt_FromLong
#define PYCAIRO_PyLong_AsLong PyInt_AsLong

#define PYCAIRO_ENC_TEXT_FORMAT "et"
#define PYCAIRO_DATA_FORMAT "s"

#define PYCAIRO_Py_hash_t long

#else

#define PYCAIRO_MOD_ERROR_VAL NULL
#define PYCAIRO_MOD_SUCCESS_VAL(val) val
#define PYCAIRO_MOD_INIT(name) PYCAIRO_MODINIT_FUNC PyInit_##name(void)

#define PYCAIRO_PyUnicode_FromString PyUnicode_FromString
#define PYCAIRO_PyUnicode_Format PyUnicode_Format
#define PYCAIRO_PyUnicode_Join PyUnicode_Join
#define PYCAIRO_PyUnicode_InternFromString PyUnicode_InternFromString
#define PYCAIRO_PyUnicode_Astring _PyUnicode_AsString
#define PYCAIRO_PyUnicode_FromFormat PyUnicode_FromFormat

#define PYCAIRO_PyNumber_Long PyNumber_Long

#define PYCAIRO_PyBytes_AsStringAndSize PyBytes_AsStringAndSize

#define PYCAIRO_PyLong_Type PyLong_Type
#define PYCAIRO_PyLongObject PyLongObject
#define PYCAIRO_PyLong_FromLong PyLong_FromLong
#define PYCAIRO_PyLong_AsLong PyLong_AsLong

#define PYCAIRO_ENC_TEXT_FORMAT "es"
#define PYCAIRO_DATA_FORMAT "y"

#define PYCAIRO_Py_hash_t Py_hash_t

#endif

#define PYCAIRO_Py_hash_t_FromVoidPtr(p) ((PYCAIRO_Py_hash_t)(Py_ssize_t)(p))

#endif /* __PYCAIRO_COMPAT_H__ */
