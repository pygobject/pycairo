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
      :rtype: cairo.Extend

      Gets the current extend mode for the *Pattern*. See
      :class:`cairo.Extend` attributes for details on the semantics of each
      extend strategy.

   .. method:: get_matrix()

      :returns: a new :class:`Matrix` which stores a copy of the *Pattern's* transformation matrix

   .. method:: get_filter()

      :returns: the current filter used for
        resizing the pattern.
      :rtype: cairo.Filter

      .. versionadded:: 1.12.0

         Used to be a method of :class:`SurfacePattern` before

   .. method:: set_filter(filter)

      :param cairo.Filter filter: a filter describing the filter
        to use for resizing the pattern

      Note that you might want to control filtering even when you do not have
      an explicit *Pattern* object, (for example when using
      :meth:`Context.set_source_surface`). In these cases, it is convenient to
      use :meth:`Context.get_source` to get access to the pattern that cairo
      creates implicitly. For example::

        context.set_source_surface(image, x, y)
        surfacepattern.set_filter(context.get_source(), cairo.FILTER_NEAREST)

      .. versionadded:: 1.12.0

         Used to be a method of :class:`SurfacePattern` before

   .. method:: set_extend(extend)

      :param cairo.Extend extend: an extend describing how the
        area outside of the *Pattern* will be drawn

      Sets the mode to be used for drawing outside the area of a *Pattern*.

      The default extend mode is :attr:`cairo.Extend.NONE` for
      :class:`SurfacePattern` and :attr:`cairo.Extend.PAD` for
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
   :raises: :exc:`MemoryError` in case of no memory

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
   :raises: :exc:`MemoryError` in case of no memory.

   .. method:: get_surface()

      :returns: the :class:`Surface` of the *SurfacePattern*.

      .. versionadded:: 1.4


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

   .. method:: get_color_stops_rgba()

      :returns: a list of (offset, red, green, blue, alpha) tuples of float
      :rtype: list

      Gets the color and offset information for all color stops specified in
      the given gradient pattern.

      .. versionadded:: 1.14


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
   :raises: :exc:`MemoryError` in case of no memory

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
   :raises: :exc:`MemoryError` in case of no memory

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


class MeshPattern(:class:`Pattern`)
===================================

.. class:: MeshPattern()

    :raises Error:
    :rtype: MeshPattern

    .. versionadded:: 1.14

    Create a new mesh pattern.

    Mesh patterns are tensor-product patch meshes (type 7 shadings in PDF).
    Mesh patterns may also be used to create other types of shadings that are
    special cases of tensor-product patch meshes such as Coons patch meshes
    (type 6 shading in PDF) and Gouraud-shaded triangle meshes (type 4 and 5
    shadings in PDF).

    Mesh patterns consist of one or more tensor-product patches, which should
    be defined before using the mesh pattern. Using a mesh pattern with a
    partially defined patch as source or mask will put the context in an error
    status with a status of :attr:`cairo.Status.INVALID_MESH_CONSTRUCTION`.

    A tensor-product patch is defined by 4 Bézier curves (side 0, 1, 2, 3) and
    by 4 additional control points (P0, P1, P2, P3) that provide further
    control over the patch and complete the definition of the tensor-product
    patch. The corner C0 is the first point of the patch.

    Degenerate sides are permitted so straight lines may be used. A zero
    length line on one side may be used to create 3 sided patches.

    ::

              C1     Side 1       C2
               +---------------+
               |               |
               |  P1       P2  |
               |               |
        Side 0 |               | Side 2
               |               |
               |               |
               |  P0       P3  |
               |               |
               +---------------+
             C0     Side 3        C3

    Each patch is constructed by first calling :meth:`begin_patch`, then
    :meth:`move_to` to specify the first point in the patch (C0). Then the
    sides are specified with calls to :meth:`curve_to` and :meth:`line_to`.

    The four additional control points (P0, P1, P2, P3) in a patch can be
    specified with :meth:`set_control_point`.

    At each corner of the patch (C0, C1, C2, C3) a color may be specified with
    :meth:`set_corner_color_rgb` or :meth:`set_corner_color_rgba`. Any corner
    whose color is not explicitly specified defaults to transparent black.

    A Coons patch is a special case of the tensor-product patch where the
    control points are implicitly defined by the sides of the patch. The
    default value for any control point not specified is the implicit value
    for a Coons patch, i.e. if no control points are specified the patch is a
    Coons patch.

    A triangle is a special case of the tensor-product patch where the control
    points are implicitly defined by the sides of the patch, all the sides are
    lines and one of them has length 0, i.e. if the patch is specified using
    just 3 lines, it is a triangle. If the corners connected by the 0-length
    side have the same color, the patch is a Gouraud-shaded triangle.

    Patches may be oriented differently to the above diagram. For example the
    first point could be at the top left. The diagram only shows the
    relationship between the sides, corners and control points. Regardless of
    where the first point is located, when specifying colors, corner 0 will
    always be the first point, corner 1 the point between side 0 and side 1
    etc.

    Calling :meth:`end_patch` completes the current patch. If less than 4
    sides have been defined, the first missing side is defined as a line from
    the current point to the first point of the patch (C0) and the other sides
    are degenerate lines from C0 to C0. The corners between the added sides
    will all be coincident with C0 of the patch and their color will be set to
    be the same as the color of C0.

    Additional patches may be added with additional calls to
    :meth:`begin_patch`/:meth:`end_patch`.

    ::

        # Add a Coons patch
        pattern = cairo.MeshPattern()
        pattern.begin_patch()
        pattern.move_to(0, 0)
        pattern.curve_to(30, -30, 60, 30, 100, 0)
        pattern.curve_to(60, 30, 130, 60, 100, 100)
        pattern.curve_to(60, 70, 30, 130, 0, 100)
        pattern.curve_to(30, 70, -30, 30, 0, 0)
        pattern.set_corner_color_rgb(0, 1, 0, 0)
        pattern.set_corner_color_rgb(1, 0, 1, 0)
        pattern.set_corner_color_rgb(2, 0, 0, 1)
        pattern.set_corner_color_rgb(3, 1, 1, 0)
        pattern.end_patch()

        # Add a Gouraud-shaded triangle
        pattern = cairo.MeshPattern()
        pattern.begin_patch()
        pattern.move_to(100, 100)
        pattern.line_to(130, 130)
        pattern.line_to(130, 70)
        pattern.set_corner_color_rgb(0, 1, 0, 0)
        pattern.set_corner_color_rgb(1, 0, 1, 0)
        pattern.set_corner_color_rgb(2, 0, 0, 1)
        pattern.end_patch()

    When two patches overlap, the last one that has been added is drawn over
    the first one.

    When a patch folds over itself, points are sorted depending on their
    parameter coordinates inside the patch. The v coordinate ranges from 0 to
    1 when moving from side 3 to side 1; the u coordinate ranges from 0 to 1
    when going from side 0 to side

    Points with higher v coordinate hide points with lower v coordinate. When
    two points have the same v coordinate, the one with higher u coordinate is
    above. This means that points nearer to side 1 are above points nearer to
    side 3; when this is not sufficient to decide which point is above (for
    example when both points belong to side 1 or side 3) points nearer to side
    2 are above points nearer to side 0.

    For a complete definition of tensor-product patches, see the PDF
    specification (ISO32000), which describes the parametrization in detail.

    Note: The coordinates are always in pattern space. For a new pattern,
    pattern space is identical to user space, but the relationship between the
    spaces can be changed with :meth:`Pattern.set_matrix`.

    .. method:: begin_patch()

        :raises Error:

        Begin a patch in a mesh pattern.

        After calling this function, the patch shape should be defined with
        :meth:`move_to`, :meth:`line_to` and :meth:`curve_to`.

        After defining the patch, :meth:`end_patch` must be called before
        using pattern as a source or mask.

    .. method:: curve_to(x1, y1, x2, y2, x3, y3)

        :param float x1: the X coordinate of the first control point
        :param float y1: the Y coordinate of the first control point
        :param float x2: the X coordinate of the second control point
        :param float y2: the Y coordinate of the second control point
        :param float x3: the X coordinate of the end of the curve
        :param float y3: the Y coordinate of the end of the curve
        :raises Error:

        Adds a cubic Bézier spline to the current patch from the current point
        to position (x3 , y3 ) in pattern-space coordinates, using (x1 , y1 )
        and (x2 , y2 ) as the control points.

        If the current patch has no current point before the call to
        :meth:`curve_to`, this function will behave as if
        preceded by a call to ``pattern.move_to(x1, y1)``.

        After this call the current point will be (x3 , y3 ).

    .. method:: end_patch()

        :raises Error:

        Indicates the end of the current patch in a mesh pattern.

        If the current patch has less than 4 sides, it is closed with a
        straight line from the current point to the first point of the patch
        as if :meth:`line_to` was used.

    .. method:: get_control_point(patch_num, point_num)

        :param int patch_num: the patch number to return data for
        :param int point_num: he control point number to return data for
        :returns: a (x, y) tuple of float - coordinates of the control point
        :rtype: tuple
        :raises Error:

        Gets the control point point_num of patch patch_num for a mesh
        pattern.

        ``patch_num`` can range from 0 to n-1 where n is the number returned
        by :meth:`get_patch_count`.

        Valid values for ``point_num`` are from 0 to 3 and identify the control
        points as explained in :class:`MeshPattern`.

    .. method:: get_corner_color_rgba(patch_num, corner_num)

        :param int patch_num: the patch number to return data for
        :param int corner_num: the corner number to return data for
        :returns: a (red, green, blue, alpha) tuple of float
        :rtype: tuple
        :raises Error:

        Gets the color information in corner ``corner_num`` of patch
        ``patch_num`` for a mesh pattern.

        ``patch_num`` can range from 0 to n-1 where n is the number returned
        by :meth:`get_patch_count`.

        Valid values for ``corner_num`` are from 0 to 3 and identify the
        corners as explained in :class:`MeshPattern`.

    .. method:: get_patch_count()

        :returns: number of patches
        :rtype: int

        Gets the number of patches specified in the given mesh pattern.

        The number only includes patches which have been finished by calling
        :meth:`end_patch`. For example it will be 0 during the definition of
        the first patch.

    .. method:: get_path(patch_num)

        :param int patch_num: the patch number to return data for
        :returns: the path defining the patch
        :rtype: Path
        :raises Error:

        Gets path defining the patch ``patch_num`` for a mesh pattern.

        ``patch_num`` can range from 0 to n-1 where n is the number returned
        by :meth:`get_patch_count`.

    .. method:: line_to(x, y)

        :param float x: the X coordinate of the end of the new line
        :param float y: the Y coordinate of the end of the new line
        :raises Error:

        Adds a line to the current patch from the current point to position (x
        , y ) in pattern-space coordinates.

        If there is no current point before the call to :meth:`line_to` this
        function will behave as ``pattern.move_to(x ,y)``.

        After this call the current point will be (x , y ).

    .. method:: move_to(x, y)

        :param float x: the X coordinate of the new position
        :param float y: the Y coordinate of the new position
        :raises Error:

        Define the first point of the current patch in a mesh pattern.

        After this call the current point will be (x , y ).

    .. method:: set_control_point(point_num, x, y)

        :param int point_num: the control point to set the position for
        :param float x: the X coordinate of the control point
        :param float y: the Y coordinate of the control point
        :raises Error:

        Set an internal control point of the current patch.

        Valid values for point_num are from 0 to 3 and identify the control
        points as explained in :class:`MeshPattern`.

    .. method:: set_corner_color_rgb(corner_num, red, green, blue)

        :param int corner_num: the corner to set the color for
        :param float red: red component of color
        :param float green: green component of color
        :param float blue: blue component of color
        :raises Error:

        Sets the color of a corner of the current patch in a mesh pattern.

        The color is specified in the same way as in
        :meth:`Context.set_source_rgb`.

        Valid values for corner_num are from 0 to 3 and identify the corners
        as explained in :class:`MeshPattern`.

    .. method:: set_corner_color_rgba(corner_num, red, green, blue, alpha)

        :param int corner_num: the corner to set the color for
        :param float red: red component of color
        :param float green: green component of color
        :param float blue: blue component of color
        :param float alpha: alpha component of color
        :raises Error:

        Sets the color of a corner of the current patch in a mesh pattern.

        The color is specified in the same way as in
        :meth:`Context.set_source_rgba`.

        Valid values for corner_num are from 0 to 3 and identify the corners
        as explained in :class:`MeshPattern`.


class RasterSourcePattern(:class:`Pattern`)
===========================================

The raster source provides the ability to supply arbitrary pixel data whilst
rendering. The pixels are queried at the time of rasterisation by means of
user callback functions, allowing for the ultimate flexibility. For example,
in handling compressed image sources, you may keep a MRU cache of decompressed
images and decompress sources on the fly and discard old ones to conserve
memory.

For the raster source to be effective, you must at least specify the acquire
and release callbacks which are used to retrieve the pixel data for the region
of interest and demark when it can be freed afterwards. Other callbacks are
provided for when the pattern is copied temporarily during rasterisation, or
more permanently as a snapshot in order to keep the pixel data available for
printing.


.. class:: RasterSourcePattern(content, width, height)

    :param Content content:
        content type for the pixel data that will be returned. Knowing the
        content type ahead of time is used for analysing the operation and
        picking the appropriate rendering path.
    :param int width:
        maximum size of the sample area
    :param int height:
        maximum size of the sample area
    :raises Error:
    :rtype: RasterSourcePattern

    Creates a new user pattern for providing pixel data.

    Use the setter functions to associate callbacks with the returned pattern.

    .. versionadded:: 1.15

    .. method:: set_acquire(acquire, release)

        :param acquire:
            acquire callback or :obj:`None` to unset it
        :type acquire: :obj:`callable`
        :param release:
            (optional) release callback or :obj:`None`
        :type release: :obj:`callable`
        :raises Error:

        Specifies the callbacks used to generate the image surface for a
        rendering operation (acquire) and the function used to cleanup that
        surface afterwards.

        The acquire callback should create a surface (preferably an image
        surface created to match the target using
        :meth:`Surface.create_similar_image`) that defines at least the region
        of interest specified by extents. The surface is allowed to be the
        entire sample area, but if it does contain a subsection of the sample
        area, the surface extents should be provided by setting the device
        offset (along with its width and height) using
        :meth:`Surface.set_device_offset`.

        .. function:: acquire(target, extents)

            :param Surface target:
                the rendering target surface
            :param RectangleInt extents:
                rectangular region of interest in pixels in sample space
            :rtype: Surface

            This function is called when a pattern is being rendered from. It
            should create a surface that provides the pixel data for the
            region of interest as defined by extents, though the surface
            itself does not have to be limited to that area. For convenience
            the surface should probably be of image type, created with
            :meth:`Surface.create_similar_image` for the target (which enables
            the number of copies to be reduced during transfer to the device).
            Another option, might be to return a similar surface to the target
            for explicit handling by the application of a set of cached
            sources on the device. The region of sample data provided should
            be defined using :meth:`Surface.set_device_offset` to specify the
            top-left corner of the sample data (along with width and height of
            the surface).

        .. function:: release(surface)

            :param Surface surface:
                the surface created during acquire

            This function is called when the pixel data is no longer being
            accessed by the pattern for the rendering operation.

        .. versionadded:: 1.15

    .. method:: get_acquire()

        :returns: a (acquire, release) tuple of callables or None as set
            through :meth:`set_acquire`

        Queries the current acquire and release callbacks.

        .. versionadded:: 1.15
