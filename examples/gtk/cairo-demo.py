#!/usr/bin/env python
"""Based on cairo-demo/X11/cairo-demo.c"""

import cairo
import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk


SIZE = 30


def triangle(ctx):
    ctx.move_to(SIZE, 0)
    ctx.rel_line_to(SIZE, 2 * SIZE)
    ctx.rel_line_to(-2 * SIZE, 0)
    ctx.close_path()


def square(ctx):
    ctx.move_to(0, 0)
    ctx.rel_line_to(2 * SIZE, 0)
    ctx.rel_line_to(0, 2 * SIZE)
    ctx.rel_line_to(-2 * SIZE, 0)
    ctx.close_path()


def bowtie(ctx):
    ctx.move_to(0, 0)
    ctx.rel_line_to(2 * SIZE, 2 * SIZE)
    ctx.rel_line_to(-2 * SIZE, 0)
    ctx.rel_line_to(2 * SIZE, -2 * SIZE)
    ctx.close_path()


def inf(ctx):
    ctx.move_to(0, SIZE)
    ctx.rel_curve_to(0, SIZE, SIZE, SIZE, 2 * SIZE, 0)
    ctx.rel_curve_to(SIZE, -SIZE, 2 * SIZE, -SIZE, 2 * SIZE, 0)
    ctx.rel_curve_to(0, SIZE, -SIZE, SIZE, -2 * SIZE, 0)
    ctx.rel_curve_to(-SIZE, -SIZE, -2 * SIZE, -SIZE, -2 * SIZE, 0)
    ctx.close_path()


def draw_shapes(ctx, x, y, fill):
    ctx.save()

    ctx.new_path()
    ctx.translate(x + SIZE, y + SIZE)
    bowtie(ctx)
    if fill:
        ctx.fill()
    else:
        ctx.stroke()

    ctx.new_path()
    ctx.translate(3 * SIZE, 0)
    square(ctx)
    if fill:
        ctx.fill()
    else:
        ctx.stroke()

    ctx.new_path()
    ctx.translate(3 * SIZE, 0)
    triangle(ctx)
    if fill:
        ctx.fill()
    else:
        ctx.stroke()

    ctx.new_path()
    ctx.translate(3 * SIZE, 0)
    inf(ctx)
    if fill:
        ctx.fill()
    else:
        ctx.stroke()

    ctx.restore()


def fill_shapes(ctx, x, y):
    draw_shapes(ctx, x, y, True)


def stroke_shapes(ctx, x, y):
    draw_shapes(ctx, x, y, False)


def draw(da, ctx):
    ctx.set_source_rgb(0, 0, 0)

    ctx.set_line_width(SIZE / 4)
    ctx.set_tolerance(0.1)

    ctx.set_line_join(cairo.LINE_JOIN_ROUND)
    ctx.set_dash([SIZE / 4.0, SIZE / 4.0], 0)
    stroke_shapes(ctx, 0, 0)

    ctx.set_dash([], 0)
    stroke_shapes(ctx, 0, 3 * SIZE)

    ctx.set_line_join(cairo.LINE_JOIN_BEVEL)
    stroke_shapes(ctx, 0, 6 * SIZE)

    ctx.set_line_join(cairo.LINE_JOIN_MITER)
    stroke_shapes(ctx, 0, 9 * SIZE)

    fill_shapes(ctx, 0, 12 * SIZE)

    ctx.set_line_join(cairo.LINE_JOIN_BEVEL)
    fill_shapes(ctx, 0, 15 * SIZE)
    ctx.set_source_rgb(1, 0, 0)
    stroke_shapes(ctx, 0, 15 * SIZE)


def main():
    win = Gtk.Window()
    win.connect('destroy', Gtk.main_quit)
    win.set_default_size(450, 550)

    drawingarea = Gtk.DrawingArea()
    win.add(drawingarea)
    drawingarea.connect('draw', draw)

    win.show_all()
    Gtk.main()


if __name__ == '__main__':
    main()
