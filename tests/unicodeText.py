#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Test unicode text

Functions/methods which accept unicode text:
  Context.select_font_face()
  Context.show_text()
  Context.text_extents()
  Context.text_path()
  ToyFontFace()
  ScaledFont.text_extents()
"""

import cairo

width, height = 300,300
surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
ctx = cairo.Context(surface)

ctx.scale(width, height)
ctx.set_line_width(0.04)

ctx.select_font_face("Sans",
                     cairo.FONT_SLANT_NORMAL,
                     cairo.FONT_WEIGHT_NORMAL)
ctx.set_font_size(0.20)
ctx.move_to(0.05, 0.5)
ctx.show_text("ēxāmple.")

surface.write_to_png("test-out.png")
