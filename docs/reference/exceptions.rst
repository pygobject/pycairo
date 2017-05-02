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

.. exception:: Error

    This exception is raised when a cairo object returns an error status.

    .. attribute:: status

        :type: :class:`cairo.Status`


.. data:: CairoError

    An alias for :exc:`Error`

    .. versionadded:: 1.12.0
