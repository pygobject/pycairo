#!/usr/bin/env python
# -*- coding: utf-8 -*-
# test unicode filenames

import math
import cairo

WIDTH, HEIGHT = 256, 256

#f = open(u"ēxāmple.pdf","w")
#surface = cairo.PDFSurface (f, WIDTH, HEIGHT)

surface = cairo.PDFSurface (u"a1ēxāmple.pdf", WIDTH, HEIGHT)

ctx = cairo.Context (surface)
ctx.scale (WIDTH, HEIGHT)
pat = cairo.LinearGradient (0.0, 0.0, 0.0, 1.0)
pat.add_color_stop_rgba (1, 0.7, 0, 0, 0.5)
pat.add_color_stop_rgba (0, 0.9, 0.7, 0.2, 1)
ctx.rectangle (0, 0, 1, 1)
ctx.set_source (pat)
ctx.fill ()
ctx.translate (0.1, 0.1)

surface.write_to_png (u"a2ēxāmple.png")
surface.show_page()
surface.flush()
surface.finish()
