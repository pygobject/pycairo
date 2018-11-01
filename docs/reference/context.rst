.. _context:

*************
Cairo Context
*************

.. currentmodule:: cairo

.. comment block
   example reST:
   (add back '..' where required at column 0)
 . class:: module.C[(signature)]
   .. classmethod:: name(signature)
   .. staticmethod:: name(signature)
   .. method:: method(signature)

      :param p1: xxx
      :type p1: int
      :param p2: xxx
      :type p2: str
      :returns: xxx
      :rtype: list of strings
      :raises: xxx

      .. versionadded:: 1.6
   links:
     :data:`cairo.ANTIALIAS_SUBPIXEL`
     :class:`Context`
     :exc:`cairo.Error`
     :meth:`.copy_page`
     :meth:`Context.copy_page`


class Context()
===============

*Context* is the main object used when drawing with cairo. To draw with cairo,
you create a *Context*, set the target surface, and drawing options for the
*Context*, create shapes with functions like :meth:`Context.move_to` and
:meth:`Context.line_to`, and then draw shapes with :meth:`Context.stroke` or
:meth:`Context.fill`.

*Contexts* can be pushed to a stack via :meth:`Context.save`. They may then
safely be changed, without loosing the current state. Use
:meth:`Context.restore` to restore to the saved state.

.. class:: Context(target)

   :param target: target :class:`Surface` for the context
   :returns: a newly allocated *Context*
   :raises: :exc:`MemoryError` in case of no memory

   Creates a new *Context* with all graphics state parameters set to default
   values and with *target* as a target surface. The target surface should be
   constructed with a backend-specific function such as :class:`ImageSurface`
   (or any other cairo backend surface create variant).

   .. method:: append_path(path)

      :param path: :class:`Path` to be appended

      Append the *path* onto the current path. The *path* may be either the
      return value from one of :meth:`Context.copy_path` or
      :meth:`Context.copy_path_flat` or it may be constructed manually (in C).

   .. method:: arc(xc, yc, radius, angle1, angle2)

      :param xc: X position of the center of the arc
      :type xc: float
      :param yc: Y position of the center of the arc
      :type yc: float
      :param radius: the radius of the arc
      :type radius: float
      :param angle1: the start angle, in radians
      :type angle1: float
      :param angle2: the end angle, in radians
      :type angle2: float

      Adds a circular arc of the given *radius* to the current path.  The arc
      is centered at (*xc, yc*), begins at *angle1* and proceeds in the
      direction of increasing angles to end at *angle2*. If *angle2* is less
      than *angle1* it will be progressively increased by 2*PI until it is
      greater than *angle1*.

      If there is a current point, an initial line segment will be added to
      the path to connect the current point to the beginning of the arc. If
      this initial line is undesired, it can be avoided by calling
      :meth:`Context.new_sub_path` before calling :meth:`Context.arc`.

      Angles are measured in radians. An angle of 0.0 is in the direction of
      the positive X axis (in user space). An angle of PI/2.0 radians (90
      degrees) is in the direction of the positive Y axis (in user
      space). Angles increase in the direction from the positive X axis toward
      the positive Y axis. So with the default transformation matrix, angles
      increase in a clockwise direction.

      To convert from degrees to radians, use ``degrees * (math.pi / 180)``.

      This function gives the arc in the direction of increasing angles; see
      :meth:`Context.arc_negative` to get the arc in the direction of
      decreasing angles.

      The arc is circular in user space. To achieve an elliptical arc,
      you can scale the current transformation matrix by different
      amounts in the X and Y directions. For example, to draw an ellipse
      in the box given by *x, y, width, height*::

        ctx.save()
        ctx.translate(x + width / 2., y + height / 2.)
        ctx.scale(width / 2., height / 2.)
        ctx.arc(0., 0., 1., 0., 2 * math.pi)
        ctx.restore()


   .. method:: arc_negative(xc, yc, radius, angle1, angle2)

      :param xc: X position of the center of the arc
      :type xc: float
      :param yc: Y position of the center of the arc
      :type yc: float
      :param radius: the radius of the arc
      :type radius: float
      :param angle1: the start angle, in radians
      :type angle1: float
      :param angle2: the end angle, in radians
      :type angle2: float

      Adds a circular arc of the given *radius* to the current path. The arc
      is centered at (*xc, yc*), begins at *angle1* and proceeds in the
      direction of decreasing angles to end at *angle2*. If *angle2* is
      greater than *angle1* it will be progressively decreased by 2*PI until
      it is less than *angle1*.

      See :meth:`Context.arc` for more details. This function differs only in
      the direction of the arc between the two angles.

   .. method:: clip()

      Establishes a new clip region by intersecting the current clip region
      with the current path as it would be filled by :meth:`Context.fill` and
      according to the current :class:`fill rule <cairo.FillRule>` (see
      :meth:`Context.set_fill_rule`).

      After :meth:`.clip`, the current path will be cleared from the
      :class:`Context`.

      The current clip region affects all drawing operations by effectively
      masking out any changes to the surface that are outside the current clip
      region.

      Calling :meth:`.clip` can only make the clip region smaller, never
      larger. But the current clip is part of the graphics state, so a
      temporary restriction of the clip region can be achieved by calling
      :meth:`.clip` within a :meth:`Context.save`/:meth:`Context.restore`
      pair. The only other means of increasing the size of the clip region is
      :meth:`Context.reset_clip`.

   .. method:: clip_extents()

      :returns: (x1, y1, x2, y2), all float
      :rtype: tuple

      * *x1*: left of the resulting extents
      * *y1*: top of the resulting extents
      * *x2*: right of the resulting extents
      * *y2*: bottom of the resulting extents

      Computes a bounding box in user coordinates covering the area inside the
      current clip.

      .. versionadded:: 1.4

   .. method:: clip_preserve()

      Establishes a new clip region by intersecting the current clip region
      with the current path as it would be filled by :meth:`Context.fill` and
      according to the current :class:`fill rule <cairo.FillRule>` (see
      :meth:`Context.set_fill_rule`).

      Unlike :meth:`Context.clip`, :meth:`.clip_preserve` preserves the path
      within the :class:`Context`.

      The current clip region affects all drawing operations by effectively
      masking out any changes to the surface that are outside the current clip
      region.

      Calling :meth:`.clip_preserve` can only make the clip region smaller,
      never larger. But the current clip is part of the graphics state, so a
      temporary restriction of the clip region can be achieved by calling
      :meth:`.clip_preserve` within a
      :meth:`Context.save`/:meth:`Context.restore` pair. The only other means
      of increasing the size of the clip region is :meth:`Context.reset_clip`.

   .. method:: close_path()

      Adds a line segment to the path from the current point to the beginning
      of the current sub-path, (the most recent point passed to
      :meth:`Context.move_to`), and closes this sub-path. After this call the
      current point will be at the joined endpoint of the sub-path.

      The behavior of :meth:`.close_path` is distinct from simply calling
      :meth:`Context.line_to` with the equivalent coordinate in the case of
      stroking. When a closed sub-path is stroked, there are no caps on the
      ends of the sub-path. Instead, there is a line join connecting the final
      and initial segments of the sub-path.

      If there is no current point before the call to :meth:`.close_path`,
      this function will have no effect.

      Note: As of cairo version 1.2.4 any call to :meth:`.close_path` will
      place an explicit MOVE_TO element into the path immediately after the
      CLOSE_PATH element, (which can be seen in :meth:`Context.copy_path` for
      example). This can simplify path processing in some cases as it may not
      be necessary to save the "last move_to point" during processing as the
      MOVE_TO immediately after the CLOSE_PATH will provide that point.

   .. method:: copy_clip_rectangle_list()

      :returns: the current clip region as a list of rectangles in user
        coordinates. Returns a list of :class:`Rectangle`
      :rtype: list

      .. versionadded:: 1.4

   .. method:: copy_page()

      Emits the current page for backends that support multiple pages, but
      doesn't clear it, so, the contents of the current page will be retained
      for the next page too.  Use :meth:`Context.show_page` if you want to get
      an empty page after the emission.

      This is a convenience function that simply calls
      :meth:`Surface.copy_page` on *Context's* target.

   .. method:: copy_path()

      :returns: :class:`Path`
      :raises: :exc:`MemoryError` in case of no memory

      Creates a copy of the current path and returns it to the user as a
      :class:`Path`.

   .. method:: copy_path_flat()

      :returns: :class:`Path`
      :raises: :exc:`MemoryError` in case of no memory

      Gets a flattened copy of the current path and returns it to the
      user as a :class:`Path`.

      This function is like :meth:`Context.copy_path` except that any curves
      in the path will be approximated with piecewise-linear approximations,
      (accurate to within the current tolerance value). That is, the result is
      guaranteed to not have any elements of type CAIRO_PATH_CURVE_TO which
      will instead be replaced by a series of CAIRO_PATH_LINE_TO elements.

   .. method:: curve_to(x1, y1, x2, y2, x3, y3)

      :param x1: the X coordinate of the first control point
      :type x1: float
      :param y1: the Y coordinate of the first control point
      :type y1: float
      :param x2: the X coordinate of the second control point
      :type x2: float
      :param y2: the Y coordinate of the second control point
      :type y2: float
      :param x3: the X coordinate of the end of the curve
      :type x3: float
      :param y3: the Y coordinate of the end of the curve
      :type y3: float

      Adds a cubic Bézier spline to the path from the current point to
      position *(x3, y3)* in user-space coordinates, using *(x1, y1)* and
      *(x2, y2)* as the control points. After this call the current point will
      be *(x3, y3)*.

      If there is no current point before the call to :meth:`.curve_to`
      this function will behave as if preceded by a call to
      ``ctx.move_to(x1, y1)``.

   .. method:: device_to_user(x, y)

      :param x: X value of coordinate
      :type x: float
      :param y: Y value of coordinate
      :type y: float
      :returns: (x, y), both float
      :rtype: tuple

      Transform a coordinate from device space to user space by multiplying
      the given point by the inverse of the current transformation matrix
      (CTM).

   .. method:: device_to_user_distance(dx, dy)

      :param dx: X component of a distance vector
      :type dx: float
      :param dy: Y component of a distance vector
      :type dy: float
      :returns: (dx, dy), both float
      :rtype: tuple

      Transform a distance vector from device space to user space. This
      function is similar to :meth:`Context.device_to_user` except that the
      translation components of the inverse CTM will be ignored when
      transforming *(dx,dy)*.

   .. method:: fill()

      A drawing operator that fills the current path according to the current
      :class:`fill rule <cairo.FillRule>`, (each sub-path is implicitly
      closed before being filled). After :meth:`.fill`, the current path will
      be cleared from the :class:`Context`. See :meth:`Context.set_fill_rule`
      and :meth:`Context.fill_preserve`.

   .. method:: fill_extents()

      :returns: (x1, y1, x2, y2), all float
      :rtype: tuple

      * *x1*: left of the resulting extents
      * *y1*: top of the resulting extents
      * *x2*: right of the resulting extents
      * *y2*: bottom of the resulting extents

      Computes a bounding box in user coordinates covering the area that would
      be affected, (the "inked" area), by a :meth:`Context.fill` operation
      given the current path and fill parameters. If the current path is
      empty, returns an empty rectangle (0,0,0,0). Surface dimensions and
      clipping are not taken into account.

      Contrast with :meth:`Context.path_extents`, which is similar, but returns
      non-zero extents for some paths with no inked area, (such as a
      simple line segment).

      Note that :meth:`.fill_extents` must necessarily do more work to compute
      the precise inked areas in light of the fill rule, so
      :meth:`Context.path_extents` may be more desirable for sake of
      performance if the non-inked path extents are desired.

      See :meth:`Context.fill`, :meth:`Context.set_fill_rule` and
      :meth:`Context.fill_preserve`.

   .. method:: fill_preserve()

      A drawing operator that fills the current path according to the current
      :class:`fill rule <cairo.FillRule>`, (each sub-path is implicitly
      closed before being filled). Unlike :meth:`Context.fill`,
      :meth:`.fill_preserve` preserves the path within the :class:`Context`.

      See :meth:`Context.set_fill_rule` and :meth:`Context.fill`.

   .. method:: font_extents()

      :returns: (ascent, descent, height, max_x_advance, max_y_advance),
        all float
      :rtype: tuple

      Gets the font extents for the currently selected font.

   .. method:: get_antialias()

      :returns: the current antialias mode,
        as set by :meth:`Context.set_antialias`.
      :rtype: cairo.Antialias

   .. method:: get_current_point()

      :returns: (x, y), both float
      :rtype: tuple

      * *x*: X coordinate of the current point
      * *y*: Y coordinate of the current point

      Gets the current point of the current path, which is conceptually the
      final point reached by the path so far.

      The current point is returned in the user-space coordinate system. If
      there is no defined current point or if :class:`Context` is in an error
      status, *x* and *y* will both be set to 0.0. It is possible to check this
      in advance with :meth:`Context.has_current_point`.

      Most path construction functions alter the current point. See the
      following for details on how they affect the current point:
      :meth:`Context.new_path`, :meth:`Context.new_sub_path`,
      :meth:`Context.append_path`, :meth:`Context.close_path`,
      :meth:`Context.move_to`, :meth:`Context.line_to`,
      :meth:`Context.curve_to`, :meth:`Context.rel_move_to`,
      :meth:`Context.rel_line_to`, :meth:`Context.rel_curve_to`,
      :meth:`Context.arc`, :meth:`Context.arc_negative`,
      :meth:`Context.rectangle`, :meth:`Context.text_path`,
      :meth:`Context.glyph_path`, :meth:`Context.stroke_to_path`.

      Some functions use and alter the current point but do not otherwise
      change current path:
      :meth:`Context.show_text`.

      Some functions unset the current path and as a result, current point:
      :meth:`Context.fill`, :meth:`Context.stroke`.

   .. method:: get_dash()

      :returns: (dashes, offset)
      :rtype: tuple

      * *dashes*: return value as a tuple for the dash array
      * *offset*: return value as float for the current dash offset

      Gets the current dash array.

      .. versionadded:: 1.4

   .. method:: get_dash_count()

      :returns: the length of the dash array, or 0 if no dash array set.
      :rtype: int

      See also :meth:`Context.set_dash` and :meth:`Context.get_dash`.

      .. versionadded:: 1.4

   .. method:: get_fill_rule()

      :returns: the current fill rule, as
        set by :meth:`Context.set_fill_rule`.
      :rtype: cairo.FillRule

   .. method:: get_font_face()

      :returns: the current :class:`FontFace` for the :class:`Context`.

   .. method:: get_font_matrix()

      :returns: the current :class:`Matrix` for the :class:`Context`.

      See :meth:`Context.set_font_matrix`.

   .. method:: get_font_options()

      :returns: the current :class:`FontOptions` for the :class:`Context`.

      Retrieves font rendering options set via
      :meth:`Context.set_font_options`. Note that the returned options do not
      include any options derived from the underlying surface; they are
      literally the options passed to :meth:`Context.set_font_options`.

   .. method:: get_group_target()

      :returns: the target :class:`Surface`.

      Gets the current destination :class:`Surface` for the
      :class:`Context`. This is either the original target surface as passed
      to :class:`Context` or the target surface for the current group as
      started by the most recent call to :meth:`Context.push_group` or
      :meth:`Context.push_group_with_content`.

      .. versionadded:: 1.2

   .. method:: get_line_cap()

      :returns: the current line cap style, as
        set by :meth:`Context.set_line_cap`.
      :rtype: cairo.LineCap

   .. method:: get_line_join()

      :returns: the current line join style, as
        set by :meth:`Context.set_line_join`.
      :rtype: cairo.LineJoin

   .. method:: get_line_width()

      :returns: the current line width
      :rtype: float

      This function returns the current line width value exactly as set by
      :meth:`Context.set_line_width`. Note that the value is unchanged even if
      the CTM has changed between the calls to :meth:`Context.set_line_width`
      and :meth:`.get_line_width`.

   .. method:: get_matrix()

      :returns: the current transformation :class:`Matrix` (CTM)

   .. method:: get_miter_limit()

      :returns: the current miter limit, as set by
        :meth:`Context.set_miter_limit`.
      :rtype: float

   .. method:: get_operator()

      :returns: the current compositing operator
        for a :class:`Context`.
      :rtype: cairo.Operator

   .. method:: get_scaled_font()

      :returns: the current :class:`ScaledFont` for a :class:`Context`.

      .. versionadded:: 1.4

   .. method:: get_source()

      :returns: the current source :class:`Pattern` for  a :class:`Context`.

   .. method:: get_target()

      :returns: the target :class:`Surface` for the :class:`Context`

   .. method:: get_tolerance()

      :returns: the current tolerance value, as set by
        :meth:`Context.set_tolerance`
      :rtype: float

   .. method:: glyph_extents(glyphs)

      :param glyphs: glyphs, a sequence of :class:`Glyph`
      :rtype: TextExtents

      Gets the extents for an array of glyphs. The extents describe a
      user-space rectangle that encloses the "inked" portion of the glyphs,
      (as they would be drawn by :meth:`Context.show_glyphs`). Additionally,
      the x_advance and y_advance values indicate the amount by which the
      current point would be advanced by :meth:`Context.show_glyphs`.

      Note that whitespace glyphs do not contribute to the size of the
      rectangle (extents.width and extents.height).

   .. method:: glyph_path(glyphs)

      :param glyphs: glyphs to show, a sequence of :class:`Glyph`

      Adds closed paths for the glyphs to the current path. The generated path
      if filled, achieves an effect similar to that of
      :meth:`Context.show_glyphs`.

   .. method:: has_current_point()

      returns: True iff a current point is defined on the current path.
        See :meth:`Context.get_current_point` for details on the current point.

      .. versionadded:: 1.6

   .. method:: identity_matrix()

      Resets the current transformation :class:`Matrix` (CTM) by setting it
      equal to the identity matrix. That is, the user-space and device-space
      axes will be aligned and one user-space unit will transform to one
      device-space unit.

   .. method:: in_fill(x, y)

      :param x: X coordinate of the point to test
      :type x: float
      :param y: Y coordinate of the point to test
      :type y: float
      :returns: True iff the point is inside the area that would be affected
        by a :meth:`Context.fill` operation given the current path and filling
        parameters. Surface dimensions and clipping are not taken into account.

      See :meth:`Context.fill`, :meth:`Context.set_fill_rule` and
      :meth:`Context.fill_preserve`.

   .. method:: in_stroke(x, y)

      :param x: X coordinate of the point to test
      :type x: float
      :param y: Y coordinate of the point to test
      :type y: float

      :returns: True iff the point is inside the area that would be affected
        by a :meth:`Context.stroke` operation given the current path and
        stroking parameters. Surface dimensions and clipping are not taken
        into account.

      See :meth:`Context.stroke`, :meth:`Context.set_line_width`,
      :meth:`Context.set_line_join`, :meth:`Context.set_line_cap`,
      :meth:`Context.set_dash`, and :meth:`Context.stroke_preserve`.

   .. method:: line_to(x, y)

      :param x: the X coordinate of the end of the new line
      :type x: float
      :param y: the Y coordinate of the end of the new line
      :type y: float

      Adds a line to the path from the current point to position *(x, y)* in
      user-space coordinates. After this call the current point will be *(x,
      y)*.

      If there is no current point before the call to :meth:`.line_to`
      this function will behave as ``ctx.move_to(x, y)``.

   .. method:: mask(pattern)

      :param pattern: a :class:`Pattern`

      A drawing operator that paints the current source using the alpha
      channel of *pattern* as a mask. (Opaque areas of *pattern* are painted
      with the source, transparent areas are not painted.)

   .. method:: mask_surface(surface, x=0.0, y=0.0)

      :param surface: a :class:`Surface`
      :param x: X coordinate at which to place the origin of *surface*
      :type x: float
      :param y: Y coordinate at which to place the origin of *surface*
      :type y: float

      A drawing operator that paints the current source using the alpha
      channel of *surface* as a mask. (Opaque areas of *surface* are painted
      with the source, transparent areas are not painted.)

   .. method:: move_to(x, y)

      :param x: the X coordinate of the new position
      :type x: float
      :param y: the Y coordinate of the new position
      :type y: float

      Begin a new sub-path. After this call the current point will be *(x,
      y)*.

   .. method:: new_path()

      Clears the current path. After this call there will be no path and no
      current point.

   .. method:: new_sub_path()

      Begin a new sub-path. Note that the existing path is not affected. After
      this call there will be no current point.

      In many cases, this call is not needed since new sub-paths are
      frequently started with :meth:`Context.move_to`.

      A call to :meth:`.new_sub_path` is particularly useful when beginning a
      new sub-path with one of the :meth:`Context.arc` calls. This makes
      things easier as it is no longer necessary to manually compute the arc's
      initial coordinates for a call to :meth:`Context.move_to`.

      .. versionadded:: 1.6

   .. method:: paint()

      A drawing operator that paints the current source everywhere within the
      current clip region.

   .. method:: paint_with_alpha(alpha)

      :param alpha: alpha value, between 0 (transparent) and 1 (opaque)
      :type alpha: float

      A drawing operator that paints the current source everywhere within the
      current clip region using a mask of constant alpha value *alpha*. The
      effect is similar to :meth:`Context.paint`, but the drawing is faded out
      using the alpha value.

   .. method:: path_extents()

      :returns: (x1, y1, x2, y2), all float
      :rtype: tuple

      * *x1*: left of the resulting extents
      * *y1*: top of the resulting extents
      * *x2*: right of the resulting extents
      * *y2*: bottom of the resulting extents

      Computes a bounding box in user-space coordinates covering the points on
      the current path. If the current path is empty, returns an empty
      rectangle (0, 0, 0, 0). Stroke parameters, fill rule, surface
      dimensions and clipping are not taken into account.

      Contrast with :meth:`Context.fill_extents` and
      :meth:`Context.stroke_extents` which return the extents of only the area
      that would be "inked" by the corresponding drawing operations.

      The result of :meth:`.path_extents` is defined as equivalent to the
      limit of :meth:`Context.stroke_extents` with cairo.LINE_CAP_ROUND as the
      line width approaches 0.0, (but never reaching the empty-rectangle
      returned by :meth:`Context.stroke_extents` for a line width of 0.0).

      Specifically, this means that zero-area sub-paths such as
      :meth:`Context.move_to`; :meth:`Context.line_to` segments, (even
      degenerate cases where the coordinates to both calls are identical),
      will be considered as contributing to the extents. However, a lone
      :meth:`Context.move_to` will not contribute to the results of
      :meth:`Context.path_extents`.

      .. versionadded:: 1.6

   .. method:: pop_group()

      :returns: a newly created :class:`SurfacePattern` containing the results
        of all drawing operations performed to the group.

      Terminates the redirection begun by a call to :meth:`Context.push_group`
      or :meth:`Context.push_group_with_content` and returns a new pattern
      containing the results of all drawing operations performed to the group.

      The :meth:`.pop_group` function calls :meth:`Context.restore`,
      (balancing a call to :meth:`Context.save` by the
      :meth:`Context.push_group` function), so that any changes to the graphics
      state will not be visible outside the group.

      .. versionadded:: 1.2

   .. method:: pop_group_to_source()

      Terminates the redirection begun by a call to :meth:`Context.push_group`
      or :meth:`Context.push_group_with_content` and installs the resulting
      pattern as the source :class:`Pattern` in the given :class:`Context`.

      The behavior of this function is equivalent to the sequence of
      operations::

        group = cairo_pop_group()
        ctx.set_source(group)

      but is more convenient as their is no need for a variable to store
      the short-lived pointer to the pattern.

      The :meth:`Context.pop_group` function calls :meth:`Context.restore`,
      (balancing a call to :meth:`Context.save` by the
      :meth:`Context.push_group` function), so that any changes to the graphics
      state will not be visible outside the group.

      .. versionadded:: 1.2

   .. method:: push_group()

      Temporarily redirects drawing to an intermediate surface known as a
      group. The redirection lasts until the group is completed by a call to
      :meth:`Context.pop_group` or :meth:`Context.pop_group_to_source`. These
      calls provide the result of any drawing to the group as a pattern,
      (either as an explicit object, or set as the source pattern).

      This group functionality can be convenient for performing intermediate
      compositing. One common use of a group is to render objects as opaque
      within the group, (so that they occlude each other), and then blend the
      result with translucence onto the destination.

      Groups can be nested arbitrarily deep by making balanced calls to
      :meth:`Context.push_group`/:meth:`Context.pop_group`. Each call
      pushes/pops the new target group onto/from a stack.

      The :meth:`.push_group` function calls :meth:`Context.save` so that any
      changes to the graphics state will not be visible outside the group,
      (the pop_group functions call :meth:`Context.restore`).

      By default the intermediate group will have a :class:`cairo.Content`
      type of :attr:`cairo.Content.COLOR_ALPHA`. Other content types can be
      chosen for the group by using :meth:`Context.push_group_with_content`
      instead.

      As an example, here is how one might fill and stroke a path with
      translucence, but without any portion of the fill being visible
      under the stroke::

        ctx.push_group()
        ctx.set_source(fill_pattern)
        ctx.fill_preserve()
        ctx.set_source(stroke_pattern)
        ctx.stroke()
        ctx.pop_group_to_source()
        ctx.paint_with_alpha(alpha)

      .. versionadded:: 1.2

   .. method:: push_group_with_content(content)

      :param cairo.Content content: a content indicating the
        type of group that will be created

      Temporarily redirects drawing to an intermediate surface known as a
      group. The redirection lasts until the group is completed by a call to
      :meth:`Context.pop_group` or :meth:`Context.pop_group_to_source`. These
      calls provide the result of any drawing to the group as a pattern,
      (either as an explicit object, or set as the source pattern).

      The group will have a content type of *content*. The ability to control
      this content type is the only distinction between this function and
      :meth:`Context.push_group` which you should see for a more detailed
      description of group rendering.

      .. versionadded:: 1.2

   .. method:: rectangle(x, y, width, height)

      :param x: the X coordinate of the top left corner of the rectangle
      :type x: float
      :param y: the Y coordinate to the top left corner of the rectangle
      :type y: float
      :param width: the width of the rectangle
      :type width: float
      :param height: the height of the rectangle
      :type height: float

      Adds a closed sub-path rectangle of the given size to the current path
      at position *(x, y)* in user-space coordinates.

      This function is logically equivalent to::

        ctx.move_to(x, y)
        ctx.rel_line_to(width, 0)
        ctx.rel_line_to(0, height)
        ctx.rel_line_to(-width, 0)
        ctx.close_path()

   .. method:: rel_curve_to(dx1, dy1, dx2, dy2, dx3, dy4)

      :param dx1: the X offset to the first control point
      :type dx1: float
      :param dy1: the Y offset to the first control point
      :type dy1: float
      :param dx2: the X offset to the second control point
      :type dx2: float
      :param dy2: the Y offset to the second control point
      :type dy2: float
      :param dx3: the X offset to the end of the curve
      :type dx3: float
      :param dy3: the Y offset to the end of the curve
      :type dy3: float
      :raises: :exc:`cairo.Error` if called with no current point.

      Relative-coordinate version of :meth:`Context.curve_to`. All
      offsets are relative to the current point. Adds a cubic Bézier spline to
      the path from the current point to a point offset from the current point
      by *(dx3, dy3)*, using points offset by *(dx1, dy1)* and *(dx2, dy2)* as
      the control points. After this call the current point will be offset by
      *(dx3, dy3)*.

      Given a current point of (x, y), ``ctx.rel_curve_to(dx1, dy1, dx2, dy2,
      dx3, dy3)`` is logically equivalent to ``ctx.curve_to(x+dx1, y+dy1,
      x+dx2, y+dy2, x+dx3, y+dy3)``.

   .. method:: rel_line_to(dx, dy)

      :param dx: the X offset to the end of the new line
      :type dx: float
      :param dy: the Y offset to the end of the new line
      :type dy: float
      :raises: :exc:`cairo.Error` if called with no current point.

      Relative-coordinate version of :meth:`Context.line_to`. Adds a line to
      the path from the current point to a point that is offset from the
      current point by *(dx, dy)* in user space. After this call the current
      point will be offset by *(dx, dy)*.

      Given a current point of (x, y), ``ctx.rel_line_to(dx, dy)`` is logically
      equivalent to ``ctx.line_to(x + dx, y + dy)``.

   .. method:: rel_move_to(dx, dy)

      :param dx: the X offset
      :type dx: float
      :param dy: the Y offset
      :type dy: float
      :raises: :exc:`cairo.Error` if called with no current point.

      Begin a new sub-path. After this call the current point will offset by
      *(dx, dy)*.

      Given a current point of (x, y), ``ctx.rel_move_to(dx, dy)`` is logically
      equivalent to ``ctx.(x + dx, y + dy)``.

   .. method:: reset_clip()

      Reset the current clip region to its original, unrestricted state. That
      is, set the clip region to an infinitely large shape containing the
      target surface. Equivalently, if infinity is too hard to grasp, one can
      imagine the clip region being reset to the exact bounds of the target
      surface.

      Note that code meant to be reusable should not call :meth:`.reset_clip`
      as it will cause results unexpected by higher-level code which calls
      :meth:`.clip`. Consider using :meth:`.save` and :meth:`.restore` around
      :meth:`.clip` as a more robust means of temporarily restricting the clip
      region.

   .. method:: restore()

      Restores :class:`Context` to the state saved by a preceding call to
      :meth:`.save` and removes that state from the stack of saved states.

   .. method:: rotate(angle)

      :param angle: angle (in radians) by which the user-space axes will be
        rotated
      :type angle: float

      Modifies the current transformation matrix (CTM) by rotating the
      user-space axes by *angle* radians. The rotation of the axes takes places
      after any existing transformation of user space. The rotation direction
      for positive angles is from the positive X axis toward the positive Y
      axis.

   .. method:: save()

      Makes a copy of the current state of :class:`Context` and saves it on an
      internal stack of saved states. When :meth:`.restore` is called,
      :class:`Context` will be restored to the saved state. Multiple calls to
      :meth:`.save` and :meth:`.restore` can be nested; each call to
      :meth:`.restore` restores the state from the matching paired
      :meth:`.save`.

   .. method:: scale(sx, sy)

      :param sx: scale factor for the X dimension
      :type sx: float
      :param sy: scale factor for the Y dimension
      :type sy: float

      Modifies the current transformation matrix (CTM) by scaling the X and Y
      user-space axes by *sx* and *sy* respectively. The scaling of the axes
      takes place after any existing transformation of user space.

   .. method:: select_font_face(family[, slant[, weight]])

      :param family: a font family name
      :type family: text
      :param cairo.FontSlant slant: the font slant of the font,
        defaults to :attr:`cairo.FontSlant.NORMAL`.
      :param cairo.FontWeight weight: the font weight of the
        font, defaults to :attr:`cairo.FontWeight.NORMAL`.

      Note: The :meth:`.select_font_face` function call is part of what the
      cairo designers call the "toy" text API. It is convenient for short
      demos and simple programs, but it is not expected to be adequate for
      serious text-using applications.

      Selects a family and style of font from a simplified description as a
      family name, slant and weight. Cairo provides no operation to list
      available family names on the system (this is a "toy", remember), but
      the standard CSS2 generic family names, ("serif", "sans-serif",
      "cursive", "fantasy", "monospace"), are likely to work as expected.

      For "real" font selection, see the font-backend-specific
      font_face_create functions for the font backend you are using. (For
      example, if you are using the freetype-based cairo-ft font backend, see
      cairo_ft_font_face_create_for_ft_face() or
      cairo_ft_font_face_create_for_pattern().) The resulting font face could
      then be used with cairo_scaled_font_create() and
      cairo_set_scaled_font().

      Similarly, when using the "real" font support, you can call directly
      into the underlying font system, (such as fontconfig or freetype), for
      operations such as listing available fonts, etc.

      It is expected that most applications will need to use a more
      comprehensive font handling and text layout library, (for example,
      pango), in conjunction with cairo.

      If text is drawn without a call to :meth:`.select_font_face`, (nor
      :meth:`.set_font_face` nor :meth:`.set_scaled_font`), the default family
      is platform-specific, but is essentially "sans-serif".  Default slant is
      :attr:`cairo.FontSlant.NORMAL`, and default weight is
      :attr:`cairo.FontWeight.NORMAL`.

      This function is equivalent to a call to :class:`ToyFontFace`
      followed by :meth:`.set_font_face`.

   .. method:: set_antialias(antialias)

      :param cairo.Antialias antialias: the new antialias mode

      Set the antialiasing mode of the rasterizer used for drawing shapes.
      This value is a hint, and a particular backend may or may not support a
      particular value.  At the current time, no backend supports
      :attr:`cairo.Antialias.SUBPIXEL` when drawing shapes.

      Note that this option does not affect text rendering, instead see
      :meth:`FontOptions.set_antialias`.

   .. method:: set_dash(dashes, [offset=0])

      :param dashes: a sequence specifying alternate lengths of on and off
        stroke portions as float.
      :param offset: an offset into the dash pattern at which the stroke
        should start, defaults to 0.
      :type offset: int
      :raises: :exc:`cairo.Error` if any value in *dashes* is negative, or if
        all values are 0.

      Sets the dash pattern to be used by :meth:`.stroke`. A dash pattern is
      specified by *dashes* - a sequence of positive values. Each value
      provides the length of alternate "on" and "off" portions of the
      stroke. The *offset* specifies an offset into the pattern at which the
      stroke begins.

      Each "on" segment will have caps applied as if the segment were a
      separate sub-path. In particular, it is valid to use an "on" length of
      0.0 with :attr:`cairo.LineCap.ROUND` or :attr:`cairo.LineCap.SQUARE`
      in order to distributed dots or squares along a path.

      Note: The length values are in user-space units as evaluated at the time
      of stroking. This is not necessarily the same as the user space at the
      time of :meth:`.set_dash`.

      If the number of dashes is 0 dashing is disabled.

      If the number of dashes is 1 a symmetric pattern is assumed with
      alternating on and off portions of the size specified by the single
      value in *dashes*.

   .. method:: set_fill_rule(fill_rule)

      :param cairo.FillRule fill_rule: a fill rule to set the
        within the cairo context. The fill rule is used to determine which
        regions are inside or outside a complex (potentially
        self-intersecting) path. The current fill rule affects both
        :meth:`.fill` and :meth:`.clip`.

      The default fill rule is :attr:`cairo.FillRule.WINDING`.

   .. method:: set_font_face(font_face)

      :param font_face: a :class:`FontFace`, or None to restore to the
        default :class:`FontFace`

      Replaces the current :class:`FontFace` object in the :class:`Context`
      with *font_face*.

   .. method:: set_font_matrix(matrix)

      :param matrix: a :class:`Matrix` describing a transform to be applied to
        the current font.

      Sets the current font matrix to *matrix*. The font matrix gives a
      transformation from the design space of the font (in this space, the
      em-square is 1 unit by 1 unit) to user space. Normally, a simple scale
      is used (see :meth:`.set_font_size`), but a more complex font matrix can
      be used to shear the font or stretch it unequally along the two axes

   .. method:: set_font_options(options)

      :param options: :class:`FontOptions` to use

      Sets a set of custom font rendering options for the :class:`Context`.
      Rendering options are derived by merging these options with the options
      derived from underlying surface; if the value in *options* has a default
      value (like :attr:`cairo.Antialias.DEFAULT`), then the value from the
      surface is used.

   .. method:: set_font_size(size)

      :param size: the new font size, in user space units
      :type size: float

      Sets the current font matrix to a scale by a factor of *size*, replacing
      any font matrix previously set with :meth:`.set_font_size` or
      :meth:`.set_font_matrix`. This results in a font size of *size* user
      space units. (More precisely, this matrix will result in the font's
      em-square being a *size* by *size* square in user space.)

      If text is drawn without a call to :meth:`.set_font_size`, (nor
      :meth:`.set_font_matrix` nor :meth:`.set_scaled_font`), the default font
      size is 10.0.

   .. method:: set_line_cap(line_cap)

      :param cairo.LineCap line_cap: a line cap style

      Sets the current line cap style within the :class:`Context`.

      As with the other stroke parameters, the current line cap style is
      examined by :meth:`.stroke`, :meth:`.stroke_extents`, and
      :meth:`.stroke_to_path`, but does not have any effect during path
      construction.

      The default line cap style is :attr:`cairo.LineCap.BUTT`.

   .. method:: set_line_join(line_join)

      :param cairo.LineJoin line_join: a line join style

      Sets the current line join style within the :class:`Context`.

      As with the other stroke parameters, the current line join style is
      examined by :meth:`.stroke`, :meth:`.stroke_extents`, and
      :meth:`.stroke_to_path`, but does not have any effect during path
      construction.

      The default line join style is :attr:`cairo.LineJoin.MITER`.

   .. method:: set_line_width(width)

      :param width: a line width
      :type width: float

      Sets the current line width within the :class:`Context`. The line width
      value specifies the diameter of a pen that is circular in user space,
      (though device-space pen may be an ellipse in general due to
      scaling/shear/rotation of the CTM).

      Note: When the description above refers to user space and CTM it refers
      to the user space and CTM in effect at the time of the stroking
      operation, not the user space and CTM in effect at the time of the call
      to :meth:`.set_line_width`. The simplest usage makes both of these
      spaces identical. That is, if there is no change to the CTM between a
      call to :meth:`.set_line_width` and the stroking operation, then one can
      just pass user-space values to :meth:`.set_line_width` and ignore this
      note.

      As with the other stroke parameters, the current line width is examined
      by :meth:`.stroke`, :meth:`.stroke_extents`, and
      :meth:`.stroke_to_path`, but does not have any effect during path
      construction.

      The default line width value is 2.0.

   .. method:: set_matrix(matrix)

      :param matrix: a transformation :class:`Matrix` from user space to
        device space.

      Modifies the current transformation matrix (CTM) by setting it equal to
      *matrix*.

   .. method:: set_miter_limit(limit)

      :param limit: miter limit to set
      :type width: float

      Sets the current miter limit within the :class:`Context`.

      If the current line join style is set to :attr:`cairo.LineJoin.MITER`
      (see :meth:`.set_line_join`), the miter limit is used to determine
      whether the lines should be joined with a bevel instead of a miter.
      Cairo divides the length of the miter by the line width. If the result
      is greater than the miter limit, the style is converted to a bevel.

      As with the other stroke parameters, the current line miter limit is
      examined by :meth:`.stroke`, :meth:`.stroke_extents`, and
      :meth:`.stroke_to_path`, but does not have any effect during path
      construction.

      The default miter limit value is 10.0, which will convert joins with
      interior angles less than 11 degrees to bevels instead of miters. For
      reference, a miter limit of 2.0 makes the miter cutoff at 60 degrees,
      and a miter limit of 1.414 makes the cutoff at 90 degrees.

      A miter limit for a desired angle can be computed as::

        miter limit = 1/math.sin(angle/2)

   .. method:: set_operator(op)

      :param cairo.Operator op: the compositing operator to set
        for use in all drawing operations.

      The default operator is :attr:`cairo.Operator.OVER`.

   .. method:: set_scaled_font(scaled_font)

      :param scaled_font: a :class:`ScaledFont`

      Replaces the current font face, font matrix, and font options in the
      :class:`Context` with those of the :class:`ScaledFont`. Except for some
      translation, the current CTM of the :class:`Context` should be the same
      as that of the :class:`ScaledFont`, which can be accessed using
      :meth:`ScaledFont.get_ctm`.

      .. versionadded:: 1.2

   .. method:: set_source(source)

      :param source: a :class:`Pattern` to be used as the source for
        subsequent drawing operations.

      Sets the source pattern within :class:`Context` to *source*. This
      pattern will then be used for any subsequent drawing operation until a
      new source pattern is set.

      Note: The pattern's transformation matrix will be locked to the user
      space in effect at the time of :meth:`.set_source`. This means that
      further modifications of the current transformation matrix will not
      affect the source pattern. See :meth:`Pattern.set_matrix`.

      The default source pattern is a solid pattern that is opaque black,
      (that is, it is equivalent to ``set_source_rgb(0.0, 0.0, 0.0)``.

   .. method:: set_source_rgb(red, green, blue)

      :param red: red component of color
      :type red: float
      :param green: green component of color
      :type green: float
      :param blue: blue component of color
      :type blue: float

      Sets the source pattern within :class:`Context` to an opaque color. This
      opaque color will then be used for any subsequent drawing operation
      until a new source pattern is set.

      The color components are floating point numbers in the range 0 to
      1. If the values passed in are outside that range, they will be
      clamped.

      The default source pattern is opaque black, (that is, it is
      equivalent to ``set_source_rgb(0.0, 0.0, 0.0)``.

   .. method:: set_source_rgba(red, green, blue[, alpha=1.0])

      :param red: red component of color
      :type red: float
      :param green: green component of color
      :type green: float
      :param blue: blue component of color
      :type blue: float
      :param alpha: alpha component of color
      :type alpha: float

      Sets the source pattern within :class:`Context` to a translucent
      color. This color will then be used for any subsequent drawing operation
      until a new source pattern is set.

      The color and alpha components are floating point numbers in the range 0
      to 1. If the values passed in are outside that range, they will be
      clamped.

      The default source pattern is opaque black, (that is, it is
      equivalent to ``set_source_rgba(0.0, 0.0, 0.0, 1.0)``.

   .. method:: set_source_surface(surface[, x=0.0[, y=0.0]])

      :param surface: a :class:`Surface` to be used to set the source pattern
      :param x: User-space X coordinate for surface origin
      :type x: float
      :param y: User-space Y coordinate for surface origin
      :type y: float

      This is a convenience function for creating a pattern from a
      :class:`Surface` and setting it as the source in :class:`Context` with
      :meth:`.set_source`.

      The *x* and *y* parameters give the user-space coordinate at which the
      surface origin should appear. (The surface origin is its upper-left
      corner before any transformation has been applied.) The *x* and *y*
      patterns are negated and then set as translation values in the pattern
      matrix.

      Other than the initial translation pattern matrix, as described above,
      all other pattern attributes, (such as its extend mode), are set to the
      default values as in :class:`SurfacePattern`.  The resulting pattern can
      be queried with :meth:`.get_source` so that these attributes can be
      modified if desired, (eg. to create a repeating pattern with
      :meth:`.Pattern.set_extend`).

   .. method:: set_tolerance(tolerance)

      :param tolerance: the tolerance, in device units (typically pixels)
      :type tolerance: float

      Sets the tolerance used when converting paths into trapezoids.  Curved
      segments of the path will be subdivided until the maximum deviation
      between the original path and the polygonal approximation is less than
      *tolerance*. The default value is 0.1. A larger value will give better
      performance, a smaller value, better appearance. (Reducing the value
      from the default value of 0.1 is unlikely to improve appearance
      significantly.)  The accuracy of paths within Cairo is limited by the
      precision of its internal arithmetic, and the prescribed *tolerance* is
      restricted to the smallest representable internal value.

   .. method:: show_glyphs(glyphs)

      :param glyphs: glyphs to show as a sequence of :class:`Glyph`

      A drawing operator that generates the shape from an array of glyphs,
      rendered according to the current font face, font size (font matrix),
      and font options.

   .. method:: show_page()

      Emits and clears the current page for backends that support multiple
      pages. Use :meth:`.copy_page` if you don't want to clear the page.

      This is a convenience function that simply calls
      ``ctx.get_target() . show_page()``

   .. method:: show_text(text)

      :param text: text
      :type text: text

      A drawing operator that generates the shape from a string of text,
      rendered according to the current font_face, font_size (font_matrix),
      and font_options.

      This function first computes a set of glyphs for the string of text. The
      first glyph is placed so that its origin is at the current point. The
      origin of each subsequent glyph is offset from that of the previous
      glyph by the advance values of the previous glyph.

      After this call the current point is moved to the origin of where the
      next glyph would be placed in this same progression. That is, the
      current point will be at the origin of the final glyph offset by its
      advance values. This allows for easy display of a single logical string
      with multiple calls to :meth:`.show_text`.

      Note: The :meth:`.show_text` function call is part of what the cairo
      designers call the "toy" text API. It is convenient for short demos
      and simple programs, but it is not expected to be adequate for
      serious text-using applications. See :meth:`.show_glyphs` for the
      "real" text display API in cairo.

   .. method:: stroke()

      A drawing operator that strokes the current path according to the
      current line width, line join, line cap, and dash settings. After
      :meth:`.stroke`, the current path will be cleared from the cairo
      context. See :meth:`.set_line_width`, :meth:`.set_line_join`,
      :meth:`.set_line_cap`, :meth:`.set_dash`, and :meth:`.stroke_preserve`.

      Note: Degenerate segments and sub-paths are treated specially and
      provide a useful result. These can result in two different situations:

      1. Zero-length "on" segments set in :meth:`.set_dash`. If the cap
      style is :attr:`cairo.LineCap.ROUND` or :attr:`cairo.LineCap.SQUARE`
      then these segments will be drawn as circular dots or squares
      respectively. In the case of :attr:`cairo.LineCap.SQUARE`, the
      orientation of the squares is determined by the direction of the
      underlying path.

      2. A sub-path created by :meth:`.move_to` followed by either a
      :meth:`.close_path` or one or more calls to :meth:`.line_to` to the same
      coordinate as the :meth:`.move_to`. If the cap style is
      :attr:`cairo.LineCap.ROUND` then these sub-paths will be drawn as
      circular dots. Note that in the case of :attr:`cairo.LineCap.SQUARE` a
      degenerate sub-path will not be drawn at all, (since the correct
      orientation is indeterminate).

      In no case will a cap style of :attr:`cairo.LineCap.BUTT` cause anything
      to be drawn in the case of either degenerate segments or sub-paths.

   .. method:: stroke_extents()

      :returns: (x1, y1, x2, y2), all float
      :rtype: tuple

      * *x1*: left of the resulting extents
      * *y1*: top of the resulting extents
      * *x2*: right of the resulting extents
      * *y2*: bottom of the resulting extents

      Computes a bounding box in user coordinates covering the area that would
      be affected, (the "inked" area), by a :meth:`.stroke` operation given
      the current path and stroke parameters. If the current path is empty,
      returns an empty rectangle (0, 0, 0, 0). Surface dimensions and
      clipping are not taken into account.

      Note that if the line width is set to exactly zero, then
      :meth:`.stroke_extents` will return an empty rectangle. Contrast with
      :meth:`.path_extents` which can be used to compute the non-empty bounds
      as the line width approaches zero.

      Note that :meth:`.stroke_extents` must necessarily do more work to
      compute the precise inked areas in light of the stroke parameters, so
      :meth:`.path_extents` may be more desirable for sake of performance if
      non-inked path extents are desired.

      See :meth:`.stroke`, :meth:`.set_line_width`, :meth:`.set_line_join`,
      :meth:`.set_line_cap`, :meth:`.set_dash`, and :meth:`.stroke_preserve`.

   .. method:: stroke_preserve()

      A drawing operator that strokes the current path according to the
      current line width, line join, line cap, and dash settings. Unlike
      :meth:`.stroke`, :meth:`.stroke_preserve` preserves the path within the
      cairo context.

      See :meth:`.set_line_width`, :meth:`.set_line_join`,
      :meth:`.set_line_cap`, :meth:`.set_dash`, and :meth:`.stroke_preserve`.

   .. method:: text_extents(text)

      :param text: text to get extents for
      :type text: text
      :rtype: TextExtents

      Gets the extents for a string of text. The extents describe a user-space
      rectangle that encloses the "inked" portion of the text, (as it would be
      drawn by :meth:`Context.show_text`). Additionally, the x_advance and
      y_advance values indicate the amount by which the current point would be
      advanced by :meth:`Context.show_text`.

      Note that whitespace characters do not directly contribute to the size
      of the rectangle (extents.width and extents.height). They do contribute
      indirectly by changing the position of non-whitespace characters. In
      particular, trailing whitespace characters are likely to not affect the
      size of the rectangle, though they will affect the x_advance and
      y_advance values.

   .. method:: text_path(text)

      :param text: text
      :type text: text

      Adds closed paths for text to the current path. The generated path if
      filled, achieves an effect similar to that of :meth:`Context.show_text`.

      Text conversion and positioning is done similar to
      :meth:`Context.show_text`.

      Like :meth:`Context.show_text`, After this call the current point is
      moved to the origin of where the next glyph would be placed in this same
      progression. That is, the current point will be at the origin of the
      final glyph offset by its advance values.  This allows for chaining
      multiple calls to to :meth:`Context.text_path` without having to set
      current point in between.

      Note: The :meth:`.text_path` function call is part of what the cairo
      designers call the "toy" text API. It is convenient for short demos and
      simple programs, but it is not expected to be adequate for serious
      text-using applications. See :meth:`Context.glyph_path` for the "real"
      text path API in cairo.

   .. method:: transform(matrix)

      :param matrix: a transformation :class:`Matrix` to be applied to the
        user-space axes

      Modifies the current transformation matrix (CTM) by applying *matrix* as
      an additional transformation. The new transformation of user space takes
      place after any existing transformation.

   .. method:: translate(tx, ty)

      :param tx: amount to translate in the X direction
      :type tx: float
      :param ty: amount to translate in the Y direction
      :type ty: float

      Modifies the current transformation matrix (CTM) by translating the
      user-space origin by *(tx, ty)*. This offset is interpreted as a
      user-space coordinate according to the CTM in place before the new call
      to :meth:`.translate`. In other words, the translation of the user-space
      origin takes place after any existing transformation.

   .. method:: user_to_device(x, y)

      :param x: X value of coordinate
      :type x: float
      :param y: Y value of coordinate
      :type y: float
      :returns: (x, y), both float
      :rtype: tuple

      * *x*: X value of coordinate
      * *y*: Y value of coordinate

      Transform a coordinate from user space to device space by multiplying
      the given point by the current transformation matrix (CTM).

   .. method:: user_to_device_distance(dx, dy)

      :param dx: X value of a distance vector
      :type dx: float
      :param dy: Y value of a distance vector
      :type dy: float
      :returns: (dx, dy), both float
      :rtype: tuple

      * *dx*: X value of a distance vector
      * *dy*: Y value of a distance vector

      Transform a distance vector from user space to device space. This
      function is similar to :meth:`Context.user_to_device` except that the
      translation components of the CTM will be ignored when transforming
      *(dx,dy)*.

   .. method:: in_clip(x, y)

      :param float x: X coordinate of the point to test
      :param float y: Y coordinate of the point to test
      :returns: :obj:`True` if the point is inside, or :obj:`False` if outside.
      :rtype: bool

      Tests whether the given point is inside the area that would be visible
      through the current clip, i.e. the area that would be filled by a
      :meth:`paint` operation.

      See :meth:`clip`, and :meth:`clip_preserve`.

      .. versionadded:: 1.12.0

    .. method:: show_text_glyphs(utf8, glyphs, clusters, cluster_flags)

      :param text utf8: a string of text
      :param list glyphs: list of glyphs to show
      :param list clusters: list of cluster mapping information
      :param TextClusterFlags cluster_flags: cluster mapping flags
      :raises Error:

      .. versionadded:: 1.15

      This operation has rendering effects similar to
      :meth:`Context.show_glyphs` but, if the target surface supports it, uses
      the provided text and cluster mapping to embed the text for the glyphs
      shown in the output. If the target does not support the extended
      attributes, this function acts like the basic
      :meth:`Context.show_glyphs` as if it had been passed ``glyphs`` .

      The mapping between utf8 and glyphs is provided by a list of clusters.
      Each cluster covers a number of text bytes and glyphs, and neighboring
      clusters cover neighboring areas of utf8 and glyphs . The clusters
      should collectively cover utf8 and glyphs in entirety.

      The first cluster always covers bytes from the beginning of utf8 . If
      ``cluster_flags`` do not have the :attr:`TextClusterFlags.BACKWARD` set,
      the first cluster also covers the beginning of glyphs , otherwise it
      covers the end of the glyphs array and following clusters move backward.

      See :class:`TextCluster` for constraints on valid clusters.

    .. method:: stroke_to_path()

        .. note:: This function is not implemented in cairo, but still
            mentioned in the documentation.

    .. method:: tag_begin(tag_name, attributes)

        :param text tag_name: tag name
        :param text attributes: tag attributes

        Marks the beginning of the tag_name structure. Call :meth:`tag_end`
        with the same tag_name to mark the end of the structure.

        The attributes string is of the form "key1=value2 key2=value2 ...".
        Values may be boolean (true/false or 1/0), integer, float, string, or
        an array.

        String values are enclosed in single quotes ('). Single quotes and
        backslashes inside the string should be escaped with a backslash.

        Boolean values may be set to true by only specifying the key. eg the
        attribute string "key" is the equivalent to "key=true".

        Arrays are enclosed in '[]'. eg "rect=[1.2 4.3 2.0 3.0]".

        If no attributes are required, attributes can be an empty string.

        See `Tags and Links Description
        <https://www.cairographics.org/manual/cairo-Tags-and-Links.html#cairo-Tags-and-Links.description>`__
        for the list of tags and attributes.

        Invalid nesting of tags or invalid attributes will cause the context
        to shutdown with a status of :attr:`Status.TAG_ERROR`.

        See :meth:`tag_end`.

        .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

    .. method:: tag_end(tag_name)

        :param text tag_name: tag name

        Marks the end of the tag_name structure.

        Invalid nesting of tags will cause the context to shutdown with a
        status of :attr:`Status.TAG_ERROR`.

        See :meth:`tag_begin`.

        .. versionadded:: 1.18.0 Only available with cairo 1.15.10+
