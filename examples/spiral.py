#!/usr/bin/env python
"""cairo/cairo-demo/png/spiral.c translated into Python
"""

import cairo

WIDTH, HEIGHT = 600, 600

def draw_spiral (ctx, width, height):
    wd = .02 * width
    hd = .02 * height

    width -= 2
    height -= 2

    ctx.move_to (width + 1, 1-hd)
    for i in range(9):
	ctx.rel_line_to (0, height - hd * (2 * i - 1))
	ctx.rel_line_to (- (width - wd * (2 *i)), 0)
	ctx.rel_line_to (0, - (height - hd * (2*i)))
	ctx.rel_line_to (width - wd * (2 * i + 1), 0)

    ctx.set_source_rgb (0, 0, 1)
    ctx.stroke()


surface = cairo.ImageSurface (cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
ctx = cairo.Context(surface)

ctx.set_source_rgb (1, 1, 1)
ctx.set_operator (cairo.OPERATOR_SOURCE)
ctx.paint()

draw_spiral (ctx, WIDTH, HEIGHT)

surface.write_to_png('spiral.png')
