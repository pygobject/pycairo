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

.. autoclass:: ScaledFont
    :members:
    :undoc-members:

    .. automethod:: __init__


class FontOptions()
===================

.. autoclass:: FontOptions
    :members:
    :undoc-members:

    .. automethod:: __init__
