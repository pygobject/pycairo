/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * PyCairo - Python bindings for Cairo
 *
 * Copyright © 2003-2004 James Henstridge
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
 *
 * Contributor(s):
 *
 */

#ifndef _PYCAIRO_H_
#define _PYCAIRO_H_

#include <Python.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <cairo-png.h>
#include <cairo-ps.h>

typedef struct {
    PyObject_HEAD
    cairo_matrix_t *matrix;
} PyCairoMatrix;

typedef struct {
    PyObject_HEAD
    cairo_t *ctx;
} PyCairoContext;

typedef struct {
    PyObject_HEAD
    cairo_surface_t *surface;
} PyCairoSurface;

typedef struct {
    PyObject_HEAD
    cairo_pattern_t *pattern;
} PyCairoPattern;

typedef struct {
    PyObject_HEAD
    cairo_font_t *font;
} PyCairoFont;

struct _PyCairo_FunctionStruct {
    int (* check_status)(cairo_status_t status);
    PyTypeObject *matrix_type;
    PyObject *(* matrix_wrap)(cairo_matrix_t *matrix);
    PyTypeObject *surface_type;
    PyObject *(* surface_wrap)(cairo_surface_t *surface);
    PyTypeObject *font_type;
    PyObject *(* font_wrap)(cairo_font_t *font);
    PyTypeObject *context_type;
    PyObject *(* context_wrap)(cairo_t *ctx);
    PyTypeObject *pattern_type;
    PyObject *(* pattern_wrap)(cairo_pattern_t *pattern);
};

#ifndef _INSIDE_PYCAIRO_

#if defined(NO_IMPORT) || defined(NO_IMPORT_PYCAIRO)
extern struct _PyCairo_FunctionStruct *_PyCairo_API;
#else
struct _PyCairo_FunctionStruct *_PyCairo_API;
#endif

#define pycairo_check_status (_PyCairo_API->check_status)
#define PyCairoMatrix_Type  *(_PyCairo_API->matrix_type)
#define pycairo_matrix_wrap  (_PyCairo_API->matrix_wrap)
#define PyCairoSurface_Type *(_PyCairo_API->surface_type)
#define pycairo_surface_wrap (_PyCairo_API->surface_wrap)
#define PyCairoFont_Type    *(_PyCairo_API->font_type)
#define pycairo_font_wrap    (_PyCairo_API->font_wrap)
#define PyCairoContext_Type *(_PyCairo_API->context_type)
#define pycairo_context_wrap (_PyCairo_API->context_wrap)
#define PyCairoPattern_Type *(_PyCairo_API->pattern_type)
#define pycairo_pattern_wrap (_PyCairo_API->pattern_wrap)

#define init_pycairo() { \
    PyObject *pycairo = PyImport_ImportModule("cairo._cairo"); \
    if (pycairo != NULL) { \
        PyObject *module_dict = PyModule_GetDict(pycairo); \
        PyObject *cobject = PyDict_GetItemString(module_dict,"_PyCairo_API"); \
        if (PyCObject_Check(cobject)) \
            _PyCairo_API = (struct _PyCairo_FunctionStruct *) \
                PyCObject_AsVoidPtr(cobject); \
        else { \
            PyErr_SetString(PyExc_RuntimeError, \
                            "could not find _PyCairo_API object"); \
            return; \
        } \
    } else { \
        return; \
    } \
}

#endif

#endif
