.. _text:

****
Text
****

.. currentmodule:: cairo

Cairo has two sets of text rendering capabilities:

* The functions with text in their name form cairo's toy text API. The toy API
  takes UTF-8 encoded text and is limited in its functionality to rendering
  simple left-to-right text with no advanced features. That means for example
  that most complex scripts like Hebrew, Arabic, and Indic scripts are out of
  question. No kerning or correct positioning of diacritical marks either. The
  font selection is pretty limited too and doesn't handle the case that the
  selected font does not cover the characters in the text. This set of
  functions are really that, a toy text API, for testing and demonstration
  purposes. Any serious application should avoid them.

* The functions with glyphs in their name form cairo's low-level text API. The
  low-level API relies on the user to convert text to a set of glyph indexes
  and positions. This is a very hard problem and is best handled by external
  libraries, like the pangocairo that is part of the Pango text layout and
  rendering library. Pango is available from http://www.pango.org/.


class FontFace()
================

A *cairo.FontFace* specifies all aspects of a font other than the size or font
matrix (a font matrix is used to distort a font by sheering it or scaling it
unequally in the two directions). A *FontFace* can be set on a
:class:`Context` by using :meth:`Context.set_font_face` the size and font
matrix are set with :meth:`Context.set_font_size` and
:meth:`Context.set_font_matrix`.

There are various types of *FontFace*, depending on the font backend they
use.

.. autoclass:: FontFace
    :members:
    :undoc-members:


class FreeTypeFontFace(:class:`FontFace`)
=========================================

FreeType Fonts - Font support for FreeType.

The FreeType font backend is primarily used to render text on GNU/Linux
systems, but can be used on other platforms too.

   .. note:: FreeType Fonts are not implemented in pycairo because there is no open source Python bindings to FreeType (and fontconfig) that provides a C API. This a possible project idea for anyone interested in adding FreeType support to pycairo.


class ToyFontFace(:class:`FontFace`)
====================================

The *cairo.ToyFontFace* class can be used instead of :meth:`Context.select_font_face` to create a toy font independently of a context.

.. autoclass:: ToyFontFace
    :members:
    :undoc-members:

    .. automethod:: __init__

class UserFontFace(:class:`FontFace`)
=====================================

The user-font feature allows the cairo user to provide drawings for glyphs in
a font. This is most useful in implementing fonts in non-standard formats,
like SVG fonts and Flash fonts, but can also be used by games and other
application to draw "funky" fonts.

   .. note:: UserFontFace support has not (yet) been added to pycairo. If you
      need this feature in pycairo register your interest by sending a message
      to the cairo mailing list, or by opening a pycairo bug report.


class ScaledFont()
==================

A *ScaledFont* is a font scaled to a particular size and device resolution. A
*ScaledFont* is most useful for low-level font usage where a library or
application wants to cache a reference to a scaled font to speed up the
computation of metrics.

There are various types of scaled fonts, depending on the font backend they
use.

.. autoclass:: ScaledFont
    :members:
    :undoc-members:

    .. automethod:: __init__


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

.. autoclass:: FontOptions
    :members:
    :undoc-members:

    .. automethod:: __init__