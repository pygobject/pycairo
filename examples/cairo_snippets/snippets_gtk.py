#!/usr/bin/env python

import os.path
import sys

import cairo
import gtk
import pango

from snippets import get_snippets


class Window(gtk.Window):
    """Composite widget"""

    WIDTH, HEIGHT = 400, 400

    def __init__(self, title=None, type=gtk.WINDOW_TOPLEVEL):
        gtk.Window.__init__(self, type)
        self.set_default_size(self.WIDTH, self.HEIGHT)

        self.da = gtk.DrawingArea()
        self.da.connect('expose-event', self.da_expose_event)

        def put_in_frame(widget):
            frame = gtk.Frame(label=None)
            frame.set_property('shadow_type', gtk.SHADOW_IN)
            frame.add(widget)
            return frame

        self.current_snippet = None

        vpaned = gtk.VPaned()
        self.add(vpaned)

        sv = self.create_text_view()
        vpaned.pack1(put_in_frame(sv), True, True)
        sv.set_size_request(self.WIDTH, int(self.HEIGHT / 2))

        hpaned = gtk.HPaned()
        vpaned.pack2(hpaned, True, False)

        sl = self.create_snippet_list()
        hpaned.pack1(put_in_frame(sl), True, True)

        hpaned.pack2(put_in_frame(self.da), True, True)
        self.da.set_size_request(int(self.WIDTH / 2), int(self.HEIGHT / 2))

        # set focus to snippet list
        sl.get_child().grab_focus()

    def da_expose_event(self, da, event, data=None):
        x, y, width, height = da.allocation

        if self.current_snippet is None:
            return False

        cr = da.window.cairo_create()
        self.current_snippet.draw_func(cr, width, height)

        return True

    def create_text_view(self):
        sw = gtk.ScrolledWindow()
        sw.set_property('shadow-type', gtk.SHADOW_IN)
        sw.set_policy(hscrollbar_policy=gtk.POLICY_AUTOMATIC,
                      vscrollbar_policy=gtk.POLICY_AUTOMATIC)

        text_view = gtk.TextView()
        sw.add(text_view)
        # set a fixed width font, so any tabs line up
        text_view.modify_font(pango.FontDescription("Fixed"))

        self.text_buffer = text_view.get_buffer()

        return sw

    def cb_selection_changed(self, tselection, data=None):
        model, iter = tselection.get_selected()
        if iter:
            self.current_snippet = model[iter][1]
            self.text_buffer.set_text(self.current_snippet.code)
            self.da.queue_draw()

    def create_snippet_list(self):
        sw = gtk.ScrolledWindow()
        sw.set_property('shadow-type', gtk.SHADOW_IN)
        sw.set_policy(hscrollbar_policy=gtk.POLICY_NEVER,
                      vscrollbar_policy=gtk.POLICY_AUTOMATIC)

        snippets = get_snippets()
        model = gtk.ListStore(str, object)
        for name, s in snippets.items():
            model.append(row=(name, s))

        tree_view = gtk.TreeView(model)
        sw.add(tree_view)
        tree_view.set_property('headers-visible', False)
        tree_view.set_property('search-column', 0)
        tree_view.set_property('rules-hint', False)

        tselection = tree_view.get_selection()
        tselection.connect("changed", self.cb_selection_changed)
        tselection.set_mode(gtk.SELECTION_BROWSE)

        cr = gtk.CellRendererText()
        tvc = gtk.TreeViewColumn(None, cr, text=0)
        tree_view.append_column(tvc)

        tselection.select_path(0,)  # select first item

        return sw


if __name__ == '__main__':
    app = Window()
    app.connect('destroy', gtk.main_quit)
    app.show_all()
    gtk.main()
