#!/usr/bin/env python

from __future__ import division
import math
import sys

import gtk
import cairo
import cairo.gtk


def oval_path(ctx, xc, yc, xr, yr):
    matrix = cairo.Matrix ()
    #cairo_current_matrix (cr, matrix);

    ctx.translate (xc, yc)
    ctx.scale (1.0, yr / xr)
    ctx.move_to (xr, 0.0)
    ctx.arc (  0, 0,
               xr,
               0, 2 * math.pi)
    ctx.close_path ()

    ctx.set_matrix (matrix)
    
def fill_checks(ctx, x, y, width, height):
    ctx.save()

    CHECK_SIZE = 32
    check = ctx.target_surface.create_similar(cairo.FORMAT_RGB24,
                                              2*CHECK_SIZE, 2*CHECK_SIZE)
    check.set_repeat(1)

    ctx.save()
    ctx.set_target_surface(check)
    ctx.set_operator(cairo.OPERATOR_SRC)

    ctx.set_rgb_color(0.4, 0.4, 0.4)
    
    ctx.rectangle(0, 0, 2*CHECK_SIZE, 2*CHECK_SIZE)
    ctx.fill()

    ctx.set_rgb_color(0.7, 0.7, 0.7)
    
    ctx.rectangle(x, y, CHECK_SIZE, CHECK_SIZE)
    ctx.fill()
    ctx.rectangle(x+CHECK_SIZE, y+CHECK_SIZE, CHECK_SIZE, CHECK_SIZE)
    ctx.fill()

    ctx.restore()

    check_pattern = cairo.Pattern (surface=check)
    ctx.set_pattern(check_pattern)
    ctx.rectangle(0, 0, width, height)
    ctx.fill()

    ctx.restore()
    

def draw_3circles(ctx, xc, yc, radius):
    subradius = radius * (2 / 3. - 0.1)

    ctx.set_rgb_color(1, 0, 0)
    oval_path(ctx,
              xc + radius / 3. * math.cos(math.pi * 0.5),
              yc - radius / 3. * math.sin(math.pi * 0.5),
              subradius, subradius)
    ctx.fill()

    ctx.set_rgb_color(0, 1, 0)
    oval_path(ctx,
              xc + radius / 3. * math.cos(math.pi * (0.5 + 2/.3)),
              yc - radius / 3. * math.sin(math.pi * (0.5 + 2/.3)),
              subradius, subradius)
    ctx.fill()

    ctx.set_rgb_color(0, 0, 1)
    oval_path(ctx,
              xc + radius / 3. * math.cos(math.pi * (0.5 + 4/.3)),
              yc - radius / 3. * math.sin(math.pi * (0.5 + 4/.3)),
              subradius, subradius)
    ctx.fill()

def expose(drawingarea, event):
    drawable = drawingarea.window
    width = drawingarea.allocation.width
    height = drawingarea.allocation.height

    radius = 0.5 * min(width, height) - 10
    xc = width / 2.
    yc = height / 2.

    ctx = cairo.Context()
    cairo.gtk.set_target_drawable(ctx, drawable)
    surface = ctx.target_surface

    overlay = surface.create_similar(cairo.FORMAT_ARGB32, width,height)
    punch   = surface.create_similar(cairo.FORMAT_A8,     width,height)
    circles = surface.create_similar(cairo.FORMAT_ARGB32, width,height)

    fill_checks(ctx, 0, 0, width, height)

    ctx.save()
    ctx.set_target_surface(overlay)

    # Draw a black circle on the overlay
    ctx.set_rgb_color(0, 0, 0)
    oval_path(ctx, xc, yc, radius, radius)
    ctx.fill()

    ctx.save()
    ctx.set_target_surface(punch)

    # Draw 3 circles to the punch surface, then cut
    # that out of the main circle in the overlay
    draw_3circles(ctx, xc, yc, radius)
    
    ctx.restore()

    ctx.set_operator(cairo.OPERATOR_OUT_REVERSE)
    ctx.show_surface(punch, width, height)

    ctx.save()

    ctx.set_target_surface(circles)
    ctx.set_alpha(0.5)
    ctx.set_operator(cairo.OPERATOR_OVER)
    draw_3circles(ctx, xc, yc, radius)
    
    ctx.restore()

    ctx.set_operator(cairo.OPERATOR_ADD)
    ctx.show_surface(circles, width, height)

    ctx.restore()

    ctx.show_surface(overlay, width, height)
    
def main():
    win = gtk.Window()
    win.connect('destroy', gtk.main_quit)
    win.set_title('Knockout Groups')
    win.set_default_size(400, 400)

    drawingarea = gtk.DrawingArea()
    drawingarea.connect('expose_event', expose)

    win.add(drawingarea)
    win.show_all()

    gtk.main()

if __name__ == '__main__':
    main()
