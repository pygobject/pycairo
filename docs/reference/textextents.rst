.. _textextents:

***********
TextExtents
***********

.. currentmodule:: cairo

class TextExtents(tuple)
========================

.. class:: TextExtents(x_bearing, y_bearing, width, height, x_advance, y_advance)

    :param float x_bearing:
        the horizontal distance from the origin to the leftmost part of the
        glyphs as drawn. Positive if the glyphs lie entirely to the right of
        the origin.
    :param float y_bearing:
        the vertical distance from the origin to the topmost part of the
        glyphs as drawn. Positive only if the glyphs lie completely below the
        origin; will usually be negative.
    :param float width:
        width of the glyphs as drawn
    :param float height:
        height of the glyphs as drawn
    :param float x_advance:
        distance to advance in the X direction after drawing these glyphs
    :param float y_advance:
        distance to advance in the Y direction after drawing these glyphs.
        Will typically be zero except for vertical text layout as found in
        East-Asian languages.
    :rtype: TextExtents

    .. versionadded:: 1.15
        In prior versions a (float, float, float, float, float, float) tuple
        was used instead of :class:`TextExtents`.

    The :class:`TextExtents` class stores the extents of a single glyph or a
    string of glyphs in user-space coordinates. Because text extents are in
    user-space coordinates, they are mostly, but not entirely, independent of
    the current transformation matrix. If you call ``context.scale(2.0,
    2.0)``, text will be drawn twice as big, but the reported text extents
    will not be doubled. They will change slightly due to hinting (so you
    can't assume that metrics are independent of the transformation matrix),
    but otherwise will remain unchanged.

    .. attribute:: x_bearing

        :class:`float`

    .. attribute:: y_bearing

        :class:`float`

    .. attribute:: width

        :class:`float`

    .. attribute:: height

        :class:`float`

    .. attribute:: x_advance

        :class:`float`

    .. attribute:: y_advance

        :class:`float`
