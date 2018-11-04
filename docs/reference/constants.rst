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

.. function:: get_include()

    :returns: a path to the directory containing the C header files
    :rtype: str

    Gives the include path which should be passed to the compiler.

    .. versionadded:: 1.16.0


Module Constants
================

.. data:: version

   the pycairo version, as a string

.. data:: version_info

   the pycairo version, as a tuple

.. data:: CAIRO_VERSION

    The version of cairo available at compile-time in the same format as
    returned by :func:`cairo_version`

    .. versionadded:: 1.18.0

.. data:: CAIRO_VERSION_STRING

    A human-readable string literal containing the version of cairo available
    at compile-time, in the form of "X.Y.Z".

    .. versionadded:: 1.18.0

.. data:: CAIRO_VERSION_MAJOR

    The major component of the version of cairo available at compile-time.

    .. versionadded:: 1.18.0

.. data:: CAIRO_VERSION_MINOR

    The minor component of the version of cairo available at compile-time.

    .. versionadded:: 1.18.0

.. data:: CAIRO_VERSION_MICRO

    The micro component of the version of cairo available at compile-time.

    .. versionadded:: 1.18.0


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

.. data:: HAS_SCRIPT_SURFACE

    .. versionadded:: 1.12.0

.. data:: HAS_TEE_SURFACE

    .. versionadded:: 1.15.3


.. _constants_TAG:

cairo.TAG
---------

.. data:: TAG_DEST
    :annotation: = "cairo.dest"

    Create a destination for a hyperlink. Destination tag attributes are
    detailed at Destinations.

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

.. data:: TAG_LINK
    :annotation: = "Link"

    Create hyperlink. Link tag attributes are detailed at Links.

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+


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

.. data:: MIME_TYPE_CCITT_FAX
    :annotation: = "image/g3fax"

    Group 3 or Group 4 CCITT facsimile encoding (International
    Telecommunication Union, Recommendations T.4 and T.6.)

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

.. data:: MIME_TYPE_CCITT_FAX_PARAMS
    :annotation: = "application/x-cairo.ccitt.params"

    Decode parameters for Group 3 or Group 4 CCITT facsimile encoding. See
    `CCITT Fax Images
    <https://cairographics.org/manual/cairo-PDF-Surfaces.html#ccitt>`__.

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

.. data:: MIME_TYPE_EPS
    :annotation: = "application/postscript"

    Encapsulated PostScript file. Encapsulated PostScript File Format
    Specification

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

.. data:: MIME_TYPE_EPS_PARAMS
    :annotation: = "application/x-cairo.eps.params"

    Embedding parameters Encapsulated PostScript data. See Embedding EPS files.

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

.. data:: MIME_TYPE_JBIG2
    :annotation: = "application/x-cairo.jbig2"

    Joint Bi-level Image Experts Group image coding standard (ISO/IEC 11544).

    .. versionadded:: 1.18.0

.. data:: MIME_TYPE_JBIG2_GLOBAL
    :annotation: = "application/x-cairo.jbig2-global"

    Joint Bi-level Image Experts Group image coding standard (ISO/IEC 11544)
    global segment.

    .. versionadded:: 1.18.0

.. data:: MIME_TYPE_JBIG2_GLOBAL_ID
    :annotation: = "application/x-cairo.jbig2-global-id"

    An unique identifier shared by a JBIG2 global segment and all JBIG2 images
    that depend on the global segment.

    .. versionadded:: 1.18.0


Other Constants
---------------

.. data:: PDF_OUTLINE_ROOT
    :annotation: = 0

    The root outline item in :meth:`PDFSurface.add_outline`

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+


Other Classes and Functions
===========================

.. class:: text()

    This type only exists for documentation purposes. It represents
    :obj:`python:str`/:obj:`python:unicode` under Python 2 and
    :obj:`python3:str` under Python 3.


.. class:: pathlike()

    This type only exists for documentation purposes.

    On Unix it is equal to what Python allows as a filesystem path. On Windows
    with cairo <=1.15.8 only ANSI paths are supported. With cairo >=1.15.10
    all paths are supported as long as they don't contain surrogates.

    Many functions taking a path also allow passing in an already open Python
    file object. This can be used to support all Python filesystem paths
    independent of the underlying platform or cairo version.

    .. versionadded:: 1.15.1
        Older versions only supported a subset of :obj:`str` paths
