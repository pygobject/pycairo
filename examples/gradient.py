#!/usr/bin/env python
"""/cairo-demo/cairo_snippets/gradient.cairo translated to Python
"""

import math
import cairo

WIDTH, HEIGHT  = 256, 256

try:
    fileObject = file('gradient.png', 'wb')
except IOError, exc:
    raise SystemExit("%s: %s" % (exc.filename, exc.strerror))

ctx = cairo.Context()
ctx.set_target_png (fileObject, cairo.FORMAT_ARGB32, WIDTH, HEIGHT)

ctx.scale (WIDTH/1.0, HEIGHT/1.0)

pat = cairo.Pattern(x0=0.0, y0=0.0, x1=0.0, y1=1.0)
pat.add_color_stop (1, 0, 0, 0, 1)
pat.add_color_stop (0, 1, 1, 1, 1)

ctx.rectangle (0,0,1,1)
ctx.set_pattern (pat)
ctx.fill ()

pat = cairo.Pattern (cx0=0.45, cy0=0.4, radius0=0.1,
                     cx1=0.4,  cy1=0.4, radius1=0.5)
pat.add_color_stop (0, 1, 1, 1, 1)
pat.add_color_stop (1, 0, 0, 0, 1)

ctx.set_pattern (pat)
ctx.arc (0.5, 0.5, 0.3, 0, 2 * math.pi)
ctx.fill ()

ctx.show_page()
