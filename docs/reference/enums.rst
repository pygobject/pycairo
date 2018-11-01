=====
Enums
=====

Before Pycairo 1.13 most of the enum values defined here where only available
as constants on the module level. See :ref:`legacy_constants`.

.. currentmodule:: cairo

.. class:: Antialias

    Specifies the type of antialiasing to do when rendering text or shapes.

    .. versionadded:: 1.13

    .. attribute:: DEFAULT

        Use the default antialiasing for the subsystem and target device

    .. attribute:: NONE

        Use a bilevel alpha mask

    .. attribute:: GRAY

        Perform single-color antialiasing (using shades of gray for black text
        on a white background, for example).

    .. attribute:: SUBPIXEL

        Perform antialiasing by taking advantage of the order of subpixel
        elements on devices such as LCD panels.

    .. attribute:: FAST

        Hint that the backend should perform some antialiasing but prefer
        speed over quality.

    .. attribute:: GOOD

        The backend should balance quality against performance.

    .. attribute:: BEST

        Hint that the backend should render at the highest quality,
        sacrificing speed if necessary.


.. class:: Content

    These constants are used to describe the content that a :class:`Surface`
    will contain, whether color information, alpha information (translucence
    vs. opacity), or both.

    .. versionadded:: 1.13

    .. attribute:: COLOR

        The surface will hold color content only.

    .. attribute:: ALPHA

        The surface will hold alpha content only.

    .. attribute:: COLOR_ALPHA

        The surface will hold color and alpha content.


.. class:: Extend

    These constants are used to describe how :class:`Pattern` color/alpha will
    be determined for areas "outside" the pattern's natural area, (for
    example, outside the surface bounds or outside the gradient geometry).

    The default extend mode is :attr:`NONE` for :class:`SurfacePattern` and
    :attr:`PAD` for :class:`Gradient` patterns.

    .. versionadded:: 1.13

    .. attribute:: NONE

        pixels outside of the source pattern are fully transparent

    .. attribute:: REPEAT

        the pattern is tiled by repeating

    .. attribute:: REFLECT

        the pattern is tiled by reflecting at the edges (Implemented for
        surface patterns since 1.6)

    .. attribute:: PAD

        pixels outside of the pattern copy the closest pixel from the source
        (Since 1.2; but only implemented for surface patterns since 1.6)


.. class:: FillRule

    These constants are used to select how paths are filled. For both fill
    rules, whether or not a point is included in the fill is determined by
    taking a ray from that point to infinity and looking at intersections with
    the path. The ray can be in any direction, as long as it doesn't pass
    through the end point of a segment or have a tricky intersection such as
    intersecting tangent to the path. (Note that filling is not actually
    implemented in this way. This is just a description of the rule that is
    applied.)

    The default fill rule is :attr:`WINDING`.

    .. versionadded:: 1.13

    .. attribute:: WINDING

       If the path crosses the ray from left-to-right, counts +1. If the path
       crosses the ray from right to left, counts -1. (Left and right are
       determined from the perspective of looking along the ray from the
       starting point.) If the total count is non-zero, the point will be
       filled.

    .. attribute:: EVEN_ODD

       Counts the total number of intersections, without regard to the
       orientation of the contour. If the total number of intersections is
       odd, the point will be filled.


.. class:: Filter

    These constants are used to indicate what filtering should be applied when
    reading pixel values from patterns. See :meth:`Pattern.set_filter` for
    indicating the desired filter to be used with a particular pattern.

    .. versionadded:: 1.13

    .. attribute:: FAST

        A high-performance filter, with quality similar *FILTER_NEAREST*

    .. attribute:: GOOD

        A reasonable-performance filter, with quality similar to
        *FILTER_BILINEAR*

    .. attribute:: BEST

        The highest-quality available, performance may not be suitable for
        interactive use.

    .. attribute:: NEAREST

        Nearest-neighbor filtering

    .. attribute:: BILINEAR

        Linear interpolation in two dimensions

    .. attribute:: GAUSSIAN

        This filter value is currently unimplemented, and should not be used
        in current code.


.. class:: FontSlant

    These constants specify variants of a :class:`FontFace` based on their
    slant.

    .. versionadded:: 1.13

    .. attribute:: NORMAL

        Upright font style

    .. attribute:: ITALIC

        Italic font style

    .. attribute:: OBLIQUE

        Oblique font style


.. class:: FontWeight

    These constants specify variants of a :class:`FontFace` based on their
    weight.

    .. versionadded:: 1.13

    .. attribute:: NORMAL

        Normal font weight

    .. attribute:: BOLD

        Bold font weight


.. class:: Format

    These constants are used to identify the memory format of
    :class:`ImageSurface` data.

    New entries may be added in future versions.

    .. versionadded:: 1.13

    .. method:: stride_for_width(width)

        :param int width: the desired width of an :class:`ImageSurface`
            to be created.
        :returns: the appropriate stride to use given the desired format and
            width, or -1 if either the format is invalid or the width too
            large.
        :rtype: int

        This method provides a stride value that will respect all alignment
        requirements of the accelerated image-rendering code within cairo.
        Typical usage will be of the form::

            format = cairo.Format.RGB24
            stride = format.stride_for_width(width)
            surface = cairo.ImageSurface.create_for_data(
                data, format, width, height, stride)

        Also available under
        :meth:`cairo.ImageSurface.format_stride_for_width`.

        .. versionadded:: 1.14

    .. attribute:: INVALID

        no such format exists or is supported.

    .. attribute:: ARGB32

        each pixel is a 32-bit quantity, with alpha in the upper 8 bits, then
        red, then green, then blue. The 32-bit quantities are stored
        native-endian. Pre-multiplied alpha is used. (That is, 50% transparent
        red is 0x80800000, not 0x80ff0000.)

    .. attribute:: RGB24

        each pixel is a 32-bit quantity, with the upper 8 bits unused. Red,
        Green, and Blue are stored in the remaining 24 bits in that order.

    .. attribute:: A8

        each pixel is a 8-bit quantity holding an alpha value.

    .. attribute:: A1

        each pixel is a 1-bit quantity holding an alpha value. Pixels are
        packed together into 32-bit quantities. The ordering of the bits
        matches the endianess of the platform. On a big-endian machine, the
        first pixel is in the uppermost bit, on a little-endian machine the
        first pixel is in the least-significant bit.

    .. attribute:: RGB16_565

        each pixel is a 16-bit quantity with red in the upper 5 bits, then
        green in the middle 6 bits, and blue in the lower 5 bits.

    .. attribute:: RGB30

        like :data:`RGB24` but with 10bpc.


.. class:: HintMetrics

    These constants specify whether to hint font metrics; hinting font metrics
    means quantizing them so that they are integer values in device space.
    Doing this improves the consistency of letter and line spacing, however it
    also means that text will be laid out differently at different zoom
    factors.

    .. versionadded:: 1.13

    .. attribute:: DEFAULT

        Hint metrics in the default manner for the font backend and target
        device

    .. attribute:: OFF

        Do not hint font metrics

    .. attribute:: ON

        Hint font metrics


.. class:: HintStyle

    These constants specify the type of hinting to do on font outlines.
    Hinting is the process of fitting outlines to the pixel grid in order to
    improve the appearance of the result. Since hinting outlines involves
    distorting them, it also reduces the faithfulness to the original outline
    shapes. Not all of the outline hinting styles are supported by all font
    backends.

    New entries may be added in future versions.

    .. versionadded:: 1.13

    .. attribute:: DEFAULT

        Use the default hint style for font backend and target device

    .. attribute:: NONE

        Do not hint outlines

    .. attribute:: SLIGHT

        Hint outlines slightly to improve contrast while retaining good
        fidelity to the original shapes.

    .. attribute:: MEDIUM

        Hint outlines with medium strength giving a compromise between fidelity
        to the original shapes and contrast

    .. attribute:: FULL

        Hint outlines to maximize contrast


.. class:: LineCap

    These constants specify how to render the endpoints of the path when
    stroking.

    The default line cap style is :attr:`BUTT`

    .. versionadded:: 1.13

    .. attribute:: BUTT

        start(stop) the line exactly at the start(end) point

    .. attribute:: ROUND

        use a round ending, the center of the circle is the end point

    .. attribute:: SQUARE

        use squared ending, the center of the square is the end point


.. class:: LineJoin

    These constants specify how to render the junction of two lines when
    stroking.

    The default line join style is :attr:`MITER`

    .. versionadded:: 1.13

    .. attribute:: MITER

        use a sharp (angled) corner, see :meth:`Context.set_miter_limit`

    .. attribute:: ROUND

        use a rounded join, the center of the circle is the joint point

    .. attribute:: BEVEL

        use a cut-off join, the join is cut off at half the line width from
        the joint point


.. class:: Operator

    These constants are used to set the compositing operator for all cairo
    drawing operations.

    The default operator is :attr:`OVER`.

    The operators marked as *unbounded* modify their destination even outside
    of the mask layer (that is, their effect is not bound by the mask layer).
    However, their effect can still be limited by way of clipping.

    To keep things simple, the operator descriptions here document the
    behavior for when both source and destination are either fully transparent
    or fully opaque.  The actual implementation works for translucent layers
    too.

    For a more detailed explanation of the effects of each operator, including
    the mathematical definitions, see https://cairographics.org/operators.

    .. versionadded:: 1.13

    .. attribute:: CLEAR

        clear destination layer (bounded)

    .. attribute:: SOURCE

        replace destination layer (bounded)

    .. attribute:: OVER

        draw source layer on top of destination layer (bounded)

    .. attribute:: IN

        draw source where there was destination content (unbounded)

    .. attribute:: OUT

        draw source where there was no destination content (unbounded)

    .. attribute:: ATOP

        draw source on top of destination content and only there

    .. attribute:: DEST

        ignore the source

    .. attribute:: DEST_OVER

        draw destination on top of source

    .. attribute:: DEST_IN

        leave destination only where there was source content (unbounded)

    .. attribute:: DEST_OUT

        leave destination only where there was no source content

    .. attribute:: DEST_ATOP

        leave destination on top of source content and only there (unbounded)

    .. attribute:: XOR

        source and destination are shown where there is only one of them

    .. attribute:: ADD

        source and destination layers are accumulated

    .. attribute:: SATURATE

        like over, but assuming source and dest are disjoint geometries

    .. attribute:: MULTIPLY

        source and destination layers are multiplied. This causes the result
        to be at least as dark as the darker inputs.

    .. attribute:: SCREEN

        source and destination are complemented and multiplied. This causes
        the result to be at least as light as the lighter inputs.

    .. attribute:: OVERLAY

        multiplies or screens, depending on the lightness of the destination
        color.

    .. attribute:: DARKEN

        replaces the destination with the source if it is darker, otherwise
        keeps the source.

    .. attribute:: LIGHTEN

        replaces the destination with the source if it is lighter, otherwise
        keeps the source.

    .. attribute:: COLOR_DODGE

        brightens the destination color to reflect the source color.

    .. attribute:: COLOR_BURN

        darkens the destination color to reflect the source color.

    .. attribute:: HARD_LIGHT

        Multiplies or screens, dependent on source color.

    .. attribute:: SOFT_LIGHT

        Darkens or lightens, dependent on source color.

    .. attribute:: DIFFERENCE

        Takes the difference of the source and destination color.

    .. attribute:: EXCLUSION

        Produces an effect similar to difference, but with lower contrast.

    .. attribute:: HSL_HUE

        Creates a color with the hue of the source and the saturation and
        luminosity of the target.

    .. attribute:: HSL_SATURATION

        Creates a color with the saturation of the source and the hue and
        luminosity of the target. Painting with this mode onto a gray area
        produces no change.

    .. attribute:: HSL_COLOR

        Creates a color with the hue and saturation of the source and the
        luminosity of the target. This preserves the gray levels of the target
        and is useful for coloring monochrome images or tinting color images.

    .. attribute:: HSL_LUMINOSITY

        Creates a color with the luminosity of the source and the hue and
        saturation of the target. This produces an inverse effect to
        :attr:`HSL_COLOR`


.. class:: PathDataType

    These constants are used to describe the type of one portion of a path
    when represented as a :class:`Path`.

    .. versionadded:: 1.13

    .. attribute:: MOVE_TO

        A move-to operation

    .. attribute:: LINE_TO

        A line-to operation

    .. attribute:: CURVE_TO

        A curve-to operation

    .. attribute:: CLOSE_PATH

        A close-path operation


.. class:: PSLevel

    These constants are used to describe the language level of the PostScript
    Language Reference that a generated PostScript file will conform to.
    Note: the constants are only defined when cairo has been compiled with PS
    support enabled.

    .. versionadded:: 1.13

    .. attribute:: LEVEL_2

        The language level 2 of the PostScript specification.

    .. attribute:: LEVEL_3

        The language level 3 of the PostScript specification.


.. class:: PDFVersion

    These constants are used to describe the version number of the PDF
    specification that a generated PDF file will conform to.

    .. versionadded:: 1.13

    .. attribute:: VERSION_1_4

        The version 1.4 of the PDF specification.

    .. attribute:: VERSION_1_5

        The version 1.5 of the PDF specification.


.. class:: SVGVersion

    These constants are used to describe the version number of the SVG
    specification that a generated SVG file will conform to.

    .. versionadded:: 1.13

    .. attribute:: VERSION_1_1

        The version 1.1 of the SVG specification.

    .. attribute:: VERSION_1_2

        The version 1.2 of the SVG specification.


.. class:: SubpixelOrder

    The subpixel order specifies the order of color elements within each pixel
    on the display device when rendering with an antialiasing mode of
    :attr:`Antialias.SUBPIXEL`.

    .. versionadded:: 1.13

    .. attribute:: DEFAULT

        Use the default subpixel order for for the target device

    .. attribute:: RGB

        Subpixel elements are arranged horizontally with red at the left

    .. attribute:: BGR

        Subpixel elements are arranged horizontally with blue at the left

    .. attribute:: VRGB

        Subpixel elements are arranged vertically with red at the top

    .. attribute:: VBGR

        Subpixel elements are arranged vertically with blue at the top


.. class:: RegionOverlap

    .. versionadded:: 1.13

    .. attribute:: IN

        The contents are entirely inside the region.

    .. attribute:: OUT

        The contents are entirely outside the region.

    .. attribute:: PART

        The contents are partially inside and partially outside the region.


.. class:: Status

    .. versionadded:: 1.13

    .. attribute:: SUCCESS
        NO_MEMORY
        INVALID_RESTORE
        INVALID_POP_GROUP
        NO_CURRENT_POINT
        INVALID_MATRIX
        INVALID_STATUS
        NULL_POINTER
        INVALID_STRING
        INVALID_PATH_DATA
        READ_ERROR
        WRITE_ERROR
        SURFACE_FINISHED
        SURFACE_TYPE_MISMATCH
        PATTERN_TYPE_MISMATCH
        INVALID_CONTENT
        INVALID_FORMAT
        INVALID_VISUAL
        FILE_NOT_FOUND
        INVALID_DASH
        INVALID_DSC_COMMENT
        INVALID_INDEX
        CLIP_NOT_REPRESENTABLE
        TEMP_FILE_ERROR
        INVALID_STRIDE
        FONT_TYPE_MISMATCH
        USER_FONT_IMMUTABLE
        USER_FONT_ERROR
        NEGATIVE_COUNT
        INVALID_CLUSTERS
        INVALID_SLANT
        INVALID_WEIGHT
        INVALID_SIZE
        USER_FONT_NOT_IMPLEMENTED
        DEVICE_TYPE_MISMATCH
        DEVICE_ERROR
        INVALID_MESH_CONSTRUCTION
        DEVICE_FINISHED
        LAST_STATUS

    .. attribute:: JBIG2_GLOBAL_MISSING

        .. versionadded:: 1.14

    .. attribute:: TAG_ERROR

        .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

    .. attribute:: FREETYPE_ERROR

        .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

    .. attribute:: WIN32_GDI_ERROR

        .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

    .. attribute:: PNG_ERROR

        .. versionadded:: 1.18.0 Only available with cairo 1.15.10+


.. class:: ScriptMode

    A set of script output variants.

    .. versionadded:: 1.14

    .. attribute:: ASCII

        the output will be in readable text (default)

    .. attribute:: BINARY

        the output will use byte codes.


.. class:: TextClusterFlags

    Specifies properties of a text cluster mapping.

    .. versionadded:: 1.14

    .. attribute:: BACKWARD

        The clusters in the cluster array map to glyphs in the glyph array
        from end to start.


.. class:: SurfaceObserverMode

    Whether operations should be recorded.

    .. versionadded:: 1.14

    .. attribute:: NORMAL

        no recording is done

    .. attribute:: RECORD_OPERATIONS

        operations are recorded


.. class:: PDFOutlineFlags

    :class:`PDFOutlineFlags` is used by the :meth:`PDFSurface.add_outline`
    method to specify the attributes of an outline item. These flags may be
    bitwise-or'd to produce any combination of flags.

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

    .. attribute:: OPEN

        The outline item defaults to open in the PDF viewer

    .. attribute:: BOLD

        The outline item is displayed by the viewer in bold text

    .. attribute:: ITALIC

        The outline item is displayed by the viewer in italic text


.. class:: SVGUnit

    :class:`SVGUnit` is used to describe the units valid for coordinates and
    lengths in the SVG specification.

    See also:

    * https://www.w3.org/TR/SVG/coords.htmlUnits
    * https://www.w3.org/TR/SVG/types.htmlDataTypeLength
    * https://www.w3.org/TR/css-values-3/lengths

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

    .. attribute:: USER

        User unit, a value in the current coordinate system. If used in the
        root element for the initial coordinate systems it corresponds to
        pixels.

    .. attribute:: EM

        The size of the element's font.

    .. attribute:: EX

        The x-height of the element’s font.

    .. attribute:: PX

        Pixels (1px = 1/96th of 1in).

    .. attribute:: IN

        Inches (1in = 2.54cm = 96px)

    .. attribute:: CM

        Centimeters (1cm = 96px/2.54).

    .. attribute:: MM

        Millimeters (1mm = 1/10th of 1cm).

    .. attribute:: PT

        Points (1pt = 1/72th of 1in).

    .. attribute:: PC

        Picas (1pc = 1/6th of 1in).

    .. attribute:: PERCENT

        Percent, a value that is some fraction of another reference value.


.. class:: PDFMetadata

    :class:`PDFMetadata` is used by the :meth:`PDFSurface.set_metadata` method
    to specify the metadata to set.

    .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

    .. attribute:: TITLE

        The document title

    .. attribute:: AUTHOR

        The document author

    .. attribute:: SUBJECT

        The document subject

    .. attribute:: KEYWORDS

        The document keywords

    .. attribute:: CREATOR

        The document creator

    .. attribute:: CREATE_DATE

        The document creation date

    .. attribute:: MOD_DATE

        The document modification date
