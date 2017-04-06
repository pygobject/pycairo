.. _mattributes:

*******************************
Module Functions and Attributes
*******************************

Module Functions
================

.. function:: cairo.cairo_version()

   :returns: the encoded version
   :rtype: int

   Returns the version of the underlying C cairo library, encoded in a single
   integer.

.. function:: cairo.cairo_version_string()

   :returns: the encoded version
   :rtype: str

   Returns the version of the underlying C cairo library as a human-readable
   string of the form "X.Y.Z".


Module Attributes
=================

.. attribute:: cairo.version

   the pycairo version, as a string

.. attribute:: cairo.version_info

   the pycairo version, as a tuple

.. attribute:: cairo.HAS_ATSUI_FONT
               cairo.HAS_FT_FONT
               cairo.HAS_GLITZ_SURFACE
               cairo.HAS_IMAGE_SURFACE
               cairo.HAS_HAS_PDF_SURFACE
               cairo.HAS_PNG_FUNCTIONS
               cairo.HAS_PS_SURFACE
               cairo.HAS_SVG_SURFACE
               cairo.HAS_USER_FONT
               cairo.HAS_QUARTZ_SURFACE
               cairo.HAS_WIN32_FONT
               cairo.HAS_WIN32_SURFACE
               cairo.HAS_XCB_SURFACE
               cairo.HAS_XLIB_SURFACE

   1 if the feature is present in the underlying C cairo library,
   0 otherwise

.. _mattributes_antialias:
.. attribute:: cairo.ANTIALIAS_DEFAULT
               cairo.ANTIALIAS_NONE
               cairo.ANTIALIAS_GRAY
               cairo.ANTIALIAS_SUBPIXEL

* *cairo.ANTIALIAS_DEFAULT*: Use the default antialiasing for the subsystem and target device
* *cairo.ANTIALIAS_NONE*: Use a bilevel alpha mask
* *cairo.ANTIALIAS_GRAY*: Perform single-color antialiasing (using shades of gray for black text on a white background, for example).
* *cairo.ANTIALIAS_SUBPIXEL*: Perform antialiasing by taking advantage of the order of subpixel elements on devices  such as LCD panels

 ANTIALIAS_x Specifies the type of antialiasing to do when rendering text or shapes.

.. _mattributes_content:
.. attribute:: cairo.CONTENT_COLOR
               cairo.CONTENT_ALPHA
               cairo.CONTENT_COLOR_ALPHA

* *cairo.CONTENT_COLOR*: The surface will hold color content only.
* *cairo.CONTENT_ALPHA*: The surface will hold alpha content only.
* *cairo.CONTENT_COLOR_ALPHA*: The surface will hold color and alpha content.

 These attributes are used to describe the content that a surface will
 contain, whether color information, alpha information (translucence
 vs. opacity), or both.

.. _mattributes_extend:
.. attribute:: cairo.EXTEND_NONE
               cairo.EXTEND_REPEAT
               cairo.EXTEND_REFLECT
               cairo.EXTEND_PAD

* *cairo.EXTEND_NONE*: pixels outside of the source pattern are fully transparent
* *cairo.EXTEND_REPEAT*: the pattern is tiled by repeating
* *cairo.EXTEND_REFLECT*: the pattern is tiled by reflecting at the edges (Implemented for surface patterns since 1.6)
* *cairo.EXTEND_PAD*: pixels outside of the pattern copy the closest pixel from the source (Since 1.2; but only implemented for surface patterns since 1.6)

 These attributes are used to describe how pattern color/alpha will be
 determined for areas "outside" the pattern's natural area, (for example,
 outside the surface bounds or outside the gradient geometry).

 The default extend mode is *cairo.EXTEND_NONE* for surface patterns and *cairo.EXTEND_PAD* for gradient patterns.

 New entries may be added in future versions.


.. attribute:: cairo.FILL_RULE_WINDING
               cairo.FILL_RULE_EVEN_ODD

* *cairo.FILL_RULE_WINDING*: If the path crosses the ray from left-to-right, counts +1. If the path crosses the ray from right to left, counts -1. (Left and right are determined from the perspective of looking along the ray from the starting point.) If the total count is non-zero, the point will be filled.
* *cairo.FILL_RULE_EVEN_ODD*: Counts the total number of intersections, without regard to the orientation of the contour. If the total number of intersections is odd, the point will be filled.

 These attributes are used to select how paths are filled. For both fill
 rules, whether or not a point is included in the fill is determined by taking
 a ray from that point to infinity and looking at intersections with the
 path. The ray can be in any direction, as long as it doesn't pass through the
 end point of a segment or have a tricky intersection such as intersecting
 tangent to the path. (Note that filling is not actually implemented in this
 way. This is just a description of the rule that is applied.)

 The default fill rule is *cairo.FILL_RULE_WINDING*.

 New entries may be added in future versions.

.. _mattributes_filter:
.. attribute:: cairo.FILTER_FAST
   	       cairo.FILTER_GOOD
   	       cairo.FILTER_BEST
   	       cairo.FILTER_NEAREST
   	       cairo.FILTER_BILINEAR
   	       cairo.FILTER_GAUSSIAN

* *cairo.FILTER_FAST*: A high-performance filter, with quality similar *cairo.FILTER_NEAREST*
* *cairo.FILTER_GOOD*: A reasonable-performance filter, with quality similar to *cairo.FILTER_BILINEAR*
* *cairo.FILTER_BEST*: The highest-quality available, performance may not be suitable for interactive use.
* *cairo.FILTER_NEAREST*: Nearest-neighbor filtering
* *cairo.FILTER_BILINEAR*: Linear interpolation in two dimensions
* *cairo.FILTER_GAUSSIAN*: This filter value is currently unimplemented, and should not be used in current code.

 These attributes are used to indicate what filtering should be applied when reading pixel values from patterns. See :meth:`SurfacePattern.set_filter` for indicating the desired filter to be used with a particular pattern.

.. attribute:: cairo.FONT_WEIGHT_NORMAL
               cairo.FONT_WEIGHT_BOLD

* *cairo.FONT_WEIGHT_NORMAL*: Normal font weight
* *cairo.FONT_WEIGHT_BOLD*: Bold font weight

 These attributes specify variants of a font face based on their weight.

.. attribute:: cairo.FONT_SLANT_NORMAL
               cairo.FONT_SLANT_ITALIC
               cairo.FONT_SLANT_OBLIQUE

* *cairo.FONT_SLANT_NORMAL*: Upright font style
* *cairo.FONT_SLANT_ITALIC*: Italic font style
* *cairo.FONT_SLANT_OBLIQUE*: Oblique font style

 These attributes specify variants of a font face based on their slant.

.. attribute:: cairo.FORMAT_ARGB32
   	       cairo.FORMAT_RGB24
   	       cairo.FORMAT_A8
   	       cairo.FORMAT_A1
   	       cairo.FORMAT_RGB16_565

* *cairo.FORMAT_ARGB32*: each pixel is a 32-bit quantity, with alpha in the upper 8 bits, then red, then green, then blue. The 32-bit quantities are stored native-endian. Pre-multiplied alpha is used. (That is, 50% transparent red is 0x80800000, not 0x80ff0000.)
* *cairo.FORMAT_RGB24*: each pixel is a 32-bit quantity, with the upper 8 bits unused. Red, Green, and Blue are stored in the remaining 24 bits in that order.
* *cairo.FORMAT_A8*: each pixel is a 8-bit quantity holding an alpha value.
* *cairo.FORMAT_A1*: each pixel is a 1-bit quantity holding an alpha value. Pixels are packed together into 32-bit quantities. The ordering of the bits matches the endianess of the platform. On a big-endian machine, the first pixel is in the uppermost bit, on a little-endian machine the first pixel is in the least-significant bit.
* *cairo.FORMAT_RGB16_565*: This format value is deprecated. It has never been properly implemented in cairo and should not be used by applications. (since 1.2)

 These attributes are used to identify the memory format of image data.

 New entries may be added in future versions.

.. _mattributes_hint_metrics:
.. attribute:: cairo.HINT_METRICS_DEFAULT
   	       cairo.HINT_METRICS_OFF
   	       cairo.HINT_METRICS_ON

* *cairo.HINT_METRICS_DEFAULT*: Hint metrics in the default manner for the font backend and target device
* *cairo.HINT_METRICS_OFF*: Do not hint font metrics
* *cairo.HINT_METRICS_ON*: Hint font metrics

 These attributes specify whether to hint font metrics; hinting font metrics means quantizing them so that they are integer values in device space. Doing this improves the consistency of letter and line spacing, however it also means that text will be laid out differently at different zoom factors.

.. _mattributes_hint_style:
.. attribute:: cairo.HINT_STYLE_DEFAULT
   	       cairo.HINT_STYLE_NONE
   	       cairo.HINT_STYLE_SLIGHT
   	       cairo.HINT_STYLE_MEDIUM
   	       cairo.HINT_STYLE_FULL

* *cairo.HINT_STYLE_DEFAULT*: Use the default hint style for font backend and target device
* *cairo.HINT_STYLE_NONE*: Do not hint outlines
* *cairo.HINT_STYLE_SLIGHT*: Hint outlines slightly to improve contrast while retaining good fidelity to the original shapes.
* *cairo.HINT_STYLE_MEDIUM*: Hint outlines with medium strength giving a compromise between fidelity to the original shapes and contrast
* *cairo.HINT_STYLE_FULL*: Hint outlines to maximize contrast

 These attributes specify the type of hinting to do on font outlines. Hinting is the process of fitting outlines to the pixel grid in order to improve the appearance of the result. Since hinting outlines involves distorting them, it also reduces the faithfulness to the original outline shapes. Not all of the outline hinting styles are supported by all font backends.

 New entries may be added in future versions.

.. attribute:: cairo.LINE_CAP_BUTT
   	       cairo.LINE_CAP_ROUND
   	       cairo.LINE_CAP_SQUARE

* *cairo.LINE_CAP_BUTT*: start(stop) the line exactly at the start(end) point
* *cairo.LINE_CAP_ROUND*: use a round ending, the center of the circle is the end point
* *cairo.LINE_CAP_SQUARE*: use squared ending, the center of the square is the end point

 These attributes specify how to render the endpoints of the path when stroking.

 The default line cap style is *cairo.LINE_CAP_BUTT*

.. attribute:: cairo.LINE_JOIN_MITER
   	       cairo.LINE_JOIN_ROUND
   	       cairo.LINE_JOIN_BEVEL

* *cairo.LINE_JOIN_MITER*: use a sharp (angled) corner, see :meth:`Context.set_miter_limit`
* *cairo.LINE_JOIN_ROUND*: use a rounded join, the center of the circle is the joint point
* *cairo.LINE_JOIN_BEVEL*: use a cut-off join, the join is cut off at half the line width from the joint point

 These attributes specify how to render the junction of two lines when stroking.

 The default line join style is *cairo.LINE_JOIN_MITER*

.. attribute:: cairo.OPERATOR_CLEAR
               cairo.OPERATOR_SOURCE
   	       cairo.OPERATOR_OVER
   	       cairo.OPERATOR_IN
   	       cairo.OPERATOR_OUT
   	       cairo.OPERATOR_ATOP
               cairo.OPERATOR_DEST
   	       cairo.OPERATOR_DEST_OVER
   	       cairo.OPERATOR_DEST_IN
   	       cairo.OPERATOR_DEST_OUT
   	       cairo.OPERATOR_DEST_ATOP
               cairo.OPERATOR_XOR
   	       cairo.OPERATOR_ADD
   	       cairo.OPERATOR_SATURATE

* *cairo.OPERATOR_CLEAR*: clear destination layer (bounded)
* *cairo.OPERATOR_SOURCE*: replace destination layer (bounded)
* *cairo.OPERATOR_OVER*: draw source layer on top of destination layer (bounded)
* *cairo.OPERATOR_IN*: draw source where there was destination content (unbounded)
* *cairo.OPERATOR_OUT*: draw source where there was no destination content (unbounded)
* *cairo.OPERATOR_ATOP*: draw source on top of destination content and only there
* *cairo.OPERATOR_DEST*: ignore the source
* *cairo.OPERATOR_DEST_OVER*: draw destination on top of source
* *cairo.OPERATOR_DEST_IN*: leave destination only where there was source content (unbounded)
* *cairo.OPERATOR_DEST_OUT*: leave destination only where there was no source content
* *cairo.OPERATOR_DEST_ATOP*: leave destination on top of source content and only there (unbounded)
* *cairo.OPERATOR_XOR*: source and destination are shown where there is only one of them
* *cairo.OPERATOR_ADD*: source and destination layers are accumulated
* *cairo.OPERATOR_SATURATE*: like over, but assuming source and dest are disjoint geometries

 These attributes are used to set the compositing operator for all cairo
 drawing operations.

 The default operator is *cairo.OPERATOR_OVER*.

 The operators marked as *unbounded* modify their destination even outside
 of the mask layer (that is, their effect is not bound by the mask layer).
 However, their effect can still be limited by way of clipping.

 To keep things simple, the operator descriptions here
 document the behavior for when both source and destination are either fully
 transparent or fully opaque.  The actual implementation works for
 translucent layers too.

 For a more detailed explanation of the effects of each operator, including
 the mathematical definitions, see http://cairographics.org/operators.

.. attribute:: cairo.PATH_MOVE_TO
   	       cairo.PATH_LINE_TO
   	       cairo.PATH_CURVE_TO
   	       cairo.PATH_CLOSE_PATH

* *cairo.PATH_MOVE_TO*: A move-to operation
* *cairo.PATH_LINE_TO*: A line-to operation
* *cairo.PATH_CURVE_TO*: A curve-to operation
* *cairo.PATH_CLOSE_PATH*: A close-path operation

 These attributes are used to describe the type of one portion of a path
 when represented as a :class:`Path`.

.. See #cairo_path_data_t for details.

.. attribute:: cairo.PS_LEVEL_2
               cairo.PS_LEVEL_3

* *cairo.PS_LEVEL_2*: The language level 2 of the PostScript specification.
* *cairo.PS_LEVEL_3*: The language level 3 of the PostScript specification.

 These attributes are used to describe the language level of the PostScript
 Language Reference that a generated PostScript file will conform to.  Note:
 the attributes are only defined when cairo has been compiled with PS
 support enabled.

.. _mattributes_subpixel:
.. attribute:: cairo.SUBPIXEL_ORDER_DEFAULT
   	       cairo.SUBPIXEL_ORDER_RGB
   	       cairo.SUBPIXEL_ORDER_BGR
   	       cairo.SUBPIXEL_ORDER_VRGB
   	       cairo.SUBPIXEL_ORDER_VBGR

* *cairo.SUBPIXEL_ORDER_DEFAULT*: Use the default subpixel order for for the target device
* *cairo.SUBPIXEL_ORDER_RGB*: Subpixel elements are arranged horizontally with red at the left
* *cairo.SUBPIXEL_ORDER_BGR*:  Subpixel elements are arranged horizontally with blue at the left
* *cairo.SUBPIXEL_ORDER_VRGB*: Subpixel elements are arranged vertically with red at the top
* *cairo.SUBPIXEL_ORDER_VBGR*: Subpixel elements are arranged vertically with blue at the top

 The subpixel order specifies the order of color elements within
 each pixel on the display device when rendering with an
 antialiasing mode of :attr:`cairo.ANTIALIAS_SUBPIXEL`.
