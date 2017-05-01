#!/usr/bin/env python
"""Display a png file
"""

import sys

import cairo
import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk


def draw_event(widget, ctx, surface):
    ctx.set_source_surface(surface, 0, 0)
    ctx.paint()


if len(sys.argv) != 2:
    raise SystemExit('usage: png_view.py png_file')

filename = sys.argv[1]

surface = cairo.ImageSurface.create_from_png(filename)
width = surface.get_width()
height = surface.get_height()

win = Gtk.Window()
win.connect('destroy', Gtk.main_quit)

drawingarea = Gtk.DrawingArea()
win.add(drawingarea)
drawingarea.connect('draw', draw_event, surface)
drawingarea.set_size_request(width, height)

win.show_all()
Gtk.main()
