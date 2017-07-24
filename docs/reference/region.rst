.. _region:

******
Region
******
Region — Representing a pixel-aligned area

.. currentmodule:: cairo


class Region()
==============
*Region* is a simple graphical data type representing an area of
integer-aligned rectangles. They are often used on raster surfaces to track
areas of interest, such as change or clip areas. 


.. class:: Region([rectangle_int|rectangle_ints])

    :param rectangle_int: a rectangle or a list of rectangle
    :type rectangle_int: :class:`RectangleInt` or [:class:`RectangleInt`]

    Allocates a new empty region object or a region object with the containing
    rectangle(s).

    .. versionadded:: 1.11.0

    .. method:: copy()

        :returns: A newly allocated :class:`Region`.
        :raises Error: if memory cannot be allocated.

        Allocates a new *Region* object copying the area from original.

    .. method:: get_extents()

        :returns: The bounding rectangle of region
        :rtype: RectangleInt

    .. method:: num_rectangles()

        :returns: The number of rectangles contained in region
        :rtype: int

    .. method:: get_rectangle(nth)

        :param int nth: a number indicating which rectangle should be returned
        :returns: The *nth* rectangle from the region
        :rtype: RectangleInt

    .. method:: is_empty()

        :returns: Whether region is empty
        :rtype: bool

    .. method:: contains_point(x, y)

        :param int x: The x coordinate of a point
        :param int y: The y coordinate of a point
        :returns: Whether (x , y) is contained in the region
        :rtype: bool

    .. method:: contains_rectangle(rectangle)

        :param RectangleInt rectangle:
        :returns: region overlap
        :rtype: cairo.RegionOverlap

        Checks whether rectangle is inside, outside or partially contained in
        region

    .. method:: equal(region)

        :param Region region:
        :returns: Whether both regions contained the same coverage
        :rtype: bool

    .. method:: translate(dx, dy)

        :param int dx: Amount to translate in the x direction
        :param int dy: Amount to translate in the y direction

        Translates region by (dx , dy ).

    .. method:: intersect(other)

        :param other:
        :type other: :class:`Region` or :class:`RectangleInt`
        :returns: The intersection of the region and the passed region or
            rectangle
        :rtype: Region

    .. method:: subtract(other)

        :param other:
        :type other: :class:`Region` or :class:`RectangleInt`
        :returns: The result of the subtraction of the region and the passed
            region or rectangle
        :rtype: Region

    .. method:: union(other)

        :param other:
        :type other: :class:`Region` or :class:`RectangleInt`
        :returns: The union of the region and the passed region or rectangle
        :rtype: Region

    .. method:: xor(other)

        :param other:
        :type other: :class:`Region` or :class:`RectangleInt`
        :returns: The exclusive difference of the region and the passed region
            or rectangle
        :rtype: Region


class RectangleInt()
====================
*RectangleInt* is a data structure for holding a rectangle with integer
coordinates.


.. class:: RectangleInt(x=0, y=0, width=0, height=0)

   :param x: X coordinate of the left side of the rectangle
   :type x: int
   :param y: Y coordinate of the the top side of the rectangle
   :type y: int
   :param width: width of the rectangle
   :type width: int
   :param height: height of the rectangle
   :type height: int

   Allocates a new *RectangleInt* object.

   .. versionadded:: 1.11.0

   .. attribute:: x

        :class:`int`

   .. attribute:: y

        :class:`int`

   .. attribute:: width

        :class:`int`

   .. attribute:: height

        :class:`int`
