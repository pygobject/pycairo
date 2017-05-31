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

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "config.h"
#include "private.h"


/* for XCB api */
#if defined(CAIRO_HAS_XCB_SURFACE) && defined(HAVE_XPYB)
xpyb_CAPI_t *xpyb_CAPI;
PyObject *xpybVISUALTYPE_type;
#endif

#if PY_MAJOR_VERSION < 3

/* A module specific exception */
PyObject *_CairoError = NULL;

#else

/* Module initialization */
struct cairo_state {
  PyObject *ErrorObject;
};

#define GETSTATE(m) ((struct cairo_state*)PyModule_GetState(m))

static struct PyModuleDef cairomoduledef;
#endif

/* Returns a borrowed reference of the error type. */
PyObject *
_Pycairo_Get_Error(void) {
#if PY_MAJOR_VERSION < 3
  assert(_CairoError != NULL);
  return _CairoError;
#else
  PyObject *cairo_module = PyState_FindModule(&cairomoduledef);
  assert(cairo_module != NULL);
  return GETSTATE(cairo_module)->ErrorObject;
#endif
}


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
  default:
    Pycairo_Set_Error(status);
  }
  return 1;
}


/* C API.  Clients get at this via Pycairo_IMPORT or import_cairo(), defined in pycairo.h.
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

  &PycairoRectangleInt_Type,
  PycairoRectangleInt_FromRectangleInt,

  &PycairoRegion_Type,
  PycairoRegion_FromRegion,
#ifdef CAIRO_HAS_RECORDING_SURFACE
  &PycairoRecordingSurface_Type,
#else
  0,
#endif
};

static PyObject *
pycairo_cairo_version (PyObject *self) {
  return PYCAIRO_PyLong_FromLong (cairo_version());
}

static PyObject *
pycairo_cairo_version_string (PyObject *self) {
  return PYCAIRO_PyUnicode_FromString (cairo_version_string());
}

static PyMethodDef cairo_functions[] = {
  {"cairo_version",    (PyCFunction)pycairo_cairo_version, METH_NOARGS},
  {"cairo_version_string", (PyCFunction)pycairo_cairo_version_string,
   METH_NOARGS},
  {NULL, NULL, 0, NULL},
};

#if PY_MAJOR_VERSION >= 3

static int
cairo_traverse(PyObject *m, visitproc visit, void *arg)
{
  Py_VISIT(GETSTATE(m)->ErrorObject);
  return 0;
}

static int
cairo_clear(PyObject *m)
{
  Py_CLEAR(GETSTATE(m)->ErrorObject);
  return 0;
}

static struct PyModuleDef cairomoduledef = {
  PyModuleDef_HEAD_INIT,
  "cairo",
  NULL,
  sizeof(struct cairo_state),
  cairo_functions,
  0,  /* m_reload */
  cairo_traverse,
  cairo_clear,
  0,  /* m_free - not needed, since all is done in m_clear */
};
#endif

PYCAIRO_MOD_INIT(_cairo)
{
  PyObject *m, *capi, *error;

  if (PyType_Ready(&PycairoContext_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoFontFace_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoToyFontFace_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoFontOptions_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoMatrix_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoPath_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  PycairoPathiter_Type.tp_iter=&PyObject_SelfIter;
  if (PyType_Ready(&PycairoPathiter_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

  if (PyType_Ready(&PycairoPattern_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoSolidPattern_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoSurfacePattern_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoGradient_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoLinearGradient_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoRadialGradient_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

  if (PyType_Ready(&PycairoRectangleInt_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

  if (PyType_Ready(&PycairoRegion_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

  if (PyType_Ready(&PycairoScaledFont_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

  if (PyType_Ready(&PycairoSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#ifdef CAIRO_HAS_IMAGE_SURFACE
  if (PyType_Ready(&PycairoImageSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
  if (PyType_Ready(&PycairoPDFSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif
#ifdef CAIRO_HAS_PS_SURFACE
  if (PyType_Ready(&PycairoPSSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif
#ifdef CAIRO_HAS_RECORDING_SURFACE
  if (PyType_Ready(&PycairoRecordingSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
  if (PyType_Ready(&PycairoSVGSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
  if (PyType_Ready(&PycairoWin32Surface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
  if (PyType_Ready(&PycairoWin32PrintingSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif
#ifdef CAIRO_HAS_XCB_SURFACE
  if (PyType_Ready(&PycairoXCBSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif
#ifdef CAIRO_HAS_XLIB_SURFACE
  if (PyType_Ready(&PycairoXlibSurface_Type) < 0)
    return PYCAIRO_MOD_ERROR_VAL;
#endif

#if PY_MAJOR_VERSION < 3
  m = Py_InitModule("cairo._cairo", cairo_functions);
#else
  m = PyModule_Create(&cairomoduledef);
#endif
  if (m == NULL)
    return PYCAIRO_MOD_ERROR_VAL;

  if(init_buffer_proxy() < 0)
    return PYCAIRO_MOD_ERROR_VAL;

  if(init_enums(m) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

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

  Py_INCREF(&PycairoRectangleInt_Type);
  PyModule_AddObject(m, "RectangleInt",  (PyObject *)&PycairoRectangleInt_Type);

  Py_INCREF(&PycairoRegion_Type);
  PyModule_AddObject(m, "Region",  (PyObject *)&PycairoRegion_Type);

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

#ifdef CAIRO_HAS_RECORDING_SURFACE
  Py_INCREF(&PycairoRecordingSurface_Type);
  PyModule_AddObject(m, "RecordingSurface",
		     (PyObject *)&PycairoRecordingSurface_Type);
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

  /* Add 'cairo.Error' to the module */
#if PY_MAJOR_VERSION >= 3
  GETSTATE(m)->ErrorObject = error_get_type();
  if (GETSTATE(m)->ErrorObject == NULL) {
    Py_DECREF(m);
    return PYCAIRO_MOD_ERROR_VAL;
  }
  error = GETSTATE(m)->ErrorObject;
#else
  if (_CairoError == NULL) {
    _CairoError = error_get_type();
    if (_CairoError == NULL)
      return PYCAIRO_MOD_ERROR_VAL;
  }
  error = _CairoError;
#endif

  Py_INCREF(error);
  if (PyModule_AddObject(m, "Error", error) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

  /* Alias for cairocffi */
  Py_INCREF(error);
  if (PyModule_AddObject(m, "CairoError", error) < 0)
    return PYCAIRO_MOD_ERROR_VAL;

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
#if CAIRO_HAS_RECORDING_SURFACE
  PyModule_AddIntConstant(m, "HAS_RECORDING_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_RECORDING_SURFACE", 0);
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
#if CAIRO_HAS_MIME_SURFACE
  PyModule_AddIntConstant(m, "HAS_MIME_SURFACE", 1);
#else
  PyModule_AddIntConstant(m, "HAS_MIME_SURFACE", 0);
#endif

#define STRCONSTANT(x) PyModule_AddStringConstant(m, #x, CAIRO_##x)

  STRCONSTANT(MIME_TYPE_JP2);
  STRCONSTANT(MIME_TYPE_JPEG);
  STRCONSTANT(MIME_TYPE_PNG);
  STRCONSTANT(MIME_TYPE_URI);
  STRCONSTANT(MIME_TYPE_UNIQUE_ID);

#undef STRCONSTANT

#if PY_MAJOR_VERSION >= 3
  /* Create a Capsule containing the CAPI pointer */
  capi = PyCapsule_New((void *)(&CAPI), "cairo.CAPI", 0);
#else
  capi = PyCObject_FromVoidPtr(&CAPI, NULL);
#endif

  if (capi != NULL) {
    PyModule_AddObject(m, "CAPI", capi);
  }

  return PYCAIRO_MOD_SUCCESS_VAL(m);
}
