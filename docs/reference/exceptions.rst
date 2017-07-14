.. _exceptions:

**********
Exceptions
**********

.. currentmodule:: cairo

When a cairo function or method call fails an exception is raised. I/O errors
raise IOError, and all other errors raise cairo.Error.

cairo.Error()
=============

.. exception:: Error

    This exception is raised when a cairo object returns an error status.

    .. attribute:: status

        :type: :class:`cairo.Status`

.. exception:: CairoError

    An alias for :exc:`Error`

    .. versionadded:: 1.12.0

.. exception:: MemoryError

    :bases: :exc:`Error`, :exc:`python3:MemoryError`

    .. versionadded:: 1.15
        Prior to 1.15 :exc:`python3:MemoryError` was raised instead of this
        type.
