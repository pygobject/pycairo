#!/usr/bin/env python
"""/cairo-demo/cairo_snippets/gradient.cairo translated to Python
"""

import math
import cairo

WIDTH, HEIGHT  = 256, 256

surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
ctx = cairo.Context(surface)

ctx.scale (WIDTH/1.0, HEIGHT/1.0)

pat = cairo.LinearGradient (0.0, 0.0, 0.0, 1.0)
pat.add_color_stop_rgba (1, 0, 0, 0, 1)
pat.add_color_stop_rgba (0, 1, 1, 1, 1)

ctx.rectangle (0,0,1,1)
ctx.set_source (pat)
ctx.fill ()

pat = cairo.RadialGradient (0.45, 0.4, 0.1,
                            0.4,  0.4, 0.5)
pat.add_color_stop_rgba (0, 1, 1, 1, 1)
pat.add_color_stop_rgba (1, 0, 0, 0, 1)

ctx.set_source (pat)
ctx.arc (0.5, 0.5, 0.3, 0, 2 * math.pi)
ctx.fill ()

surface.write_to_png('gradient.png')
#surface.write_to_png(10)
