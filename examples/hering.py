#!/usr/bin/env python
"""cairo/cairo-demo/png/hering.c translated into Python
"""

from math import pi
import cairo

WIDTH  = 300
HEIGHT = 600

def draw_hering (ctx, width, height):
    LINES= 32
    MAX_THETA = .80 * pi * 2
    THETA_INC = 2.0 * MAX_THETA / (LINES-1)

    ctx.set_rgb_color (0, 0, 0)
    ctx.set_line_width (2.0)

    ctx.save()

    ctx.translate (width / 2, height / 2)
    ctx.rotate (MAX_THETA)
	
    for i in range (LINES):
        ctx.move_to (-2 * width, 0)
        ctx.line_to (2 * width, 0)
        ctx.stroke()
	    
        ctx.rotate (- THETA_INC)

    ctx.restore()

    ctx.set_line_width (6)
    ctx.set_rgb_color (1, 0, 0)

    ctx.move_to (width / 4.0, 0)
    ctx.rel_line_to (0, height)
    ctx.stroke()

    ctx.move_to (3 * width / 4.0, 0)
    ctx.rel_line_to (0, height)
    ctx.stroke()


try:
    fileObject = file('hering.png', 'wb')
except IOError, exc:
    raise SystemExit("%s: %s" % (exc.filename, exc.strerror))

ctx = cairo.Context()
ctx.set_target_png (fileObject, cairo.FORMAT_ARGB32, WIDTH, HEIGHT)

ctx.rectangle (0, 0, WIDTH, HEIGHT)
ctx.set_rgb_color (1, 1, 1)
ctx.fill()

draw_hering (ctx, WIDTH, HEIGHT)

ctx.show_page()
