.. _patterns:

********
Patterns
********

.. currentmodule:: cairo


Patterns are the paint with which cairo draws. The primary use of patterns is
as the source for all cairo drawing operations, although they can also be used
as masks, that is, as the brush too.

A cairo *Pattern* is created by using one of the *PatternType* constructors
listed below, or implicitly through *Context.set_source_<type>()* methods.


class Pattern()
===============

*Pattern* is the abstract base class from which all the other pattern classes
derive. It cannot be instantiated directly.

.. class:: Pattern()

   .. method:: get_extend()

      :returns: the current extend strategy used for drawing the *Pattern*.
      :rtype: int

      Gets the current extend mode for the *Pattern*. See
      :ref:`EXTEND attributes <constants_EXTEND>`
      for details on the semantics of each extend strategy.

   .. method:: get_matrix()

      :returns: a new :class:`Matrix` which stores a copy of the *Pattern's* transformation matrix

   .. method:: set_extend(extend)

      :param extend: an :ref:`EXTEND <constants_EXTEND>` describing how the
        area outside of the *Pattern* will be drawn

      Sets the mode to be used for drawing outside the area of a *Pattern*.

      The default extend mode is :data:`cairo.EXTEND_NONE` for
      :class:`SurfacePattern` and :data:`cairo.EXTEND_PAD` for
      :class:`Gradient` Patterns.

   .. method:: set_matrix(matrix)

      :param matrix: a :class:`Matrix`

      Sets the *Pattern's* transformation matrix to *matrix*. This matrix is a
      transformation from user space to pattern space.

      When a *Pattern* is first created it always has the identity matrix for
      its transformation matrix, which means that pattern space is initially
      identical to user space.

      Important: Please note that the direction of this transformation matrix
      is from user space to pattern space. This means that if you imagine the
      flow from a *Pattern* to user space (and on to device space), then
      coordinates in that flow will be transformed by the inverse of the
      *Pattern* matrix.

      For example, if you want to make a *Pattern* appear twice as large as it
      does by default the correct code to use is::

        matrix = cairo.Matrix(xx=0.5,yy=0.5)
        pattern.set_matrix(matrix)

      Meanwhile, using values of 2.0 rather than 0.5 in the code above would
      cause the *Pattern* to appear at half of its default size.

      Also, please note the discussion of the user-space locking semantics of
      :class:`Context.set_source`.


class SolidPattern(:class:`Pattern`)
====================================

.. class:: SolidPattern(red, green, blue, alpha=1.0)

   :param red: red component of the color
   :type red: float
   :param green: green component of the color
   :type green: float
   :param blue: blue component of the color
   :type blue: float
   :param alpha: alpha component of the color
   :type alpha: float
   :returns: a new *SolidPattern*
   :raises: *MemoryError* in case of no memory

   Creates a new *SolidPattern* corresponding to a translucent color. The
   color components are floating point numbers in the range 0 to 1. If the
   values passed in are outside that range, they will be clamped.


   .. method:: get_rgba()

      :returns: (red, green, blue, alpha) a tuple of float

      Gets the solid color for a *SolidPattern*.

      .. versionadded:: 1.4


class SurfacePattern(:class:`Pattern`)
======================================

.. class:: SurfacePattern(surface)

   :param surface: a cairo :class:`Surface`
   :returns: a newly created *SurfacePattern* for the given surface.
   :raises: *MemoryError* in case of no memory.

   .. method:: get_filter()

      :returns: the current :ref:`FILTER <constants_filter>` used for
        resizing the *SurfacePattern*.

   .. method:: get_surface()

      :returns: the :class:`Surface` of the *SurfacePattern*.

      .. versionadded:: 1.4

   .. method:: set_filter(filter)

      :param filter: a :ref:`FILTER <constants_filter>` describing the filter
        to use for resizing the *Pattern*

      Note that you might want to control filtering even when you do not have
      an explicit *Pattern* object, (for example when using
      :meth:`Context.set_source_surface`). In these cases, it is convenient to use
      :meth:`Context.get_source` to get access to the pattern that cairo creates
      implicitly. For example::

        context.set_source_surface(image, x, y)
        surfacepattern.set_filter(context.get_source(), cairo.FILTER_NEAREST)


class Gradient(:class:`Pattern`)
================================

*Gradient* is an abstract base class from which other *Pattern* classes
derive. It cannot be instantiated directly.

.. class:: Gradient()

   .. method:: add_color_stop_rgb(offset, red, green, blue)

      :param offset: an offset in the range [0.0 .. 1.0]
      :type offset: float
      :param red: red component of color
      :type red: float
      :param green: green component of color
      :type green: float
      :param blue: blue component of color
      :type blue: float

      Adds an opaque color stop to a *Gradient* pattern. The offset specifies
      the location along the gradient's control vector. For example, a
      *LinearGradient's* control vector is from (x0,y0) to (x1,y1) while a
      *RadialGradient's* control vector is from any point on the start circle
      to the corresponding point on the end circle.

      The color is specified in the same way as in :meth:`Context.set_source_rgb`.

      If two (or more) stops are specified with identical offset values, they
      will be sorted according to the order in which the stops are added,
      (stops added earlier will compare less than stops added later). This can
      be useful for reliably making sharp color transitions instead of the
      typical blend.

   .. method:: add_color_stop_rgba(offset, red, green, blue, alpha)

      :param offset: an offset in the range [0.0 .. 1.0]
      :type offset: float
      :param red: red component of color
      :type red: float
      :param green: green component of color
      :type green: float
      :param blue: blue component of color
      :type blue: float
      :param alpha: alpha component of color
      :type alpha: float

      Adds an opaque color stop to a *Gradient* pattern. The offset specifies
      the location along the gradient's control vector. For example, a
      *LinearGradient's* control vector is from (x0,y0) to (x1,y1) while a
      *RadialGradient's* control vector is from any point on the start circle
      to the corresponding point on the end circle.

      The color is specified in the same way as in :meth:`Context.set_source_rgb`.

      If two (or more) stops are specified with identical offset values, they
      will be sorted according to the order in which the stops are added,
      (stops added earlier will compare less than stops added later). This can
      be useful for reliably making sharp color transitions instead of the
      typical blend.


class LinearGradient(:class:`Gradient`)
=======================================
.. class:: LinearGradient(x0, y0, x1, y1)

   :param x0: x coordinate of the start point
   :type x0: float
   :param y0: y coordinate of the start point
   :type y0: float
   :param x1: x coordinate of the end point
   :type x1: float
   :param y1: y coordinate of the end point
   :type y1: float
   :returns: a new *LinearGradient*
   :raises: *MemoryError* in case of no memory

   Create a new *LinearGradient* along the line defined by (x0, y0) and (x1,
   y1).  Before using the *Gradient* pattern, a number of color stops should
   be defined using :meth:`Gradient.add_color_stop_rgb` or
   :meth:`Gradient.add_color_stop_rgba`

   Note: The coordinates here are in pattern space. For a new *Pattern*,
   pattern space is identical to user space, but the relationship between the
   spaces can be changed with :meth:`Pattern.set_matrix`

   .. method:: get_linear_points()

      :returns: (x0, y0, x1, y1) - a tuple of float

        * x0: return value for the x coordinate of the first point
        * y0: return value for the y coordinate of the first point
        * x1: return value for the x coordinate of the second point
        * y1: return value for the y coordinate of the second point

      Gets the gradient endpoints for a *LinearGradient*.

      .. versionadded:: 1.4


class RadialGradient(:class:`Gradient`)
=======================================
.. class:: RadialGradient(cx0, cy0, radius0, cx1, cy1, radius1)

   :param cx0: x coordinate for the center of the start circle
   :type cx0: float
   :param cy0: y coordinate for the center of the start circle
   :type cy0: float
   :param radius0: radius of the start circle
   :type radius0: float
   :param cx1: x coordinate for the center of the end circle
   :type cx1: float
   :param cy1: y coordinate for the center of the end circle
   :type cy1: float
   :param radius1: radius of the end circle
   :type radius1: float
   :returns: the newly created *RadialGradient*
   :raises: *MemoryError* in case of no memory

   Creates a new *RadialGradient* pattern between the two circles defined by
   (cx0, cy0, radius0) and (cx1, cy1, radius1).  Before using the gradient
   pattern, a number of color stops should be defined using
   :meth:`Gradient.add_color_stop_rgb` or :meth:`Gradient.add_color_stop_rgba`.

   Note: The coordinates here are in pattern space. For a new pattern, pattern
   space is identical to user space, but the relationship between the spaces
   can be changed with :meth:`Pattern.set_matrix`.

   .. method:: get_radial_circles()

      :returns: (x0, y0, r0, x1, y1, r1) - a tuple of float

	* x0: return value for the x coordinate of the center of the first circle
	* y0: return value for the y coordinate of the center of the first circle
	* r0: return value for the radius of the first circle
	* x1: return value for the x coordinate of the center of the second circle
	* y1: return value for the y coordinate of the center of the second circle
	* r1: return value for the radius of the second circle

      Gets the *Gradient* endpoint circles for a *RadialGradient*, each
      specified as a center coordinate and a radius.

      .. versionadded:: 1.4
