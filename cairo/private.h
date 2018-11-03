/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2003 James Henstridge
 * Copyright © 2004-2011 Steven Chaplin
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
#include "compat.h"

#define PYCAIRO_STRINGIFY(s) PYCAIRO_STRINGIFY_ARG(s)
#define PYCAIRO_STRINGIFY_ARG(s) #s

int init_error(PyObject *module);

int Pycairo_fspath_converter (PyObject *obj, char** result);
int Pycairo_fspath_none_converter (PyObject *obj, char** result);
int Pycairo_writer_converter (PyObject *obj, PyObject** file);
int Pycairo_reader_converter (PyObject *obj, PyObject** file);
int Pycairo_is_fspath (PyObject *obj);

cairo_glyph_t * _PycairoGlyphs_AsGlyphs (PyObject *py_object, int *num_glyphs);
int _PyGlyph_AsGlyph (PyObject *pyobj, cairo_glyph_t *glyph);
int _PyTextCluster_AsTextCluster (PyObject *pyobj,
                                  cairo_text_cluster_t *cluster);

int _conv_pyobject_to_ulong (PyObject *pyobj, unsigned long *result);

PyObject* Pycairo_richcompare (void* a, void *b, int op);

extern PyTypeObject PycairoContext_Type;
PyObject *PycairoContext_FromContext (cairo_t *ctx, PyTypeObject *type,
				      PyObject *base);

extern PyTypeObject PycairoFontFace_Type;
extern PyTypeObject PycairoToyFontFace_Type;
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
PyObject *PycairoPattern_FromPattern (cairo_pattern_t *pattern,
				      PyObject *base);

typedef PycairoPattern PycairoMeshPattern;
extern PyTypeObject PycairoMeshPattern_Type;

typedef PycairoPattern PycairoRasterSourcePattern;
extern PyTypeObject PycairoRasterSourcePattern_Type;

extern PyTypeObject PycairoRectangleInt_Type;
PyObject *PycairoRectangleInt_FromRectangleInt (
    const cairo_rectangle_int_t *rectangle_int);

extern PyTypeObject PycairoRegion_Type;
PyObject *PycairoRegion_FromRegion (cairo_region_t *region);

extern PyTypeObject PycairoDevice_Type;
PyObject *PycairoDevice_FromDevice (cairo_device_t *device);
extern PyTypeObject PycairoScriptDevice_Type;

extern PyTypeObject PycairoScaledFont_Type;
PyObject *PycairoScaledFont_FromScaledFont (cairo_scaled_font_t *scaled_font);

extern PyTypeObject PycairoSurface_Type;
extern PyTypeObject PycairoImageSurface_Type;
extern PyTypeObject PycairoMappedImageSurface_Type;

extern PyTypeObject PycairoGlyph_Type;
typedef PyTupleObject PycairGlyph;

extern PyTypeObject PycairoRectangle_Type;
typedef PyTupleObject PycairRectangle;

extern PyTypeObject PycairoTextCluster_Type;
typedef PyTupleObject PycairTextCluster;

extern PyTypeObject PycairoTextExtents_Type;
typedef PyTupleObject PycairoTextExtents;

typedef struct {
    PyObject_HEAD
    cairo_device_t *device;
} PycairoDevice;

#ifdef CAIRO_HAS_SCRIPT_SURFACE
typedef PycairoDevice PycairoScriptDevice;
extern PyTypeObject PycairoScriptSurface_Type;
#endif

#ifdef CAIRO_HAS_PDF_SURFACE
extern PyTypeObject PycairoPDFSurface_Type;
#endif

#ifdef CAIRO_HAS_PS_SURFACE
extern PyTypeObject PycairoPSSurface_Type;
#endif

#ifdef CAIRO_HAS_RECORDING_SURFACE
extern PyTypeObject PycairoRecordingSurface_Type;
#endif

#ifdef CAIRO_HAS_SVG_SURFACE
extern PyTypeObject PycairoSVGSurface_Type;
#endif

#ifdef CAIRO_HAS_WIN32_SURFACE
extern PyTypeObject PycairoWin32Surface_Type;
extern PyTypeObject PycairoWin32PrintingSurface_Type;
#endif

#ifdef CAIRO_HAS_XCB_SURFACE
extern PyTypeObject PycairoXCBSurface_Type;
#ifdef HAVE_XPYB
#  include <xpyb.h>
extern xpyb_CAPI_t *xpyb_CAPI;
extern PyObject *xpybVISUALTYPE_type;
#endif
#endif

#ifdef CAIRO_HAS_XLIB_SURFACE
extern PyTypeObject PycairoXlibSurface_Type;
#endif

#ifdef CAIRO_HAS_TEE_SURFACE
typedef PycairoSurface PycairoTeeSurface;
extern PyTypeObject PycairoTeeSurface_Type;
#endif

PyObject *PycairoSurface_FromSurface (cairo_surface_t *surface,
                                      PyObject *base);

int Pycairo_Check_Status (cairo_status_t status);

/* error checking macros */
#define RETURN_NULL_IF_CAIRO_ERROR(status)    \
  do {					      \
    if (status != CAIRO_STATUS_SUCCESS) {     \
      Pycairo_Check_Status (status);	      \
      return NULL;			      \
    }					      \
  } while (0)

#define RETURN_NULL_IF_CAIRO_CONTEXT_ERROR(ctx)    \
  do {						   \
    cairo_status_t status = cairo_status (ctx);	   \
    if (status != CAIRO_STATUS_SUCCESS) {	   \
      Pycairo_Check_Status (status);		   \
      return NULL;				   \
    }						   \
  } while (0)

#define RETURN_NULL_IF_CAIRO_PATTERN_ERROR(pattern)             \
  do {								\
    cairo_status_t status = cairo_pattern_status (pattern);	\
    if (status != CAIRO_STATUS_SUCCESS) {			\
      Pycairo_Check_Status (status);				\
      return NULL;						\
    }								\
  } while (0)

#define RETURN_NULL_IF_CAIRO_REGION_ERROR(region)             \
  do {								\
    cairo_status_t status = cairo_region_status (region);	\
    if (status != CAIRO_STATUS_SUCCESS) {			\
      Pycairo_Check_Status (status);				\
      return NULL;						\
    }								\
  } while (0)

#define RETURN_NULL_IF_CAIRO_SURFACE_ERROR(surface)	        \
  do {								\
    cairo_status_t status = cairo_surface_status (surface);	\
    if (status != CAIRO_STATUS_SUCCESS) {			\
      Pycairo_Check_Status (status);				\
      return NULL;						\
    }								\
  } while (0)

#define RETURN_NULL_IF_CAIRO_DEVICE_ERROR(device)	        \
  do {								\
    cairo_status_t status = cairo_device_status (device);	\
    if (status != CAIRO_STATUS_SUCCESS) {			\
      Pycairo_Check_Status (status);				\
      return NULL;						\
    }								\
  } while (0)

#define RETURN_NULL_IF_CAIRO_SCALED_FONT_ERROR(sc_font)             \
  do {								    \
    cairo_status_t status = cairo_scaled_font_status (sc_font);	    \
    if (status != CAIRO_STATUS_SUCCESS) {			    \
      Pycairo_Check_Status (status);				    \
      return NULL;						    \
    }								    \
  } while (0)

#define RETURN_NULL_IF_CAIRO_FONT_OPTIONS_ERROR(fo)	        \
  do {								\
    cairo_status_t status = cairo_font_options_status (fo);	\
    if (status != CAIRO_STATUS_SUCCESS) {			\
      Pycairo_Check_Status (status);				\
      return NULL;						\
    }								\
  } while (0)


PyObject *error_get_type(void);

int init_buffer_proxy(void);

PyObject *buffer_proxy_create_view(PyObject *exporter, void *buf,
                                   Py_ssize_t len, int readonly);

/* int enums */

int init_enums(PyObject *module);
PyObject *int_enum_create(PyTypeObject *type, long value);

#define DECL_ENUM(name) extern PyTypeObject Pycairo_##name##_Type;

/* A defined variant of (int)(unsigned int)(u). The cast from unsigned to
 * signed gives the reverse result of a signed to unsigned cast. */
#define _ENSURE_INT(u) ( \
        ((unsigned int)(u) > INT_MAX) ? \
            -(int)(UINT_MAX - (unsigned int)(u)) - 1 : (int)(unsigned int)(u) \
    )

#define CREATE_INT_ENUM(type_name, value) \
    (int_enum_create(&Pycairo_##type_name##_Type, _ENSURE_INT(value)))

#define RETURN_INT_ENUM(type_name, value) \
    return CREATE_INT_ENUM(type_name, value);

DECL_ENUM(Antialias)
DECL_ENUM(Content)
DECL_ENUM(Extend)
DECL_ENUM(FillRule)
DECL_ENUM(Filter)
DECL_ENUM(FontSlant)
DECL_ENUM(FontWeight)
DECL_ENUM(Format)
DECL_ENUM(HintMetrics)
DECL_ENUM(HintStyle)
DECL_ENUM(LineCap)
DECL_ENUM(LineJoin)
DECL_ENUM(Operator)
DECL_ENUM(PathDataType)
DECL_ENUM(RegionOverlap)
DECL_ENUM(Status)
DECL_ENUM(SubpixelOrder)
DECL_ENUM(TextClusterFlags)
DECL_ENUM(SurfaceObserverMode)
#ifdef CAIRO_HAS_SVG_SURFACE
DECL_ENUM(SVGVersion)
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 15, 10)
DECL_ENUM(SVGUnit)
#endif
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
DECL_ENUM(PDFVersion)
#endif
#ifdef CAIRO_HAS_PS_SURFACE
DECL_ENUM(PSLevel)
#endif
#ifdef CAIRO_HAS_SCRIPT_SURFACE
DECL_ENUM(ScriptMode)
#endif

/* Use to disable deprecation warnings temporarily */
#ifdef _MSC_VER
# define PYCAIRO_BEGIN_IGNORE_DEPRECATED \
  __pragma (warning (push)) \
  __pragma (warning (disable : 4996))

# define PYCAIRO_END_IGNORE_DEPRECATED \
  __pragma (warning (pop))

#else

# define PYCAIRO_BEGIN_IGNORE_DEPRECATED \
  _Pragma ("GCC diagnostic push") \
  _Pragma ("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

# define PYCAIRO_END_IGNORE_DEPRECATED \
  _Pragma ("GCC diagnostic pop")
#endif

#endif /* _PYCAIRO_PRIVATE_H_ */
