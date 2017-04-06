#!/usr/bin/env python
"""cairo/cairo-demo/png/spiral.c translated into Python
Modified to produce PostScript output
"""

import cairo

PPI = 300
WIDTH_IN, HEIGHT_IN = 4, 4
WIDTH, HEIGHT = WIDTH_IN * 96.0, HEIGHT_IN * 96.0

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

    ctx.set_rgb_color (0, 0, 1)
    ctx.stroke()


try:
    fileObject = file('spiral.ps', 'wb')
except IOError, exc:
    raise SystemExit("%s: %s" % (exc.filename, exc.strerror))

ctx = cairo.Context()
ctx.set_target_ps (fileObject, WIDTH_IN, HEIGHT_IN, PPI, PPI)

ctx.rectangle (0, 0, WIDTH, HEIGHT)
ctx.set_rgb_color (1, 1, 1)
ctx.fill()

draw_spiral (ctx, WIDTH, HEIGHT)

ctx.show_page()
