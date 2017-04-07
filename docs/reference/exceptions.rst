.. _exceptions:

**********
Exceptions
**********

.. currentmodule:: cairo

When a cairo function or method call fails an exception is raised. I/O errors
raise IOError, memory errors raise MemoryError, and all other errors raise
cairo.Error.

cairo.Error()
=============

.. exception:: cairo.Error

   This exception is raised when a cairo object returns an error status.
