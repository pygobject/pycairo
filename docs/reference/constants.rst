.. _constants:

******************************
Module Functions and Constants
******************************

.. currentmodule:: cairo


Module Functions
================

.. autofunction:: cairo_version

.. autofunction:: cairo_version_string

.. autofunction:: get_include


Module Constants
================

.. autodata:: version

.. autodata:: version_info

.. autodata:: CAIRO_VERSION

.. autodata:: CAIRO_VERSION_STRING

.. autodata:: CAIRO_VERSION_MAJOR

.. autodata:: CAIRO_VERSION_MINOR

.. autodata:: CAIRO_VERSION_MICRO

.. _constants_HAS:

cairo.HAS
---------

1 if the feature is present in the underlying C cairo library, 0 otherwise.

.. autodata:: HAS_ATSUI_FONT
.. autodata:: HAS_FT_FONT
.. autodata:: HAS_GLITZ_SURFACE
.. autodata:: HAS_IMAGE_SURFACE
.. autodata:: HAS_PDF_SURFACE
.. autodata:: HAS_PNG_FUNCTIONS
.. autodata:: HAS_PS_SURFACE
.. autodata:: HAS_RECORDING_SURFACE
.. autodata:: HAS_SVG_SURFACE
.. autodata:: HAS_USER_FONT
.. autodata:: HAS_QUARTZ_SURFACE
.. autodata:: HAS_WIN32_FONT
.. autodata:: HAS_WIN32_SURFACE
.. autodata:: HAS_XCB_SURFACE
.. autodata:: HAS_XLIB_SURFACE
.. autodata:: HAS_MIME_SURFACE
.. autodata:: HAS_SCRIPT_SURFACE
.. autodata:: HAS_TEE_SURFACE

.. _constants_TAG:

cairo.TAG
---------

.. autodata:: TAG_DEST

.. autodata:: TAG_LINK

.. _constants_MIME_TYPE:

cairo.MIME_TYPE
---------------

.. autodata:: MIME_TYPE_JP2

.. autodata:: MIME_TYPE_JPEG

.. autodata:: MIME_TYPE_PNG

.. autodata:: MIME_TYPE_URI

.. autodata:: MIME_TYPE_UNIQUE_ID

.. autodata:: MIME_TYPE_CCITT_FAX

.. autodata:: MIME_TYPE_CCITT_FAX_PARAMS

.. autodata:: MIME_TYPE_EPS

.. autodata:: MIME_TYPE_EPS_PARAMS

.. autodata:: MIME_TYPE_JBIG2

.. autodata:: MIME_TYPE_JBIG2_GLOBAL

.. autodata:: MIME_TYPE_JBIG2_GLOBAL_ID

Other Constants
---------------

.. autodata:: PDF_OUTLINE_ROOT

Other Classes and Functions
===========================

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

.. class:: _PathLike

.. class:: _FileLike
