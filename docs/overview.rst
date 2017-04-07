********
Overview
********

.. currentmodule:: cairo

Pycairo is a Python binding for the cairo graphics library.

The Pycairo bindings are designed to match the cairo C API as closely as
possible, and to deviate only in cases which are clearly better implemented in
a more 'Pythonic' way.

Features of the Pycairo bindings:

* Provides an object oriented interface to cairo.
* Pycairo_Check_Status() is called to check the status of cairo operations, and raise exceptions as appropriate.
* Provides a C API that can be used by other Python extensions.

The C cairo functions cairo_reference(), cairo_destroy(),
cairo_surface_reference(), cairo_surface_destroy() (and their equivalents for
surfaces and patterns) are not made public by the pycairo bindings. This is
because pycairo handles cairo object construction and destruction.

To use the pycairo library::

  import cairo

See :ref:`Reference <reference_index>` for further details.

For examples of pycairo code see the 'examples' directory that comes with the
pycairo distribution.
