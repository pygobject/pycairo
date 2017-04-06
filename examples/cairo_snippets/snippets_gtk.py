#!/usr/bin/env python
"""Python version of cairo-demo/cairo_snippets/cairo_snippets_gtk.c
"""
from __future__ import division
from math import pi as M_PI  # used by many snippets
import os.path
import sys

import cairo
import gtk
import pango

from snippets import snip_list, snippet_normalize


Width, Height = 400, 400

snippets_path = os.path.join(os.path.dirname(__file__), 'snippets')


def gdkcolor_to_rgb (gdkcolor):
    return gdkcolor.red/65535, gdkcolor.green/65535, gdkcolor.blue/65535


class Window (gtk.Window):
    """Composite widget"""
    def __init__ (self, title=None, type=gtk.WINDOW_TOPLEVEL):
        gtk.Window.__init__ (self, type)
        self.set_default_size (Width, Height)

        self.da = gtk.DrawingArea()
        self.da.connect('expose-event', self.da_expose_event)

        def put_in_frame (widget):
            frame = gtk.Frame (label=None)
            frame.set_property ('shadow_type', gtk.SHADOW_IN)
            frame.add (widget)
            return frame

        vpaned = gtk.VPaned()
        self.add (vpaned)

        sv = self.create_text_view()
        vpaned.pack1 (put_in_frame (sv), True, True)
        sv.set_size_request (Width, int(Height/2))

        hpaned = gtk.HPaned()
        vpaned.pack2 (hpaned, True, False)

        sl = self.create_snippet_list()
        hpaned.pack1 (put_in_frame (sl), True, True)

        hpaned.pack2 (put_in_frame (self.da), True, True)
        self.da.set_size_request (int(Width/2), int(Height/2))

        # set focus to snippet list
        sl.get_child().grab_focus()


    def da_expose_event (self, da, event, data=None):
        x, y, width, height = da.allocation

        cr = da.window.cairo_create()

        try:
            exec (self.snippet_str, globals(), locals())
        except:
            exc_type, exc_value = sys.exc_info()[:2]
            print >> sys.stderr, exc_type, exc_value

        return True


    def create_text_view (self):
        sw = gtk.ScrolledWindow()
        sw.set_property ('shadow-type', gtk.SHADOW_IN)
        sw.set_policy (hscrollbar_policy=gtk.POLICY_AUTOMATIC,
                       vscrollbar_policy=gtk.POLICY_AUTOMATIC)

        text_view = gtk.TextView()
        sw.add (text_view)
        # set a fixed width font, so any tabs line up
        text_view.modify_font(pango.FontDescription ("Fixed"))

        self.text_buffer = text_view.get_buffer()

        return sw


    def cb_selection_changed (self, tselection, data=None):
        model, iter = tselection.get_selected()
        if iter:
            filename = model[iter][0] + '.py'
            try:
                path = os.path.join(snippets_path, filename)
                file_obj = open(path, 'r')
                self.snippet_str = file_obj.read()
                file_obj.close()
                self.text_buffer.set_text(self.snippet_str)
            except IOError, exc:
                print "%s: %s" % (exc.filename, exc.strerror)

            self._draw_pixmap = True
            self.da.queue_draw()


    def create_snippet_list (self):
        sw = gtk.ScrolledWindow()
        sw.set_property ('shadow-type', gtk.SHADOW_IN)
        sw.set_policy (hscrollbar_policy=gtk.POLICY_NEVER,
                       vscrollbar_policy=gtk.POLICY_AUTOMATIC)

        model = gtk.ListStore (str,)
        for row in snip_list:
            model.append (row=(row,))

        tree_view = gtk.TreeView (model)
        sw.add (tree_view)
        tree_view.set_property ('headers-visible', False)
        tree_view.set_property ('search-column', 0)
        tree_view.set_property ('rules-hint', False)

        tselection = tree_view.get_selection()
        tselection.connect ("changed", self.cb_selection_changed)
        tselection.set_mode (gtk.SELECTION_BROWSE)

        cr = gtk.CellRendererText()
        tvc = gtk.TreeViewColumn (None, cr, text=0)
        tree_view.append_column (tvc)

        tselection.select_path(0,) # select first item

        return sw


if __name__ == '__main__':
    app = Window ()
    app.connect('destroy', gtk.main_quit)
    app.show_all()
    gtk.main()
