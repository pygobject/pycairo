/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2003 James Henstridge, Steven Chaplin
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

/* to read CAIRO_PS_LEVEL_* constants */
#ifdef CAIRO_HAS_PS_SURFACE
#  include <cairo-ps.h>
#endif

/* for XCB api */
#if defined(CAIRO_HAS_XCB_SURFACE) && defined(HAVE_XPYB)
xpyb_CAPI_t *xpyb_CAPI;
PyObject *xpybVISUALTYPE_type;
#endif

/* A module specific exception */
PyObject *CairoError = NULL;

int
Pycairo_Check_Status (cairo_status_t status) {
  if (PyErr_Occurred() != NULL)
    return 1;

  switch (status) {
  case CAIRO_STATUS_SUCCESS:
    return 0;
    /* if appropriate - translate the status string into Python,
     * else - use cairo_status_to_string()
     */
  case CAIRO_STATUS_NO_MEMORY:
    PyErr_NoMemory();
    break;
  case CAIRO_STATUS_READ_ERROR:
  case CAIRO_STATUS_WRITE_ERROR:
    PyErr_SetString(PyExc_IOError, cairo_status_to_string (status));
    break;
  case CAIRO_STATUS_INVALID_RESTORE:
    PyErr_SetString(CairoError, "Context.restore without matching "
		    "Context.save");
    break;
  case CAIRO_STATUS_INVALID_POP_GROUP:
    PyErr_SetString(CairoError, "Context.pop_group without matching "
		    "Context.push_group");
    break;
  default:
    PyErr_SetString(CairoError, cairo_status_to_string (status));
  }
  return 1;
}


/* C API.  Clients get at this via Pycairo_IMPORT, defined in pycairo.h.
 */
static Pycairo_CAPI_t CAPI = {
  &PycairoContext_Type,
  PycairoContext_FromContext,

  &PycairoFontFace_Type,
  &PycairoToyFontFace_Type,
  PycairoFontFace_FromFontFace,

  &PycairoFontOptions_Type,
  PycairoFontOptions_FromFontOptions,

  &PycairoMatrix_Type,
  PycairoMatrix_FromMatrix,

  &PycairoPath_Type,
  PycairoPath_FromPath,

  &PycairoPattern_Type,
  &PycairoSolidPattern_Type,
  &PycairoSurfacePattern_Type,
  &PycairoGradient_Type,
  &PycairoLinearGradient_Type,
  &PycairoRadialGradient_Type,
  PycairoPattern_FromPattern,

  &PycairoScaledFont_Type,
  PycairoScaledFont_FromScaledFont,

  &PycairoSurface_Type,
#ifdef CAIRO_HAS_IMAGE_SURFACE
  &PycairoImageSurface_Type,
#else
  0,
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
  &PycairoPDFSurface_Type,
#else
  0,
#endif
#ifdef CAIRO_HAS_PS_SURFACE
  &PycairoPSSurface_Type,
#else
  0,
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
  &PycairoSVGSurface_Type,
#else
  0,
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
  &PycairoWin32Surface_Type,
  &PycairoWin32PrintingSurface_Type,
#else
  0,
  0,
#endif
#ifdef CAIRO_HAS_XCB_SURFACE
  &PycairoXCBSurface_Type,
#else
  0,
#endif
#ifdef CAIRO_HAS_XLIB_SURFACE
  &PycairoXlibSurface_Type,
#else
  0,
#endif
  PycairoSurface_FromSurface,

  Pycairo_Check_Status,
};

static PyObject *
pycairo_cairo_version (PyObject *self) {
  return PyInt_FromLong (cairo_version());
}

static PyObject *
pycairo_cairo_version_string (PyObject *self) {
  return PyString_FromString (cairo_version_string());
}

static PyMethodDef cairo_functions[] = {
  {"cairo_version",    (PyCFunction)pycairo_cairo_version, METH_NOARGS},
  {"cairo_version_string", (PyCFunction)pycairo_cairo_version_string,
   METH_NOARGS},
  {NULL, NULL, 0, NULL},
};


DL_EXPORT(void)
init_cairo(void)
{
  PyObject *m;

  if (PyType_Ready(&PycairoContext_Type) < 0)
    return;
  if (PyType_Ready(&PycairoFontFace_Type) < 0)
    return;
  if (PyType_Ready(&PycairoToyFontFace_Type) < 0)
    return;
  if (PyType_Ready(&PycairoFontOptions_Type) < 0)
    return;
  if (PyType_Ready(&PycairoMatrix_Type) < 0)
    return;
  if (PyType_Ready(&PycairoPath_Type) < 0)
    return;
  PycairoPathiter_Type.tp_iter=&PyObject_SelfIter;
  if (PyType_Ready(&PycairoPathiter_Type) < 0)
    return;

  if (PyType_Ready(&PycairoPattern_Type) < 0)
    return;
  if (PyType_Ready(&PycairoSolidPattern_Type) < 0)
    return;
  if (PyType_Ready(&PycairoSurfacePattern_Type) < 0)
    return;
  if (PyType_Ready(&PycairoGradient_Type) < 0)
    return;
  if (PyType_Ready(&PycairoLinearGradient_Type) < 0)
    return;
  if (PyType_Ready(&PycairoRadialGradient_Type) < 0)
    return;

  if (PyType_Ready(&PycairoScaledFont_Type) < 0)
    return;

  if (PyType_Ready(&PycairoSurface_Type) < 0)
    return;
#ifdef CAIRO_HAS_IMAGE_SURFACE
  if (PyType_Ready(&PycairoImageSurface_Type) < 0)
    return;
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
  if (PyType_Ready(&PycairoPDFSurface_Type) < 0)
    return;
#endif
#ifdef CAIRO_HAS_PS_SURFACE
  if (PyType_Ready(&PycairoPSSurface_Type) < 0)
    return;
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
  if (PyType_Ready(&PycairoSVGSurface_Type) < 0)
    return;
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
  if (PyType_Ready(&PycairoWin32Surface_Type) < 0)
    return;
  if (PyType_Ready(&PycairoWin32PrintingSurface_Type) < 0)
    return;
#endif
#ifdef CAIRO_HAS_XCB_SURFACE
  if (PyType_Ready(&PycairoXCBSurface_Type) < 0)
    return;
#endif
#ifdef CAIRO_HAS_XLIB_SURFACE
  if (PyType_Ready(&PycairoXlibSurface_Type) < 0)
    return;
#endif

  m = Py_InitModule("cairo._cairo", cairo_functions);

  PyModule_AddStringConstant(m, "version", VERSION);
  PyModule_AddObject(m, "version_info",
		     Py_BuildValue("(iii)",
				   PYCAIRO_VERSION_MAJOR,
				   PYCAIRO_VERSION_MINOR,
				   PYCAIRO_VERSION_MICRO
				   ));

  Py_INCREF(&PycairoContext_Type);
  PyModule_AddObject(m, "Context", (PyObject *)&PycairoContext_Type);
  Py_INCREF(&PycairoFontFace_Type);
  PyModule_AddObject(m, "FontFace",(PyObject *)&PycairoFontFace_Type);
  Py_INCREF(&PycairoToyFontFace_Type);
  PyModule_AddObject(m, "ToyFontFace",(PyObject *)&PycairoToyFontFace_Type);
  Py_INCREF(&PycairoFontOptions_Type);
  PyModule_AddObject(m, "FontOptions",(PyObject *)&PycairoFontOptions_Type);
  Py_INCREF(&PycairoMatrix_Type);
  PyModule_AddObject(m, "Matrix",  (PyObject *)&PycairoMatrix_Type);
  Py_INCREF(&PycairoPath_Type);
  /* Don't add Path object since it is not accessed directly as 'cairo.Path'
   * PyModule_AddObject(m, "Path", (PyObject *)&PycairoPath_Type);
   */
  Py_INCREF(&PycairoPattern_Type);
  PyModule_AddObject(m, "Pattern", (PyObject *)&PycairoPattern_Type);
  Py_INCREF(&PycairoSolidPattern_Type);
  PyModule_AddObject(m, "SolidPattern",
		     (PyObject *)&PycairoSolidPattern_Type);
  Py_INCREF(&PycairoSurfacePattern_Type);
  PyModule_AddObject(m, "SurfacePattern",
		     (PyObject *)&PycairoSurfacePattern_Type);
  Py_INCREF(&PycairoGradient_Type);
  PyModule_AddObject(m, "Gradient", (PyObject *)&PycairoGradient_Type);
  Py_INCREF(&PycairoLinearGradient_Type);
  PyModule_AddObject(m, "LinearGradient",
		     (PyObject *)&PycairoLinearGradient_Type);
  Py_INCREF(&PycairoRadialGradient_Type);
  PyModule_AddObject(m, "RadialGradient",
		     (PyObject *)&PycairoRadialGradient_Type);

  Py_INCREF(&PycairoScaledFont_Type);
  PyModule_AddObject(m, "ScaledFont", (PyObject *)&PycairoScaledFont_Type);

  Py_INCREF(&PycairoSurface_Type);
  PyModule_AddObject(m, "Surface", (PyObject *)&PycairoSurface_Type);

#ifdef CAIRO_HAS_IMAGE_SURFACE
  Py_INCREF(&PycairoImageSurface_Type);
  PyModule_AddObject(m, "ImageSurface",
		     (PyObject *)&PycairoImageSurface_Type);
#endif

#ifdef CAIRO_HAS_PDF_SURFACE
  Py_INCREF(&PycairoPDFSurface_Type);
  PyModule_AddObject(m, "PDFSurface", (PyObject *)&PycairoPDFSurface_Type);
#endif

#ifdef CAIRO_HAS_PS_SURFACE
  Py_INCREF(&PycairoPSSurface_Type);
  PyModule_AddObject(m, "PSSurface", (PyObject *)&PycairoPSSurface_Type);
#endif

#ifdef CAIRO_HAS_SVG_SURFACE
  Py_INCREF(&PycairoSVGSurface_Type);
  PyModule_AddObject(m, "SVGSurface", (PyObject *)&PycairoSVGSurface_Type);
#endif

#ifdef CAIRO_HAS_WIN32_SURFACE
  Py_INCREF(&PycairoWin32Surface_Type);
  PyModule_AddObject(m, "Win32Surface",
		     (PyObject *)&PycairoWin32Surface_Type);
  Py_INCREF(&PycairoWin32PrintingSurface_Type);
  PyModule_AddObject(m, "Win32PrintingSurface",
		     (PyObject *)&PycairoWin32PrintingSurface_Type);
#endif

#ifdef CAIRO_HAS_XCB_SURFACE
  Py_INCREF(&PycairoXCBSurface_Type);
  PyModule_AddObject(m, "XCBSurface",
		     (PyObject *)&PycairoXCBSurface_Type);
#endif

#ifdef CAIRO_HAS_XLIB_SURFACE
  Py_INCREF(&PycairoXlibSurface_Type);
  PyModule_AddObject(m, "XlibSurface",
		     (PyObject *)&PycairoXlibSurface_Type);
#endif

  PyModule_AddObject(m, "CAPI", PyCObject_FromVoidPtr(&CAPI, NULL));

  /* Add 'cairo.Error' to the module */
  if (CairoError == NULL) {
    CairoError = PyErr_NewException("cairo.Error", NULL, NULL);
    if (CairoError == NULL)
      return;
  }
  Py_INCREF(CairoError);
  if (PyModule_AddObject(m, "Error", CairoError) < 0)
    return;

    /* constants */
#if CAIRO_HAS_ATSUI_FONT
  PyModule_AddIntConstant(m, "HAS_ATSUI_FONT", 1);
#else
  PyModule_AddIntConstant(m, "HAS_ATSUI_FONT", 0);
#endif
#if CAIRO_HAS_FT_FONT
  PyModule_AddIntConstant(m, "HAS_FT_FONT", 1);
#else
  PyModule_AddIntConstant(m, "HAS_FT_FONT", 0);
#endif
#if CAIRO_HAS_GLITZ_SURFACE
  PyModule_AddIntConstant(m, "HAS_GLITZ_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_GLITZ_SURFACE", 0);
#endif
#if CAIRO_HAS_IMAGE_SURFACE
  PyModule_AddIntConstant(m, "HAS_IMAGE_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_IMAGE_SURFACE", 0);
#endif
#if CAIRO_HAS_PDF_SURFACE
  PyModule_AddIntConstant(m, "HAS_PDF_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_PDF_SURFACE", 0);
#endif
#if CAIRO_HAS_PNG_FUNCTIONS
  PyModule_AddIntConstant(m, "HAS_PNG_FUNCTIONS", 1);
#else
  PyModule_AddIntConstant(m, "HAS_PNG_FUNCTIONS", 0);
#endif
#if CAIRO_HAS_PS_SURFACE
  PyModule_AddIntConstant(m, "HAS_PS_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_PS_SURFACE", 0);
#endif
#if CAIRO_HAS_SVG_SURFACE
  PyModule_AddIntConstant(m, "HAS_SVG_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_SVG_SURFACE", 0);
#endif
#if CAIRO_HAS_USER_FONT
  PyModule_AddIntConstant(m, "HAS_USER_FONT", 1);
#else
  PyModule_AddIntConstant(m, "HAS_USER_FONT", 0);
#endif
#if CAIRO_HAS_QUARTZ_SURFACE
  PyModule_AddIntConstant(m, "HAS_QUARTZ_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_QUARTZ_SURFACE", 0);
#endif
#if CAIRO_HAS_WIN32_FONT
  PyModule_AddIntConstant(m, "HAS_WIN32_FONT", 1);
#else
  PyModule_AddIntConstant(m, "HAS_WIN32_FONT", 0);
#endif
#if CAIRO_HAS_WIN32_SURFACE
  PyModule_AddIntConstant(m, "HAS_WIN32_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_WIN32_SURFACE", 0);
#endif
#if CAIRO_HAS_XCB_SURFACE
  PyModule_AddIntConstant(m, "HAS_XCB_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_XCB_SURFACE", 0);
#endif
#if CAIRO_HAS_XLIB_SURFACE
  PyModule_AddIntConstant(m, "HAS_XLIB_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_XLIB_SURFACE", 0);
#endif

#define CONSTANT(x) PyModule_AddIntConstant(m, #x, CAIRO_##x)
  CONSTANT(ANTIALIAS_DEFAULT);
  CONSTANT(ANTIALIAS_NONE);
  CONSTANT(ANTIALIAS_GRAY);
  CONSTANT(ANTIALIAS_SUBPIXEL);

  CONSTANT(CONTENT_COLOR);
  CONSTANT(CONTENT_ALPHA);
  CONSTANT(CONTENT_COLOR_ALPHA);

  CONSTANT(EXTEND_NONE);
  CONSTANT(EXTEND_REPEAT);
  CONSTANT(EXTEND_REFLECT);
  CONSTANT(EXTEND_PAD);

  CONSTANT(FILL_RULE_WINDING);
  CONSTANT(FILL_RULE_EVEN_ODD);

  CONSTANT(FILTER_FAST);
  CONSTANT(FILTER_GOOD);
  CONSTANT(FILTER_BEST);
  CONSTANT(FILTER_NEAREST);
  CONSTANT(FILTER_BILINEAR);
  CONSTANT(FILTER_GAUSSIAN);

  CONSTANT(FONT_WEIGHT_NORMAL);
  CONSTANT(FONT_WEIGHT_BOLD);

  CONSTANT(FONT_SLANT_NORMAL);
  CONSTANT(FONT_SLANT_ITALIC);
  CONSTANT(FONT_SLANT_OBLIQUE);

  CONSTANT(FORMAT_ARGB32);
  CONSTANT(FORMAT_RGB24);
  CONSTANT(FORMAT_A8);
  CONSTANT(FORMAT_A1);
  CONSTANT(FORMAT_RGB16_565);

  CONSTANT(HINT_METRICS_DEFAULT);
  CONSTANT(HINT_METRICS_OFF);
  CONSTANT(HINT_METRICS_ON);

  CONSTANT(HINT_STYLE_DEFAULT);
  CONSTANT(HINT_STYLE_NONE);
  CONSTANT(HINT_STYLE_SLIGHT);
  CONSTANT(HINT_STYLE_MEDIUM);
  CONSTANT(HINT_STYLE_FULL);

  CONSTANT(LINE_CAP_BUTT);
  CONSTANT(LINE_CAP_ROUND);
  CONSTANT(LINE_CAP_SQUARE);

  CONSTANT(LINE_JOIN_MITER);
  CONSTANT(LINE_JOIN_ROUND);
  CONSTANT(LINE_JOIN_BEVEL);

  CONSTANT(OPERATOR_CLEAR);

  CONSTANT(OPERATOR_SOURCE);
  CONSTANT(OPERATOR_OVER);
  CONSTANT(OPERATOR_IN);
  CONSTANT(OPERATOR_OUT);
  CONSTANT(OPERATOR_ATOP);

  CONSTANT(OPERATOR_DEST);
  CONSTANT(OPERATOR_DEST_OVER);
  CONSTANT(OPERATOR_DEST_IN);
  CONSTANT(OPERATOR_DEST_OUT);
  CONSTANT(OPERATOR_DEST_ATOP);

  CONSTANT(OPERATOR_XOR);
  CONSTANT(OPERATOR_ADD);
  CONSTANT(OPERATOR_SATURATE);

  CONSTANT(PATH_MOVE_TO);
  CONSTANT(PATH_LINE_TO);
  CONSTANT(PATH_CURVE_TO);
  CONSTANT(PATH_CLOSE_PATH);

#ifdef CAIRO_HAS_PS_SURFACE
  CONSTANT(PS_LEVEL_2);
  CONSTANT(PS_LEVEL_3);
#endif

  CONSTANT(SUBPIXEL_ORDER_DEFAULT);
  CONSTANT(SUBPIXEL_ORDER_RGB);
  CONSTANT(SUBPIXEL_ORDER_BGR);
  CONSTANT(SUBPIXEL_ORDER_VRGB);
  CONSTANT(SUBPIXEL_ORDER_VBGR);
#undef CONSTANT
}
