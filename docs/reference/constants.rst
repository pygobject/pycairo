.. _constants:

******************************
Module Functions and Constants
******************************

.. currentmodule:: cairo


Module Functions
================

.. function:: cairo_version()

   :returns: the encoded version
   :rtype: int

   Returns the version of the underlying C cairo library, encoded in a single
   integer.

.. function:: cairo_version_string()

   :returns: the encoded version
   :rtype: str

   Returns the version of the underlying C cairo library as a human-readable
   string of the form "X.Y.Z".


Module Constants
================

.. data:: version

   the pycairo version, as a string

.. data:: version_info

   the pycairo version, as a tuple


.. _constants_HAS:

cairo.HAS
---------

1 if the feature is present in the underlying C cairo library, 0 otherwise.

.. data:: HAS_ATSUI_FONT
          HAS_FT_FONT
          HAS_GLITZ_SURFACE
          HAS_IMAGE_SURFACE
          HAS_PDF_SURFACE
          HAS_PNG_FUNCTIONS
          HAS_PS_SURFACE
          HAS_RECORDING_SURFACE
          HAS_SVG_SURFACE
          HAS_USER_FONT
          HAS_QUARTZ_SURFACE
          HAS_WIN32_FONT
          HAS_WIN32_SURFACE
          HAS_XCB_SURFACE
          HAS_XLIB_SURFACE

.. data:: HAS_MIME_SURFACE

    .. versionadded:: 1.12.0


.. _constants_MIME_TYPE:

cairo.MIME_TYPE
---------------

.. data:: MIME_TYPE_JP2
    :annotation: = "image/jp2"

    The Joint Photographic Experts Group (JPEG) 2000 image coding standard
    (ISO/IEC 15444-1).

    .. versionadded:: 1.12.0

.. data:: MIME_TYPE_JPEG
    :annotation: = "image/jpeg"

    The Joint Photographic Experts Group (JPEG) image coding standard (ISO/IEC
    10918-1).

    .. versionadded:: 1.12.0

.. data:: MIME_TYPE_PNG
    :annotation: = "image/png"

    The Portable Network Graphics image file format (ISO/IEC 15948).

    .. versionadded:: 1.12.0

.. data:: MIME_TYPE_URI
    :annotation: = "text/x-uri"

    URI for an image file (unofficial MIME type).

    .. versionadded:: 1.12.0

.. data:: MIME_TYPE_UNIQUE_ID
    :annotation: = "application/x-cairo.uuid"

    Unique identifier for a surface (cairo specific MIME type). All surfaces
    with the same unique identifier will only be embedded once.

    .. versionadded:: 1.12.0


Other Classes and Functions
===========================

.. class:: text()

    This type only exists for documentation purposes. It represents
    :obj:`python:str`/:obj:`python:unicode` under Python 2 and
    :obj:`python3:str` under Python 3.
