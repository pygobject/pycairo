#!/usr/bin/env python
"""cairo/cairo-demo/png/hering.c translated into Python
"""

import math

import cairo

WIDTH  = 300
HEIGHT = 600

def draw_hering (ctx, width, height):
    LINES= 32
    MAX_THETA = .80 * math.pi * 2
    THETA_INC = 2.0 * MAX_THETA / (LINES-1)

    ctx.set_source_rgb (0, 0, 0)
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
    ctx.set_source_rgb (1, 0, 0)

    ctx.move_to (width / 4.0, 0)
    ctx.rel_line_to (0, height)
    ctx.stroke()

    ctx.move_to (3 * width / 4.0, 0)
    ctx.rel_line_to (0, height)
    ctx.stroke()


surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
ctx = cairo.Context(surface)

ctx.set_source_rgb (1, 1, 1)
ctx.set_operator (cairo.OPERATOR_SOURCE)
ctx.paint()

draw_hering (ctx, WIDTH, HEIGHT)

surface.write_to_png('hering.png')
