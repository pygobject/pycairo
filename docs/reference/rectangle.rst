.. _rectangle:

*********
Rectangle
*********

.. currentmodule:: cairo

class Rectangle(tuple)
======================

.. class:: Rectangle(x, y, width, height)

    :param float x:
        X coordinate of the left side of the rectangle
    :param float y:
        Y coordinate of the the top side of the rectangle
    :param float width:
        width of the rectangle
    :param float height:
        height of the rectangle
    :rtype: Rectangle

    .. versionadded:: 1.15
        In prior versions a (float, float, float, float) tuple was used instead
        of :class:`Rectangle`.

    A data structure for holding a rectangle.

    .. attribute:: x

        :class:`float`

    .. attribute:: y

        :class:`float`

    .. attribute:: width

        :class:`float`

    .. attribute:: height

        :class:`float`
