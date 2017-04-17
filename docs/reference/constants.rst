<.. _constants:

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


.. _constants_ANTIALIAS:

cairo.ANTIALIAS
---------------
ANTIALIAS specifies the type of antialiasing to do when rendering text or
shapes.

.. data:: ANTIALIAS_DEFAULT

   Use the default antialiasing for the subsystem and target device

.. data:: ANTIALIAS_NONE

   Use a bilevel alpha mask

.. data:: ANTIALIAS_GRAY

   Perform single-color antialiasing (using shades of gray for black text on a
   white background, for example).

.. data:: ANTIALIAS_SUBPIXEL

  Perform antialiasing by taking advantage of the order of subpixel elements
  on devices such as LCD panels.

.. data:: ANTIALIAS_FAST

    Hint that the backend should perform some antialiasing but prefer speed
    over quality.

    .. versionadded:: 1.12.0

.. data:: ANTIALIAS_GOOD

    The backend should balance quality against performance.

    .. versionadded:: 1.12.0

.. data:: ANTIALIAS_BEST

    Hint that the backend should render at the highest quality, sacrificing
    speed if necessary.

    .. versionadded:: 1.12.0


.. _constants_CONTENT:

cairo.CONTENT
-------------
These constants are used to describe the content that a :class:`Surface` will
contain, whether color information, alpha information (translucence
vs. opacity), or both.

.. data:: CONTENT_COLOR

   The surface will hold color content only.

.. data:: CONTENT_ALPHA

   The surface will hold alpha content only.

.. data:: CONTENT_COLOR_ALPHA

   The surface will hold color and alpha content.


.. _constants_EXTEND:

cairo.EXTEND
------------
These constants are used to describe how :class:`Pattern` color/alpha will be
determined for areas "outside" the pattern's natural area, (for example,
outside the surface bounds or outside the gradient geometry).

The default extend mode is *EXTEND_NONE* for :class:`SurfacePattern` and
*EXTEND_PAD* for :class:`Gradient` patterns.

.. data:: EXTEND_NONE

   pixels outside of the source pattern are fully transparent

.. data:: EXTEND_REPEAT

   the pattern is tiled by repeating

.. data:: EXTEND_REFLECT

   the pattern is tiled by reflecting at the edges (Implemented for surface
   patterns since 1.6)

.. data:: EXTEND_PAD

   pixels outside of the pattern copy the closest pixel from the source (Since
   1.2; but only implemented for surface patterns since 1.6)

New entries may be added in future versions.


.. _constants_FILL_RULE:

cairo.FILL_RULE
---------------
These constants are used to select how paths are filled. For both fill
rules, whether or not a point is included in the fill is determined by taking
a ray from that point to infinity and looking at intersections with the
path. The ray can be in any direction, as long as it doesn't pass through the
end point of a segment or have a tricky intersection such as intersecting
tangent to the path. (Note that filling is not actually implemented in this
way. This is just a description of the rule that is applied.)

The default fill rule is *FILL_RULE_WINDING*.

.. data:: FILL_RULE_WINDING

   If the path crosses the ray from left-to-right, counts +1. If the path
   crosses the ray from right to left, counts -1. (Left and right are
   determined from the perspective of looking along the ray from the starting
   point.) If the total count is non-zero, the point will be filled.

.. data::  FILL_RULE_EVEN_ODD

   Counts the total number of intersections, without regard to the orientation
   of the contour. If the total number of intersections is odd, the point will
   be filled.

New entries may be added in future versions.


.. _constants_FILTER:

cairo.FILTER
------------
These constants are used to indicate what filtering should be applied when
reading pixel values from patterns. See :meth:`Pattern.set_filter` for
indicating the desired filter to be used with a particular pattern.

.. data:: FILTER_FAST

   A high-performance filter, with quality similar *FILTER_NEAREST*

.. data:: FILTER_GOOD

   A reasonable-performance filter, with quality similar to *FILTER_BILINEAR*

.. data:: FILTER_BEST

   The highest-quality available, performance may not be suitable for
   interactive use.

.. data:: FILTER_NEAREST

   Nearest-neighbor filtering

.. data:: FILTER_BILINEAR

   Linear interpolation in two dimensions

.. data:: FILTER_GAUSSIAN

   This filter value is currently unimplemented, and should not be used in
   current code.


.. _constants_FONT_SLANT:

cairo.FONT_SLANT
----------------
These constants specify variants of a :class:`FontFace` based on their slant.

.. data:: FONT_SLANT_NORMAL

   Upright font style

.. data:: FONT_SLANT_ITALIC

   Italic font style

.. data:: FONT_SLANT_OBLIQUE

   Oblique font style


.. _constants_FONT_WEIGHT:

cairo.FONT_WEIGHT
-----------------
These constants specify variants of a :class:`FontFace` based on their weight.

.. data:: FONT_WEIGHT_NORMAL

   Normal font weight

.. data:: FONT_WEIGHT_BOLD

   Bold font weight


.. _constants_FORMAT:

cairo.FORMAT
------------
These constants are used to identify the memory format of
:class:`ImageSurface` data.

New entries may be added in future versions.

.. data:: FORMAT_INVALID

    no such format exists or is supported.

    .. versionadded:: 1.12.0

.. data:: FORMAT_ARGB32

   each pixel is a 32-bit quantity, with alpha in the upper 8 bits, then red,
   then green, then blue. The 32-bit quantities are stored
   native-endian. Pre-multiplied alpha is used. (That is, 50% transparent red
   is 0x80800000, not 0x80ff0000.)

.. data:: FORMAT_RGB24

   each pixel is a 32-bit quantity, with the upper 8 bits unused. Red, Green,
   and Blue are stored in the remaining 24 bits in that order.

.. data:: FORMAT_A8

   each pixel is a 8-bit quantity holding an alpha value.

.. data:: FORMAT_A1

   each pixel is a 1-bit quantity holding an alpha value. Pixels are packed
   together into 32-bit quantities. The ordering of the bits matches the
   endianess of the platform. On a big-endian machine, the first pixel is in
   the uppermost bit, on a little-endian machine the first pixel is in the
   least-significant bit.

.. data:: FORMAT_RGB16_565

   each pixel is a 16-bit quantity with red in the upper 5 bits, then green in
   the middle 6 bits, and blue in the lower 5 bits.

.. data:: FORMAT_RGB30

    like :data:`FORMAT_RGB24` but with 10bpc.

    .. versionadded:: 1.12.0


.. _constants_HINT_METRICS:

cairo.HINT_METRICS
------------------
These constants specify whether to hint font metrics; hinting font metrics
means quantizing them so that they are integer values in device space. Doing
this improves the consistency of letter and line spacing, however it also
means that text will be laid out differently at different zoom factors.

.. data:: HINT_METRICS_DEFAULT

   Hint metrics in the default manner for the font backend and target device

.. data:: HINT_METRICS_OFF

   Do not hint font metrics

.. data:: HINT_METRICS_ON

   Hint font metrics


.. _constants_HINT_STYLE:

cairo.HINT_STYLE
----------------
These constants specify the type of hinting to do on font outlines. Hinting is
the process of fitting outlines to the pixel grid in order to improve the
appearance of the result. Since hinting outlines involves distorting them, it
also reduces the faithfulness to the original outline shapes. Not all of the
outline hinting styles are supported by all font backends.

.. data:: HINT_STYLE_DEFAULT

   Use the default hint style for font backend and target device

.. data:: HINT_STYLE_NONE

   Do not hint outlines

.. data:: HINT_STYLE_SLIGHT

   Hint outlines slightly to improve contrast while retaining good fidelity to
   the original shapes.

.. data:: HINT_STYLE_MEDIUM

   Hint outlines with medium strength giving a compromise between fidelity to
   the original shapes and contrast

.. data:: HINT_STYLE_FULL

   Hint outlines to maximize contrast

New entries may be added in future versions.


.. _constants_LINE_CAP:

cairo.LINE_CAP
--------------
These constants specify how to render the endpoints of the path when stroking.

The default line cap style is *LINE_CAP_BUTT*

.. data:: LINE_CAP_BUTT

   start(stop) the line exactly at the start(end) point

.. data:: LINE_CAP_ROUND

   use a round ending, the center of the circle is the end point

.. data:: LINE_CAP_SQUARE

   use squared ending, the center of the square is the end point


.. _constants_LINE_JOIN:

cairo.LINE_JOIN
---------------
These constants specify how to render the junction of two lines when stroking.

The default line join style is *LINE_JOIN_MITER*

.. data:: LINE_JOIN_MITER

   use a sharp (angled) corner, see :meth:`Context.set_miter_limit`

.. data:: LINE_JOIN_ROUND

   use a rounded join, the center of the circle is the joint point

.. data:: LINE_JOIN_BEVEL

   use a cut-off join, the join is cut off at half the line width from the
   joint point


.. _constants_OPERATOR:

cairo.OPERATOR
--------------
These constants are used to set the compositing operator for all cairo drawing
operations.

The default operator is *OPERATOR_OVER*.

The operators marked as *unbounded* modify their destination even outside of
the mask layer (that is, their effect is not bound by the mask layer).
However, their effect can still be limited by way of clipping.

To keep things simple, the operator descriptions here document the behavior
for when both source and destination are either fully transparent or fully
opaque.  The actual implementation works for translucent layers too.

For a more detailed explanation of the effects of each operator, including the
mathematical definitions, see https://cairographics.org/operators.

.. data:: OPERATOR_CLEAR

   clear destination layer (bounded)

.. data:: OPERATOR_SOURCE

  replace destination layer (bounded)

.. data:: OPERATOR_OVER

   draw source layer on top of destination layer (bounded)

.. data:: OPERATOR_IN

   draw source where there was destination content (unbounded)

.. data:: OPERATOR_OUT

   draw source where there was no destination content (unbounded)

.. data:: OPERATOR_ATOP

   draw source on top of destination content and only there

.. data:: OPERATOR_DEST

   ignore the source

.. data:: OPERATOR_DEST_OVER

   draw destination on top of source

.. data:: OPERATOR_DEST_IN

   leave destination only where there was source content (unbounded)

.. data:: OPERATOR_DEST_OUT

   leave destination only where there was no source content

.. data:: OPERATOR_DEST_ATOP

   leave destination on top of source content and only there (unbounded)

.. data:: OPERATOR_XOR

   source and destination are shown where there is only one of them

.. data:: OPERATOR_ADD

   source and destination layers are accumulated

.. data:: OPERATOR_SATURATE

   like over, but assuming source and dest are disjoint geometries

.. data:: OPERATOR_MULTIPLY

    source and destination layers are multiplied. This causes the result to be
    at least as dark as the darker inputs.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_SCREEN

    source and destination are complemented and multiplied. This causes the
    result to be at least as light as the lighter inputs.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_OVERLAY

    multiplies or screens, depending on the lightness of the destination
    color.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_DARKEN

    replaces the destination with the source if it is darker, otherwise keeps
    the source.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_LIGHTEN

    replaces the destination with the source if it is lighter, otherwise keeps
    the source.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_COLOR_DODGE

    brightens the destination color to reflect the source color.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_COLOR_BURN

    darkens the destination color to reflect the source color.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_HARD_LIGHT

    Multiplies or screens, dependent on source color.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_SOFT_LIGHT

    Darkens or lightens, dependent on source color.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_DIFFERENCE

    Takes the difference of the source and destination color.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_EXCLUSION

    Produces an effect similar to difference, but with lower contrast.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_HSL_HUE

    Creates a color with the hue of the source and the saturation and
    luminosity of the target.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_HSL_SATURATION

    Creates a color with the saturation of the source and the hue and
    luminosity of the target. Painting with this mode onto a gray area
    produces no change.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_HSL_COLOR

    Creates a color with the hue and saturation of the source and the
    luminosity of the target. This preserves the gray levels of the target and
    is useful for coloring monochrome images or tinting color images.

    .. versionadded:: 1.12.0

.. data:: OPERATOR_HSL_LUMINOSITY

    Creates a color with the luminosity of the source and the hue and
    saturation of the target. This produces an inverse effect to
    :data:`OPERATOR_HSL_COLOR`

    .. versionadded:: 1.12.0


.. _constants_PATH:

cairo.PATH
----------
These constants are used to describe the type of one portion of a path when
represented as a :class:`Path`.

.. See #cairo_path_data_t for details.

.. data:: PATH_MOVE_TO

   A move-to operation

.. data:: PATH_LINE_TO

   A line-to operation

.. data:: PATH_CURVE_TO

   A curve-to operation

.. data:: PATH_CLOSE_PATH

   A close-path operation


.. _constants_PS_LEVEL:

cairo.PS_LEVEL
--------------
These constants are used to describe the language level of the PostScript
Language Reference that a generated PostScript file will conform to.  Note:
the constants are only defined when cairo has been compiled with PS support
enabled.

.. data:: PS_LEVEL_2

   The language level 2 of the PostScript specification.

.. data:: PS_LEVEL_3

   The language level 3 of the PostScript specification.


.. _constants_PDF_VERSION:

cairo.PDF_VERSION
-----------------

These constants are used to describe the version number of the PDF
specification that a generated PDF file will conform to.

.. data:: PDF_VERSION_1_4

    The version 1.4 of the PDF specification.

    .. versionadded:: 1.12.0


.. data:: PDF_VERSION_1_5

    The version 1.5 of the PDF specification.

    .. versionadded:: 1.12.0


.. _constants_SVG_VERSION:

cairo.SVG_VERSION
-----------------

These constants are used to describe the version number of the SVG
specification that a generated SVG file will conform to.

.. data:: SVG_VERSION_1_1

    The version 1.1 of the SVG specification.

    .. versionadded:: 1.12.0


.. data:: SVG_VERSION_1_2

    The version 1.2 of the SVG specification.

    .. versionadded:: 1.12.0


.. _constants_SUBPIXEL_ORDER:

cairo.SUBPIXEL_ORDER
--------------------
The subpixel order specifies the order of color elements within each pixel on
the display device when rendering with an antialiasing mode of
:data:`ANTIALIAS_SUBPIXEL`.

.. data:: SUBPIXEL_ORDER_DEFAULT

   Use the default subpixel order for for the target device

.. data:: SUBPIXEL_ORDER_RGB

   Subpixel elements are arranged horizontally with red at the left

.. data:: SUBPIXEL_ORDER_BGR

   Subpixel elements are arranged horizontally with blue at the left

.. data:: SUBPIXEL_ORDER_VRGB

   Subpixel elements are arranged vertically with red at the top

.. data:: SUBPIXEL_ORDER_VBGR

   Subpixel elements are arranged vertically with blue at the top


.. _constants_REGION_OVERLAP:

cairo.REGION_OVERLAP
--------------------

.. data:: REGION_OVERLAP_IN

    The contents are entirely inside the region.

.. data:: REGION_OVERLAP_OUT

    The contents are entirely outside the region.

.. data:: REGION_OVERLAP_PART

    The contents are partially inside and partially outside the region.


.. _constants_STATUS:

cairo.STATUS
------------

.. data:: STATUS_SUCCESS
    STATUS_NO_MEMORY
    STATUS_INVALID_RESTORE
    STATUS_INVALID_POP_GROUP
    STATUS_NO_CURRENT_POINT
    STATUS_INVALID_MATRIX
    STATUS_INVALID_STATUS
    STATUS_NULL_POINTER
    STATUS_INVALID_STRING
    STATUS_INVALID_PATH_DATA
    STATUS_READ_ERROR
    STATUS_WRITE_ERROR
    STATUS_SURFACE_FINISHED
    STATUS_SURFACE_TYPE_MISMATCH
    STATUS_PATTERN_TYPE_MISMATCH
    STATUS_INVALID_CONTENT
    STATUS_INVALID_FORMAT
    STATUS_INVALID_VISUAL
    STATUS_FILE_NOT_FOUND
    STATUS_INVALID_DASH
    STATUS_INVALID_DSC_COMMENT
    STATUS_INVALID_INDEX
    STATUS_CLIP_NOT_REPRESENTABLE
    STATUS_TEMP_FILE_ERROR
    STATUS_INVALID_STRIDE
    STATUS_FONT_TYPE_MISMATCH
    STATUS_USER_FONT_IMMUTABLE
    STATUS_USER_FONT_ERROR
    STATUS_NEGATIVE_COUNT
    STATUS_INVALID_CLUSTERS
    STATUS_INVALID_SLANT
    STATUS_INVALID_WEIGHT
    STATUS_INVALID_SIZE
    STATUS_USER_FONT_NOT_IMPLEMENTED
    STATUS_DEVICE_TYPE_MISMATCH
    STATUS_DEVICE_ERROR
    STATUS_INVALID_MESH_CONSTRUCTION
    STATUS_DEVICE_FINISHED
    STATUS_LAST_STATUS


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
