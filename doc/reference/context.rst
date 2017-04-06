.. _context:

*******************************
Cairo Context (incomplete docs)
*******************************

.. comment block
   example reST:
   (add back '..' for each line except ':' lines)
 . class:: module.C[(signature)]
   .. method:: C.method(p1, p2)

     :param p1: xxx
     :param p2: xxx
     :type p2: integer or None
     :returns: xxx
     :rtype: list of strings
     :raises: xxx

   staticmethod:: name(signature)
   attribute:: C.name
..


class Context()
===============

.. class:: Context()

   .. method:: append_path()

   .. method:: arc()

   .. method:: arc_negative()

   .. method:: clip()

   .. method:: clip_extents()

   .. method:: clip_preserve()

   .. method:: close_path()

   .. method:: copy_clip_rectangle_list()

   .. method:: copy_page()

   .. method:: copy_path()

   .. method:: copy_path_flat()

   .. method:: curve_to()

   .. method:: device_to_user()

   .. method:: device_to_user_distance()

   .. method:: fill()

   .. method:: fill_extents()

   .. method:: fill_preserve()

   .. method:: font_extents()

   .. method:: get_antialias()

   .. method:: get_current_point()

   .. method:: get_dash()

   .. method:: get_dash_count()

   .. method:: get_fill_rule()

   .. method:: get_font_face()

   .. method:: get_font_matrix()

   .. method:: get_font_options()

   .. method:: get_group_target()

   .. method:: get_line_cap()

   .. method:: get_line_join()

   .. method:: get_line_width()

   .. method:: get_matrix()

   .. method:: get_miter_limit()

   .. method:: get_operator()

   .. method:: get_scaled_font()

   .. method:: get_source()

   .. method:: get_target()

   .. method:: get_tolerance()

   .. method:: glyph_extents()

   .. method:: glyph_path()

   .. method:: has_current_point()

   .. method:: identity_matrix()

   .. method:: in_fill()

   .. method:: in_stroke()

   .. method:: line_to()

   .. method:: mask()

   .. method:: mask_surface()

   .. method:: move_to()

   .. method:: new_path()

   .. method:: new_sub_path()

   .. method:: paint()

   .. method:: paint_with_alpha()

   .. method:: path_extents()

   .. method:: pop_group()

   .. method:: pop_group_to_source()

   .. method:: push_group()

   .. method:: push_group_with_content()

   .. method:: rectangle()

   .. method:: rel_curve_to()

   .. method:: rel_line_to()

   .. method:: rel_move_to()

   .. method:: reset_clip()

   .. method:: restore()

   .. method:: rotate()

   .. method:: save()

   .. method:: scale()

   .. method:: select_font_face()

   .. method:: set_antialias()

   .. method:: set_dash()

   .. method:: set_fill_rule()

   .. method:: set_font_face()

   .. method:: set_font_matrix()

   .. method:: set_font_options()

   .. method:: set_font_size()

   .. method:: set_line_cap()

   .. method:: set_line_join()

   .. method:: set_line_width()

   .. method:: set_matrix()

   .. method:: set_miter_limit()

   .. method:: set_operator()

   .. method:: set_source()

   .. method:: set_source_rgb()

   .. method:: set_source_rgba()

   .. method:: set_source_surface()

   .. method:: set_tolerance()

   .. method:: show_glyphs()

   .. method:: show_page()

   .. method:: show_text()

   .. method:: stroke()

   .. method:: stroke_extents()

   .. method:: stroke_preserve()

   .. method:: text_extents()

   .. method:: text_path()

   .. method:: transform()

   .. method:: translate()

   .. method:: user_to_device()

   .. method:: user_to_device_distance()


.. comment
 incorporate these notes:
 C : cr = cairo_create (surface);
 Py: ctx = cairo.Context (surface)

 C : cairo_set_dash (cairo_t *cr, double *dashes, int ndash, double offset);
 Py: ctx.set_dash (dash_sequence, offset)

 Methods supporting default argument values:
 ctx.mask_surface (surface, x=0.0, y=0.0)
 ctx.select_font_face (family, slant=cairo.FONT_SLANT_NORMAL)
                       weight=cairo.FONT_WEIGHT_NORMAL)
 ctx.set_source_surface (surface, x=0.0, y=0.0)
 ctx.set_source_rgba (r, g, b, a=1.0)
