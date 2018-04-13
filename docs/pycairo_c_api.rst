.. highlightlang:: c


***************
C API Reference
***************

.. currentmodule:: cairo

This manual documents the API used by C and C++ programmers who want to write
extension modules that use pycairo.


Pycairo Compiler Flags
======================

To compile a Python extension using Pycairo you need to know where Pycairo and
cairo are located and what flags to pass to the compiler and linker.

1. Variant:

    Only available since version 1.16.0.

    While Pycairo installs a pkg-config file, in case of virtualenvs,
    installation to the user directory or when using wheels/eggs, pkg-config
    will not be able to locate the .pc file. The :func:`get_include` function
    should work in all cases, as long as Pycairo is in your Python search path.

    Compiler Flags:
        * ``python -c "import cairo; print(cairo.get_include())"``
        * ``pkg-config --cflags cairo``

    Linker Flags:
        * ``pkg-config --libs cairo``

2. Variant:

    This works with older versions, but with the limitations mentioned above.
    Use it as a fallback if you want to support older versions or if your
    module does not require virtualenv/pip support.

    Compiler Flags:
        * ``pkg-config --cflags pycairo`` or ``pkg-config --cflags py3cairo``

    Linker Flags:
        * ``pkg-config --libs pycairo`` or ``pkg-config --libs py3cairo``


.. _api-includes:

To access the Pycairo C API under Python 2
==========================================

Edit the client module file to add the following lines::

  /* All function, type and macro definitions needed to use the Pycairo/C API
   * are included in your code by the following line
   */
  #include "pycairo.h"

  /* define a variable for the C API */
  Pycairo_CAPI_t *Pycairo_CAPI;

  /* import pycairo - add to the init<module> function */
  Pycairo_IMPORT;

In case you want to use the API from another compilation unit::

  #include <pycairo.h>

  extern Pycairo_CAPI_t *Pycairo_CAPI;

  ...

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

In case you want to use the API from another compilation unit::

  #define PYCAIRO_NO_IMPORT
  #include <py3cairo.h>

  ...

.. versionadded:: 1.17.0

    The ``PYCAIRO_NO_IMPORT`` macro is used since 1.17.0


Misc Functions
==============

.. c:function::  int Pycairo_Check_Status(cairo_status_t status)

    :param cairo_status_t status:
    :returns:
        -1 in case of an error, otherwise 0.
        Sets an exception in case of an error.

    Takes a status value and converts it to an exception if it represents an
    error status.


Cairo Context
=============

.. c:type:: PyObject PycairoContext

    .. c:member:: cairo_t* PycairoContext.ctx

        The wrapped :any:`cairo_t`

.. c:type:: PyTypeObject *PycairoContext_Type

.. c:macro::  cairo_t * PycairoContext_GET(PycairoContext *obj)

    :param PycairoContext obj:
    :returns: :any:`cairo_t` [transfer none]

    Get the :any:`cairo_t` object out of the :any:`PycairoContext`.

.. c:function:: PyObject * PycairoContext_FromContext(cairo_t *ctx, PyTypeObject *type, PyObject *base)

    :param cairo_t ctx:
        a cairo_t to 'wrap' into a Python object.
        It is unreferenced if the PycairoContext creation fails, or if
        the cairo_t has an error status. [transfer full]
    :param PyTypeObject type:
        a pointer to the type to instantiate.
        It can be &PycairoContext_Type, or a PycairoContext_Type subtype.
        (cairo.Context or a cairo.Context subclass)
        [transfer none]
    :param PyObject base:
        the base object used to create the context, or NULL.
        it is referenced to keep it alive while the cairo_t is being used
        [transfer none]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoContext from a :any:`cairo_t`


Cairo Font Face
===============

.. c:type:: PyObject PycairoFontFace

    .. c:member:: cairo_font_face_t* PycairoFontFace.font_face

    The wrapped :any:`cairo_font_face_t`

.. c:type:: PyTypeObject *PycairoFontFace_Type

.. c:function:: PyObject * PycairoFontFace_FromFontFace(cairo_font_face_t *font_face)

    :param cairo_font_face_t font_face:
        a cairo_font_face_t to 'wrap' into a Python object.
        it is unreferenced if the PycairoFontFace creation fails
        [transfer full]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoFontFace from a cairo_font_face_t

.. c:type:: PycairoFontFace PycairoToyFontFace

.. c:type:: PyTypeObject *PycairoToyFontFace_Type


Cairo Font Options
==================

.. c:type:: PyObject PycairoFontOptions

    .. c:member:: cairo_font_options_t* PycairoFontOptions.font_options

.. c:type:: PyTypeObject *PycairoFontOptions_Type

.. c:function:: PyObject * PycairoFontOptions_FromFontOptions(cairo_font_options_t *font_options)

    :param cairo_font_options_t font_options:
        a cairo_font_options_t to 'wrap' into a Python object.
        it is unreferenced if the PycairoFontOptions creation fails
        [transfer full]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoFontOptions from a cairo_font_options_t


Cairo Matrix
============

.. c:type:: PyObject PycairoMatrix

    .. c:member:: cairo_matrix_t PycairoMatrix.matrix

.. c:type:: PyTypeObject *PycairoMatrix_Type

.. c:function:: PyObject * PycairoMatrix_FromMatrix(const cairo_matrix_t *matrix)

    :param cairo_matrix_t matrix:
        a cairo_matrix_t to 'wrap' into a Python object.
        the cairo_matrix_t values are copied.
        [transfer none]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoMatrix from a cairo_matrix_t


Cairo Path
==========

.. c:type:: PyObject PycairoPath

    .. c:member:: cairo_path_t* PycairoPath.path

.. c:type:: PyTypeObject *PycairoPath_Type

.. c:function:: PyObject * PycairoPath_FromPath(cairo_path_t *path)

    :param cairo_path_t path:
        a cairo_path_t to 'wrap' into a Python object.
        path is unreferenced if the PycairoPath creation fails, or if path
        is in an error status.
        [transfer full]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoPath from a cairo_path_t


Cairo Pattern
=============

.. c:type:: PyObject PycairoPattern

    .. c:member:: cairo_pattern_t* PycairoPattern.pattern

.. c:type:: PyTypeObject *PycairoPattern_Type

.. c:type:: PycairoPattern PycairoSolidPattern

.. c:type:: PyTypeObject *PycairoSolidPattern_Type

.. c:type:: PycairoPattern PycairoSurfacePattern

.. c:type:: PyTypeObject *PycairoSurfacePattern_Type

.. c:type:: PycairoPattern PycairoGradient

.. c:type:: PyTypeObject *PycairoGradient_Type

.. c:type:: PycairoGradient PycairoLinearGradient

.. c:type:: PyTypeObject *PycairoLinearGradient_Type

.. c:type:: PycairoGradient PycairoRadialGradient

.. c:type:: PyTypeObject *PycairoRadialGradient_Type

.. c:function:: PyObject * PycairoPattern_FromPattern(cairo_pattern_t *pattern, PyObject *base)

    :param cairo_pattern_t pattern:
        a cairo_pattern_t to 'wrap' into a Python object.
        It is unreferenced if the PycairoPattern creation fails, or if the
        pattern has an error status.
        [transfer full]
    :param PyObject base:
        the base object used to create the pattern, or NULL.
        It is referenced to keep it alive while the cairo_pattern_t is being
        used.
        [transfer none]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoSolidPattern, PycairoSurfacePattern,
    PycairoLinearGradient, or PycairoRadialGradient from a cairo_pattern_t.


Cairo Region
============

.. c:type:: PyObject PycairoRegion

    .. c:member:: cairo_region_t* PycairoRegion.region

.. c:type:: PyTypeObject *PycairoRegion_Type

.. c:function:: PyObject * PycairoRegion_FromRegion(cairo_region_t *region)

    :param cairo_region_t region:
        a cairo_region_t to 'wrap' into a Python object.
        region is unreferenced if the PycairoRegion creation fails, or if
        region is in an error status.
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoRegion from a cairo_region_t


Cairo RectangleInt
==================

.. c:type:: PyObject PycairoRectangleInt

    .. c:member:: cairo_rectangle_int_t* PycairoRectangleInt.rectangle_int

.. c:type:: PyTypeObject *PycairoRectangleInt_Type

.. c:function:: PyObject * PycairoRectangleInt_FromRectangleInt(const cairo_rectangle_int_t *rectangle_int)

    :param cairo_rectangle_int_t rectangle_int:
        a cairo_rectangle_int_t to 'wrap' into a Python object.
        rectangle_int is unreferenced if the PycairoRectangleInt creation
        fails.
        [transfer none]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoRectangleInt from a cairo_rectangle_int_t


Scaled Font
===========

.. c:type:: PyObject PycairoScaledFont

    .. c:member:: cairo_scaled_font_t* PycairoScaledFont.scaled_font

.. c:type:: PyTypeObject *PycairoScaledFont_Type

.. c:function:: PyObject * PycairoScaledFont_FromScaledFont(cairo_scaled_font_t *scaled_font)

    :param cairo_scaled_font_t scaled_font:
        a cairo_scaled_font_t to 'wrap' into a Python object.
        it is unreferenced if the PycairoScaledFont creation fails
        [transfer full]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoScaledFont from a cairo_scaled_font_t


Cairo Surface
=============

.. c:type:: PyObject PycairoSurface

    .. c:member:: cairo_surface_t* PycairoSurface.surface

.. c:type:: PyTypeObject *PycairoSurface_Type

.. c:type:: PycairoSurface PycairoImageSurface
.. c:type:: PyTypeObject *PycairoImageSurface_Type

.. c:type:: PycairoSurface PycairoPDFSurface
.. c:type:: PyTypeObject *PycairoPDFSurface_Type

.. c:type:: PycairoSurface PycairoPSSurface
.. c:type:: PyTypeObject *PycairoPSSurface_Type

.. c:type:: PycairoSurface PycairoRecordingSurface
.. c:type:: PyTypeObject *PycairoRecordingSurface_Type

.. c:type:: PycairoSurface PycairoSVGSurface
.. c:type:: PyTypeObject *PycairoSVGSurface_Type

.. c:type:: PycairoSurface PycairoWin32Surface
.. c:type:: PyTypeObject *PycairoWin32Surface_Type

.. c:type:: PycairoSurface PycairoXCBSurface
.. c:type:: PyTypeObject *PycairoXCBSurface_Type

.. c:type:: PycairoSurface PycairoXlibSurface
.. c:type:: PyTypeObject *PycairoXlibSurface_Type

.. c:function::  PyObject * PycairoSurface_FromSurface(cairo_surface_t *surface, PyObject *base)

    :param cairo_surface_t surface:
        a cairo_surface_t to 'wrap' into a Python object.
        It is unreferenced if the PycairoSurface creation fails, or if the
        cairo_surface_t has an error status.
        [transfer full]
    :param PyObject base:
        the base object used to create the surface, or NULL.
        It is referenced to keep it alive while the cairo_surface_t is being
        used.
        [transfer none]
    :returns:
        New reference or NULL on failure and sets an exception
        [transfer full]

    Create a new PycairoImageSurface, PycairoPDFSurface, PycairoPSSurface,
    PycairoRecordingSurface, PycairoSVGSurface, PycairoWin32Surface,
    PycairoWin32PrintingSurface, PycairoXCBSurface, or PycairoXlibSurface from
    a cairo_surface_t.


Cairo Types
===========

These are only listed here so they can be referenced in the documentation.

See https://www.cairographics.org/manual/ for details.

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
