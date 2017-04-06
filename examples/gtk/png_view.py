#!/usr/bin/env python
"""Display a png file
"""

import sys

import gtk
import cairo
import cairo.gtk

def expose_event(widget, event, surface):
    widget.window.clear()
    
    ctx = cairo.gtk.gdk_cairo_create(widget.window)
    ctx.set_source_surface(surface, 0,0)
    ctx.paint()


if len(sys.argv) != 2:
    raise SystemExit('usage: png_view.py png_file')

filename = sys.argv[1]

surface = cairo.ImageSurface.create_from_png(filename)
Width  = surface.get_width()
Height = surface.get_height()

win = gtk.Window()
win.connect('destroy', lambda x: gtk.main_quit())

drawingarea = gtk.DrawingArea()
win.add(drawingarea)
drawingarea.connect('expose_event', expose_event, surface)
drawingarea.set_size_request(Width,Height)
drawingarea.set_double_buffered(False)
    
win.show_all()
gtk.main()
