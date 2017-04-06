#!/usr/bin/env python

import gtk
import cairo
import cairo.gtk

def expose_event(widget, event):
    ctx = cairo.Context()
    cairo.gtk.set_target_drawable(ctx, widget.window)
    ctx.set_line_width(6)
    ctx.set_tolerance(.1)

    ctx.select_font('sans-serif')
    ctx.scale_font(48)
    (x, y, width, height, dx, dy) = ctx.text_extents('Hello World')

    ctx.translate (100, 100)

    ctx.new_path()
    ctx.move_to(x-10,y-10)
    ctx.rel_line_to(width + 20, 0)
    ctx.rel_line_to(0, height + 20)
    ctx.rel_line_to(-(width + 20), 0)
    ctx.close_path()
    ctx.set_rgb_color(0,0,1)
    ctx.stroke()

    ctx.move_to(0, 0)
    ctx.set_rgb_color(0,0,0)
    ctx.show_text('Hello World')

win = gtk.Window()
win.set_title('Cairo Demo')

drawingarea = gtk.DrawingArea()
drawingarea.set_size_request(400,100)
win.add(drawingarea)

win.show_all()

win.connect('destroy', lambda x: gtk.main_quit())
drawingarea.connect('expose_event', expose_event)

gtk.main()
