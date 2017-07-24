.. _glyph:

*****
Glyph
*****

.. currentmodule:: cairo

class Glyph(tuple)
==================

.. class:: Glyph(index, x, y)

    :param int index:
        glyph index in the font. The exact interpretation of the glyph index
        depends on the font technology being used.
    :param float x:
        the offset in the X direction between the origin used for drawing or
        measuring the string and the origin of this glyph.
    :param float y:
        the offset in the Y direction between the origin used for drawing or
        measuring the string and the origin of this glyph.
    :rtype: Glyph

    .. versionadded:: 1.15
        In prior versions a (int, float, float) tuple was used instead
        of :class:`Glyph`.

    The :class:`Glyph` holds information about a single glyph when drawing or
    measuring text. A font is (in simple terms) a collection of shapes used to
    draw text. A glyph is one of these shapes. There can be multiple glyphs
    for a single character (alternates to be used in different contexts, for
    example), or a glyph can be a ligature of multiple characters. Cairo
    doesn't expose any way of converting input text into glyphs, so in order
    to use the Cairo interfaces that take arrays of glyphs, you must directly
    access the appropriate underlying font system.

    Note that the offsets given by x and y are not cumulative. When drawing or
    measuring text, each glyph is individually positioned with respect to the
    overall origin

    .. attribute:: index

        :class:`int`

    .. attribute:: x

        :class:`float`

    .. attribute:: y

        :class:`float`
