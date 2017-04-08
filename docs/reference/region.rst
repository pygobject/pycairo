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


   .. method:: copy()

      :returns: A newly allocated :class:`Region`.
      :raises: :exc:`NoMemory` if memory cannot be allocated.

      Allocates a new *Region* object copying the area from original.

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


