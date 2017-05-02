.. highlightlang:: c


***************
C API Reference
***************

.. currentmodule:: cairo

This manual documents the API used by C and C++ programmers who want to write
extension modules that use pycairo.


.. _api-includes:

To access the Pycairo C API under Python 2
==========================================

Edit the client module file to add the following lines::

  /* All function, type and macro definitions needed to use the Pycairo/C API
   * are included in your code by the following line
   */
  #include "Pycairo.h"

  /* define a variable for the C API */
  static Pycairo_CAPI_t *Pycairo_CAPI;

  /* import pycairo - add to the init<module> function */
  Pycairo_IMPORT;


To access the Pycairo C API under Python 3
==========================================

Example showing how to import the pycairo API::

  #include "py3cairo.h"

  PyMODINIT_FUNC
  PyInit_client(void)
  {
    PyObject *m;

    m = PyModule_Create(&clientmodule);
    if (m == NULL)
        return NULL;
    if (import_cairo() < 0)
        return NULL;
    /* additional initialization can happen here */
    return m;
  }


Pycairo Objects
===============

.. c:type:: PycairoContext
.. c:type:: PycairoFontFace
.. c:type:: PycairoToyFontFace
.. c:type:: PycairoFontOptions
.. c:type:: PycairoMatrix
.. c:type:: PycairoPath
.. c:type:: PycairoPattern
.. c:type:: PycairoRegion
.. c:type:: PycairoRectangleInt
.. c:type:: PycairoSolidPattern
.. c:type:: PycairoSurfacePattern
.. c:type:: PycairoGradient
.. c:type:: PycairoLinearGradient
.. c:type:: PycairoRadialGradient
.. c:type:: PycairoScaledFont
.. c:type:: PycairoSurface
.. c:type:: PycairoImageSurface
.. c:type:: PycairoPDFSurface
.. c:type:: PycairoPSSurface
.. c:type:: PycairoRecordingSurface
.. c:type:: PycairoSVGSurface
.. c:type:: PycairoWin32Surface
.. c:type:: PycairoXCBSurface
.. c:type:: PycairoXlibSurface


Pycairo Types
=============

.. c:type:: PyTypeObject *Context_Type
.. c:type:: PyTypeObject *FontFace_Type
.. c:type:: PyTypeObject *ToyFontFace_Type
.. c:type:: PyTypeObject *FontOptions_Type
.. c:type:: PyTypeObject *Matrix_Type
.. c:type:: PyTypeObject *Path_Type
.. c:type:: PyTypeObject *Pattern_Type
.. c:type:: PyTypeObject *Region_Type
.. c:type:: PyTypeObject *RectangleInt_Type
.. c:type:: PyTypeObject *SolidPattern_Type
.. c:type:: PyTypeObject *SurfacePattern_Type
.. c:type:: PyTypeObject *Gradient_Type
.. c:type:: PyTypeObject *LinearGradient_Type
.. c:type:: PyTypeObject *RadialGradient_Type
.. c:type:: PyTypeObject *ScaledFont_Type
.. c:type:: PyTypeObject *Surface_Type
.. c:type:: PyTypeObject *ImageSurface_Type
.. c:type:: PyTypeObject *PDFSurface_Type
.. c:type:: PyTypeObject *PSSurface_Type
.. c:type:: PyTypeObject *RecordingSurface_Type
.. c:type:: PyTypeObject *SVGSurface_Type
.. c:type:: PyTypeObject *Win32Surface_Type
.. c:type:: PyTypeObject *XCBSurface_Type
.. c:type:: PyTypeObject *XlibSurface_Type


Functions
=========

.. c:macro::  cairo_t * PycairoContext_GET(PycairoContext *obj)

   get the C cairo_t \* object out of the PycairoContext \*obj


.. c:function::  PyObject * PycairoContext_FromContext(cairo_t *ctx, PyTypeObject *type, PyObject *base)


.. c:function::  PyObject * PycairoFontFace_FromFontFace(cairo_font_face_t *font_face)


.. c:function::  PyObject * PycairoFontOptions_FromFontOptions(cairo_font_options_t *font_options)


.. c:function::  PyObject * PycairoMatrix_FromMatrix(const cairo_matrix_t *matrix)


.. c:function::  PyObject * PycairoPath_FromPath(cairo_path_t *path)


.. c:function::  PyObject * PycairoPattern_FromPattern(cairo_pattern_t *pattern, PyObject *base)


.. c:function::  PyObject * PycairoRegion_FromRegion(cairo_region_t *region)


.. c:function::  PyObject * PycairoRectangleInt_FromRectangleInt(const cairo_rectangle_int_t *rectangle_int)


.. c:function::  PyObject * PycairoScaledFont_FromScaledFont(cairo_scaled_font_t *scaled_font)


.. c:function::  PyObject * PycairoSurface_FromSurface(cairo_surface_t *surface, PyObject *base)


.. c:function::  int Pycairo_Check_Status(cairo_status_t status)


Cairo Types
===========

These are only listed here so they can be referenced in the documentation.

.. c:type:: cairo_t
    cairo_status_t
    cairo_surface_t
    cairo_scaled_font_t
    cairo_rectangle_int_t
    cairo_region_t
    cairo_pattern_t
    cairo_matrix_t
    cairo_font_options_t
    cairo_path_t
    cairo_font_face_t
