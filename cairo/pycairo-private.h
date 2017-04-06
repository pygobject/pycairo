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


extern PyObject *CairoError;

char * __PyBaseString_AsUTF8 (PyObject *o);

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

extern PyTypeObject PycairoPathiter_Type;

extern PyTypeObject PycairoPattern_Type;
extern PyTypeObject PycairoSolidPattern_Type;
extern PyTypeObject PycairoSurfacePattern_Type;
extern PyTypeObject PycairoGradient_Type;
extern PyTypeObject PycairoLinearGradient_Type;
extern PyTypeObject PycairoRadialGradient_Type;
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

#if CAIRO_HAS_SVG_SURFACE
extern PyTypeObject PycairoSVGSurface_Type;
#endif

#if CAIRO_HAS_WIN32_SURFACE
extern PyTypeObject PycairoWin32Surface_Type;
#endif

#if CAIRO_HAS_XLIB_SURFACE
extern PyTypeObject PycairoXlibSurface_Type;
#endif

PyObject *PycairoSurface_FromSurface (cairo_surface_t *surface,
                                      PyObject *base);

int Pycairo_Check_Status (cairo_status_t status);

/* Python 2.4 compatibility */
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

/* Python 2.5 compatibility */
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
typedef inquiry lenfunc;
typedef intargfunc ssizeargfunc;
typedef intobjargproc ssizeobjargproc;
typedef getreadbufferproc readbufferproc;
typedef getwritebufferproc writebufferproc;
typedef getsegcountproc segcountproc;
typedef getcharbufferproc charbufferproc;
#endif

/* error checking macros */
#define RETURN_NULL_IF_CAIRO_ERROR(status)    \
    do {                                      \
	if (status != CAIRO_STATUS_SUCCESS) { \
	    Pycairo_Check_Status (status);    \
            return NULL;		      \
	}                                     \
    } while (0)

#define RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(ctx) \
    do {                                        \
	cairo_status_t status = cairo_status (ctx); \
	if (status != CAIRO_STATUS_SUCCESS) {   \
	    Pycairo_Check_Status (status);      \
            return NULL;		        \
	}                                       \
    } while (0)

#define RETURN_NULL_IF_CAIRO_PATTERN_ERROR(pattern) \
    do {                                         \
	cairo_status_t status = cairo_pattern_status (pattern); \
	if (status != CAIRO_STATUS_SUCCESS) {    \
	    Pycairo_Check_Status (status);       \
            return NULL;		         \
	}                                        \
    } while (0)

#define RETURN_NULL_IF_CAIRO_SURFACE_ERROR(surface) \
    do {                                         \
	cairo_status_t status = cairo_surface_status (surface); \
	if (status != CAIRO_STATUS_SUCCESS) {    \
	    Pycairo_Check_Status (status);       \
            return NULL;		         \
	}                                        \
    } while (0)

#define RETURN_NULL_IF_CAIRO_SCALED_FONT_ERROR(sc_font) \
    do {                                         \
	cairo_status_t status = cairo_scaled_font_status (sc_font); \
	if (status != CAIRO_STATUS_SUCCESS) {    \
	    Pycairo_Check_Status (status);       \
            return NULL;		         \
	}                                        \
    } while (0)

#define RETURN_NULL_IF_CAIRO_FONT_OPTIONS_ERROR(fo) \
    do {                                         \
	cairo_status_t status = cairo_font_options_status (fo); \
	if (status != CAIRO_STATUS_SUCCESS) {    \
	    Pycairo_Check_Status (status);       \
            return NULL;		         \
	}                                        \
    } while (0)


#endif /* _PYCAIRO_PRIVATE_H_ */
