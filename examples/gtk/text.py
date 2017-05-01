#!/usr/bin/env python

import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk


def draw_event(widget, ctx):
    ctx.set_line_width(6)
    ctx.set_tolerance(.1)

    ctx.select_font_face('sans-serif')
    ctx.set_font_size(48)
    (x, y, width, height, dx, dy) = ctx.text_extents('Hello World')

    ctx.translate(100, 100)

    ctx.new_path()
    ctx.move_to(x - 10, y - 10)
    ctx.rel_line_to(width + 20, 0)
    ctx.rel_line_to(0, height + 20)
    ctx.rel_line_to(-(width + 20), 0)
    ctx.close_path()
    ctx.set_source_rgb(0, 0, 1)
    ctx.stroke()

    ctx.move_to(0, 0)
    ctx.set_source_rgb(0, 0, 0)
    ctx.show_text('Hello World')


win = Gtk.Window()
win.connect('destroy', Gtk.main_quit)

drawingarea = Gtk.DrawingArea()
win.add(drawingarea)
drawingarea.connect('draw', draw_event)
drawingarea.set_size_request(400, 150)

win.show_all()
Gtk.main()
