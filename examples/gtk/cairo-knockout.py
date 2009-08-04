#!/usr/bin/env python
"""Based on gtk+/test/testcairo.c
"""

from __future__ import division
import math
import sys

import cairo
import gtk


def oval_path(ctx, xc, yc, xr, yr):
    ctx.save()

    ctx.translate (xc, yc)
    ctx.scale (1.0, yr / xr)
    ctx.move_to (xr, 0.0)
    ctx.arc (0, 0, xr, 0, 2 * math.pi)
    ctx.close_path ()

    ctx.restore()

def fill_checks(ctx, x, y, width, height):
    CHECK_SIZE = 32

    ctx.rectangle (x, y, width, height)
    ctx.set_source_rgb (0.4, 0.4, 0.4)
    ctx.fill ()

    # Only works for CHECK_SIZE a power of 2
    for j in range (x & -CHECK_SIZE, height, CHECK_SIZE):
        for i in range (y & -CHECK_SIZE, width, CHECK_SIZE):
            if ((i / CHECK_SIZE + j / CHECK_SIZE) % 2 == 0):
                ctx.rectangle (i, j, CHECK_SIZE, CHECK_SIZE)

    ctx.set_source_rgb (0.7, 0.7, 0.7)
    ctx.fill ()

def draw_3circles(ctx, xc, yc, radius, alpha):
    subradius = radius * (2 / 3. - 0.1)

    ctx.set_source_rgba(1, 0, 0, alpha)
    oval_path(ctx,
              xc + radius / 3. * math.cos(math.pi * 0.5),
              yc - radius / 3. * math.sin(math.pi * 0.5),
              subradius, subradius)
    ctx.fill()

    ctx.set_source_rgba(0, 1, 0, alpha)
    oval_path(ctx,
              xc + radius / 3. * math.cos(math.pi * (0.5 + 2/.3)),
              yc - radius / 3. * math.sin(math.pi * (0.5 + 2/.3)),
              subradius, subradius)
    ctx.fill()

    ctx.set_source_rgba(0, 0, 1, alpha)
    oval_path(ctx,
              xc + radius / 3. * math.cos(math.pi * (0.5 + 4/.3)),
              yc - radius / 3. * math.sin(math.pi * (0.5 + 4/.3)),
              subradius, subradius)
    ctx.fill()

def draw (ctx, width, height):
    radius = 0.5 * min(width, height) - 10
    xc = width / 2.
    yc = height / 2.

    target  = ctx.get_target()
    overlay = target.create_similar(cairo.CONTENT_COLOR_ALPHA, width, height)
    punch   = target.create_similar(cairo.CONTENT_ALPHA, width, height)
    circles = target.create_similar(cairo.CONTENT_COLOR_ALPHA, width, height)

    fill_checks(ctx, 0, 0, width, height)

    # Draw a black circle on the overlay
    overlay_cr = cairo.Context (overlay)
    overlay_cr.set_source_rgb (0, 0, 0)
    oval_path (overlay_cr, xc, yc, radius, radius)
    overlay_cr.fill()

    # Draw 3 circles to the punch surface, then cut
    # that out of the main circle in the overlay
    punch_cr = cairo.Context (punch)
    draw_3circles (punch_cr, xc, yc, radius, 1.0)

    overlay_cr.set_operator (cairo.OPERATOR_DEST_OUT)
    overlay_cr.set_source_surface (punch, 0, 0)
    overlay_cr.paint()

    # Now draw the 3 circles in a subgroup again
    # at half intensity, and use OperatorAdd to join up
    # without seams.
    circles_cr = cairo.Context (circles)

    circles_cr.set_operator (cairo.OPERATOR_OVER)
    draw_3circles (circles_cr, xc, yc, radius, 0.5)

    overlay_cr.set_operator (cairo.OPERATOR_ADD)
    overlay_cr.set_source_surface (circles, 0, 0)
    overlay_cr.paint()

    ctx.set_source_surface (overlay, 0, 0)
    ctx.paint()

def expose(drawingarea, event):
    ctx = drawingarea.window.cairo_create()

    _, _, width, height = drawingarea.allocation
    draw (ctx, width, height)

    return False

def main():
    win = gtk.Window()
    win.connect('destroy', gtk.main_quit)
    win.set_title('Knockout Groups')
    win.set_default_size(400, 400)

    drawingarea = gtk.DrawingArea()
    win.add(drawingarea)
    drawingarea.connect('expose_event', expose)

    win.show_all()
    gtk.main()

if __name__ == '__main__':
    main()
