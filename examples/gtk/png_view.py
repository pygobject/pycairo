#!/usr/bin/env python
"""Display a png file
"""

import sys

import cairo
import gtk


def expose_event(widget, event, surface):
    ctx = widget.window.cairo_create()
    ctx.set_source_surface(surface, 0,0)
    ctx.paint()


if len(sys.argv) != 2:
    raise SystemExit('usage: png_view.py png_file')

filename = sys.argv[1]

surface = cairo.ImageSurface.create_from_png(filename)
Width  = surface.get_width()
Height = surface.get_height()

win = gtk.Window()
win.connect('destroy', gtk.main_quit)

drawingarea = gtk.DrawingArea()
win.add(drawingarea)
drawingarea.connect('expose_event', expose_event, surface)
drawingarea.set_size_request(Width,Height)

win.show_all()
gtk.main()
