import gi

gi.require_version("Adw", "1")
gi.require_version("Gtk", "4.0")
from gi.repository import Adw, Gtk


def draw_event(widget, ctx, w, h, data):
    ctx.set_line_width(6)
    ctx.set_tolerance(0.1)

    ctx.select_font_face("sans-serif")
    ctx.set_font_size(48)
    (x, y, width, height, dx, dy) = ctx.text_extents("Hello World")

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
    ctx.show_text("Hello World")


def on_activate(app):
    win = Gtk.ApplicationWindow(application=app, title="Cairo GTK Text Example")
    win.set_default_size(450, 150)

    drawingarea = Gtk.DrawingArea()
    drawingarea.set_draw_func(draw_event, None)
    win.set_child(drawingarea)
    win.present()


app = Adw.Application()
app.connect("activate", on_activate)

app.run(None)
