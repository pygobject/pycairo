/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2003-2005 James Henstridge
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

#ifndef _PYCAIRO_PRIVATE_H_
#define _PYCAIRO_PRIVATE_H_

#ifdef _PYCAIRO_H_
#  error "don't include pycairo.h and pycairo-private.h together"
#endif

#define _INSIDE_PYCAIRO_
#include <Python.h>

#include "pycairo.h"


extern PyTypeObject PycairoContext_Type;
PyObject *PycairoContext_FromContext (cairo_t *ctx, PyTypeObject *type,
				      PyObject *base);

extern PyTypeObject PycairoFontFace_Type;
PyObject *PycairoFontFace_FromFontFace (cairo_font_face_t *font_face);

extern PyTypeObject PycairoFontOptions_Type;
PyObject *PycairoFontOptions_FromFontOptions (
                                          cairo_font_options_t *font_options);

extern PyTypeObject PycairoMatrix_Type;
PyObject *PycairoMatrix_FromMatrix (const cairo_matrix_t *matrix);

extern PyTypeObject PycairoPath_Type;
PyObject *PycairoPath_FromPath (cairo_path_t *path);

extern PyTypeObject PycairoPattern_Type;
PyObject *PycairoPattern_FromPattern (cairo_pattern_t *pattern);

extern PyTypeObject PycairoScaledFont_Type;
PyObject *PycairoScaledFont_FromScaledFont (cairo_scaled_font_t *scaled_font);

extern PyTypeObject PycairoSurface_Type;
extern PyTypeObject PycairoImageSurface_Type;

#if CAIRO_HAS_PDF_SURFACE
extern PyTypeObject PycairoPDFSurface_Type;
#endif

#if CAIRO_HAS_PS_SURFACE
extern PyTypeObject PycairoPSSurface_Type;
#endif

#if CAIRO_HAS_WIN32_SURFACE
extern PyTypeObject PycairoWin32Surface_Type;
#endif

PyObject *PycairoSurface_FromSurface (cairo_surface_t *surface,  
				      PyTypeObject *type,
				      PyObject *base);

int Pycairo_Check_Status (cairo_status_t status);

/* useful macros from Python 2.4 */
#if PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION < 4
#  define Py_RETURN_NONE return Py_INCREF(Py_None), Py_None
#  define Py_RETURN_TRUE return Py_INCREF(Py_True), Py_True
#  define Py_RETURN_FALSE return Py_INCREF(Py_False), Py_False
#  define Py_CLEAR(op)				\
        do {                            	\
                if (op) {			\
                        PyObject *tmp = (PyObject *)(op);	\
                        (op) = NULL;		\
                        Py_DECREF(tmp);		\
                }				\
        } while (0)
#endif /* PY_MAJOR_VERSION */

#endif /* _PYCAIRO_PRIVATE_H_ */
