#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Test unicode filenames

Functions/methods using unicode filenames:
  cairo.PDFSurface()
  cairo.PSSurface()
  cairo.SVGSurface()
  surface.write_to_png()
  surface.create_from_png()
"""

import cairo

WIDTH, HEIGHT = 256, 256

#f = open(u"a1ēxāmple.pdf","wb")
#surface = cairo.PDFSurface (f, WIDTH, HEIGHT)

surface = cairo.PDFSurface(u"a1ēxāmple.pdf", WIDTH, HEIGHT)

ctx = cairo.Context(surface)
ctx.set_source_rgb(1,0,0)
ctx.set_operator(cairo.OPERATOR_SOURCE)
ctx.paint()

surface.write_to_png(u"a2ēxāmple.png")
surface.show_page()
surface.flush()
surface.finish()
