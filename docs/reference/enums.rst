=====
Enums
=====

Many of the enum values are available as constants on the module level in
older versions of Pycairo. See :ref:`legacy_constants`.

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
