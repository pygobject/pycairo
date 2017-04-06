.. _text:

****
Text
****

Cairo has two sets of text rendering capabilities:

* The functions with text in their name form cairo's toy text API. The toy API takes UTF-8 encoded text and is limited in its functionality to rendering simple left-to-right text with no advanced features. That means for example that most complex scripts like Hebrew, Arabic, and Indic scripts are out of question. No kerning or correct positioning of diacritical marks either. The font selection is pretty limited too and doesn't handle the case that the selected font does not cover the characters in the text. This set of functions are really that, a toy text API, for testing and demonstration purposes. Any serious application should avoid them.

* The functions with glyphs in their name form cairo's low-level text API. The low-level API relies on the user to convert text to a set of glyph indexes and positions. This is a very hard problem and is best handled by external libraries, like the pangocairo that is part of the Pango text layout and rendering library. Pango is available from http://www.pango.org/.



class FontFace()
================

A *cairo.FontFace* specifies all aspects of a font other than the size or font
matrix (a font matrix is used to distort a font by sheering it or scaling it
unequally in the two directions). A *FontFace* can be set on a
:class:`Context` by using :meth:`Context.set_font_face` the size and font
matrix are set with :meth:`Context.set_font_size` and
:meth:`Context.set_font_matrix`.

*FontFace* cannot be instantiated directly, it is returned from
:meth:`Context.get_font_face`

There are various types of *FontFace*, depending on the font backend they
use.


class ScaledFont()
==================

A *ScaledFont* is a font scaled to a particular size and device resolution. A
*ScaledFont* is most useful for low-level font usage where a library or
application wants to cache a reference to a scaled font to speed up the
computation of metrics.

There are various types of scaled fonts, depending on the font backend they
use.

.. class:: ScaledFont(font_face, font_matrix, ctm, options)

   :param font_face: a :class:`FontFace` instance
   :param font_matrix: font space to user space transformation :class:`Matrix` for the
      font. In the simplest case of a N point font, this matrix is
      just a scale by N, but it can also be used to shear the font
      or stretch it unequally along the two axes. See :meth:`Context.set_font_matrix`.
   :param ctm: user to device transformation :class:`Matrix` with which the font will
      be used.
   :param options: a :class:`FontOptions` instance to use when getting metrics for the font and rendering with it.

   Creates a *ScaledFont* object from a *FontFace* and matrices that describe
   the size of the font and the environment in which it will be used.

   .. method:: extents()

      :returns: (ascent, descent, height, max_x_advance, max_y_advance), a tuple of float values.

      Gets the metrics for a *ScaledFont*.

   .. method:: get_font_face()

      :returns: the :class:`FontFace` that this *ScaledFont* was created for.

      Since: 1.2

   .. method:: get_scale_matrix()

      :returns: the scale :class:`Matrix`

      The scale matrix is product of the font matrix and the ctm associated
      with the scaled font, and hence is the matrix mapping from font space to
      device space.

      Since: 1.8


   .. method:: text_extents()

      :param utf8: text (str or unicode), encoded in UTF-8
      :returns: (x_bearing, y_bearing, width, height, x_advance, y_advance), a tuple of float values.

      Gets the extents for a string of text. The extents describe a user-space
      rectangle that encloses the "inked" portion of the text drawn at the
      origin (0,0) (as it would be drawn by :meth:`Context.show_text` if the cairo
      graphics state were set to the same font_face, font_matrix, ctm, and
      font_options as *ScaledFont*).  Additionally, the x_advance and
      y_advance values indicate the amount by which the current point would be
      advanced by :meth:`Context.show_text`.

      Note that whitespace characters do not directly contribute to the size
      of the rectangle (width and height). They do contribute indirectly by
      changing the position of non-whitespace characters. In particular,
      trailing whitespace characters are likely to not affect the size of the
      rectangle, though they will affect the x_advance and y_advance values.

      Since: 1.2


class FontOptions()
===================

An opaque structure holding all options that are used when rendering fonts.

Individual features of a *FontOptions* can be set or accessed using functions
named *FontOptions.set_<feature_name>* and
*FontOptions.get_<feature_name>*,  like :meth:`FontOptions.set_antialias`
and :meth:`FontOptions.get_antialias`.

New features may be added to a *FontOptions* in the future. For this reason,
:meth:`FontOptions.copy()`, :meth:`FontOptions.equal()`,
:meth:`FontOptions.merge()`, and :meth:`FontOptions.hash()` should be used to
copy, check for equality, merge, or compute a hash value of FontOptions
objects.

.. class:: FontOptions()

   :returns: a newly allocated *FontOptions*.

   Allocates a new *FontOptions* object with all options initialized to default values.

   .. method:: get_antialias()

      :returns: the antialiasing mode for the *FontOptions* object

      Gets the antialiasing mode for the *FontOptions* object.
      See the documentation for :ref:`antialias <mattributes_antialias>`
      for full details.

   .. method:: get_hint_metrics()

      :returns: the metrics hinting mode for the *FontOptions* object

      Gets the metrics hinting mode for the *FontOptions* object.
      See the documentation for :ref:`hint metrics <mattributes_hint_metrics>`
      for full details.

   .. method:: get_hint_style()

      :returns: the hint style for the *FontOptions* object

      Gets the hint style for font outlines for the *FontOptions* object.
      See the documentation for :ref:`hint style <mattributes_hint_style>`
      for full details.

   .. method:: get_subpixel_order()

      :returns: the subpixel order for the *FontOptions* object

      Gets the subpixel order for the *FontOptions* object.
      See the documentation for :ref:`subpixel order <mattributes_subpixel>`
      for full details.

   .. method:: set_antialias(antialias)

      :param antialias: the new antialiasing mode

      Sets the antialiasing mode for the *FontOptions* object. This specifies
      the type of antialiasing to do when rendering text.
      See the documentation for :ref:`antialias <mattributes_antialias>`
      for full details.

   .. method:: set_hint_metrics(hint_metrics)

      :param hint_metrics: the new metrics hinting mode

      Sets the metrics hinting mode for the *FontOptions* object. This controls
      whether metrics are quantized to integer values in device units.
      See the documentation for :ref:`hint metrics <mattributes_hint_metrics>`
      for full details.

   .. method:: set_hint_style(hint_style)

      :param hint_style: the new hint style

      Sets the hint style for font outlines for the *FontOptions* object. This
      controls whether to fit font outlines to the pixel grid, and if so,
      whether to optimize for fidelity or contrast.
      See the documentation for :ref:`hint style <mattributes_hint_style>`
      for full details.

   .. method:: set_subpixel_order(subpixel_order)

      :param subpixel_order: the new subpixel order

      Sets the subpixel order for the *FontOptions* object. The subpixel order
      specifies the order of color elements within each pixel on the display
      device when rendering with an antialiasing mode of
      :attr:`cairo.ANTIALIAS_SUBPIXEL`.
      See the documentation for :ref:`subpixel order <mattributes_subpixel>`
      for full details.
