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
