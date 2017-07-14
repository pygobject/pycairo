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

#include "config.h"
#include "private.h"


PyObject*
Pycairo_richcompare (void* a, void *b, int op)
{
    PyObject *res;

    switch (op) {
        case Py_EQ:
            res = (a == b) ? Py_True : Py_False;
            break;
      case Py_NE:
            res = (a != b) ? Py_True : Py_False;
            break;
      case Py_LT:
            res = (a < b) ? Py_True : Py_False;
            break;
      case Py_LE:
            res = (a <= b) ? Py_True : Py_False;
            break;
      case Py_GT:
            res = (a > b) ? Py_True : Py_False;
            break;
      case Py_GE:
            res = (a >= b) ? Py_True : Py_False;
            break;
      default:
            res = Py_NotImplemented;
            break;
    }

    Py_INCREF (res);
    return res;
}
