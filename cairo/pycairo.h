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

#ifndef _PYCAIRO_H_
#define _PYCAIRO_H_

#include <Python.h>

#include <cairo.h>


typedef struct {
    PyObject_HEAD
    cairo_t *ctx;
    PyObject *base; /* base object used to create context, or NULL */
} PycairoContext;

typedef struct {
    PyObject_HEAD
    cairo_font_face_t *font_face;
} PycairoFontFace;

typedef struct {
    PyObject_HEAD
    cairo_matrix_t matrix;
} PycairoMatrix;

typedef struct {
    PyObject_HEAD
    cairo_path_t *path;
} PycairoPath;

typedef struct {
    PyObject_HEAD
    cairo_pattern_t *pattern;
} PycairoPattern;

typedef struct {
    PyObject_HEAD
    cairo_scaled_font_t *scaled_font;
} PycairoScaledFont;

typedef struct {
    PyObject_HEAD
    cairo_surface_t *surface;
    PyObject *base; /* base object used to create surface, or NULL */
} PycairoSurface;

#define PycairoImageSurface PycairoSurface
#define PycairoPDFSurface   PycairoSurface
#define PycairoPSSurface    PycairoSurface
#define PycairoWin32Surface PycairoSurface

/* get C object out of the Python wrapper */
#define PycairoContext_GET(obj)    (((PycairoContext *)(obj))->ctx)

/* Define structure for C API. */
typedef struct {
    /* (type object, constructor) pairs */
    PyTypeObject *Context_Type;
    PyObject *(*Context_FromContext)(cairo_t *ctx, PyTypeObject *type,
				     PyObject *base);
    PyTypeObject *FontFace_Type;
    PyObject *(*FontFace_FromFontFace)(cairo_font_face_t *font_face);
    PyTypeObject *Matrix_Type;
    PyObject *(*Matrix_FromMatrix)(const cairo_matrix_t *matrix);
    PyTypeObject *Path_Type;
    PyObject *(*Path_FromPath)(cairo_path_t *path);
    PyTypeObject *Pattern_Type;
    PyObject *(*Pattern_FromPattern)(cairo_pattern_t *pattern);
    PyTypeObject *ScaledFont_Type;
    PyObject *(*ScaledFont_FromScaledFont)(cairo_scaled_font_t *scaled_font);

    PyTypeObject *Surface_Type;
    PyTypeObject *ImageSurface_Type;
    PyTypeObject *PDFSurface_Type;
    PyTypeObject *PSSurface_Type;
    PyTypeObject *Win32Surface_Type;
    PyObject *(*Surface_FromSurface)(cairo_surface_t *surface, 
				     PyTypeObject *type, PyObject *base);

    /* misc functions */
    int (*Check_Status)(cairo_status_t status);
} Pycairo_CAPI_t;


#ifndef _INSIDE_PYCAIRO_

/* Macros for accessing the C API */
#define PycairoContext_Type         *(Pycairo_CAPI->Context_Type)
#define PycairoContext_FromContext   (Pycairo_CAPI->Context_FromContext)
#define PycairoFontFace_Type        *(Pycairo_CAPI->Fontface_Type)
#define PycairoFontFace_FromFontFace (Pycairo_CAPI->Fontface_FromFontFace)
#define PycairoMatrix_Type          *(Pycairo_CAPI->Matrix_Type)
#define PycairoMatrix_FromMatrix     (Pycairo_CAPI->Matrix_FromMatrix)
#define PycairoPath_Type            *(Pycairo_CAPI->Path_Type)
#define PycairoPath_FromPath         (Pycairo_CAPI->Path_FromPath)
#define PycairoPattern_Type         *(Pycairo_CAPI->Pattern_Type)
#define PycairoPattern_FromPattern   (Pycairo_CAPI->Pattern_FromPattern)
#define PycairoScaledFont_Type      *(Pycairo_CAPI->ScaledFont_Type)
#define PycairoScaledFont_FromScaledFont \
        (Pycairo_CAPI->ScaledFont_FromScaledFont)

#define PycairoSurface_Type         *(Pycairo_CAPI->Surface_Type)
#define PycairoImageSurface_Type    *(Pycairo_CAPI->ImageSurface_Type)

#if CAIRO_HAS_PDF_SURFACE
#define PycairoPDFSurface_Type      *(Pycairo_CAPI->PDFSurface_Type)
#endif

#if CAIRO_HAS_PS_SURFACE
#define PycairoPSSurface_Type       *(Pycairo_CAPI->PSSurface_Type)
#endif

#if CAIRO_HAS_WIN32_SURFACE
#define PycairoWin32Surface_Type    *(Pycairo_CAPI->Win32Surface_Type)
#endif

#define PycairoSurface_FromSurface   (Pycairo_CAPI->Surface_FromSurface)

#define Pycairo_Check_Status         (Pycairo_CAPI->Check_Status)


/* To access the Pycairo C API, edit the client module file to:
 * 1) Add the following line to define a global variable for the C API
 *    static Pycairo_CAPI_t *Pycairo_CAPI;
 * 2) Add 'Pycairo_IMPORT;' to the init<module> function
 */
#define Pycairo_IMPORT \
        Pycairo_CAPI = (Pycairo_CAPI_t*) PyCObject_Import("cairo", "CAPI")

#endif /* ifndef _INSIDE_PYCAIRO_ */

#endif /* ifndef _PYCAIRO_H_ */
