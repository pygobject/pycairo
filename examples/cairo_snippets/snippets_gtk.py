#!/usr/bin/env python

import gi
gi.require_version("Gtk", "3.0")
gi.require_foreign("cairo")
from gi.repository import Gtk, Pango


from snippets import get_snippets


class Window(Gtk.Window):
    """Composite widget"""

    WIDTH, HEIGHT = 400, 400

    def __init__(self, title=None):
        super(Window, self).__init__()
        self.set_default_size(self.WIDTH, self.HEIGHT)

        self.da = Gtk.DrawingArea()
        self.da.connect('draw', self.da_draw_event)

        def put_in_frame(widget):
            frame = Gtk.Frame(label=None)
            frame.set_property('shadow_type', Gtk.ShadowType.IN)
            frame.add(widget)
            return frame

        self.current_snippet = None

        vpaned = Gtk.VPaned()
        self.add(vpaned)

        sv = self.create_text_view()
        vpaned.pack1(put_in_frame(sv), True, True)
        sv.set_size_request(self.WIDTH, int(self.HEIGHT / 2))

        hpaned = Gtk.HPaned()
        vpaned.pack2(hpaned, True, False)

        sl = self.create_snippet_list()
        hpaned.pack1(put_in_frame(sl), True, True)

        hpaned.pack2(put_in_frame(self.da), True, True)
        self.da.set_size_request(int(self.WIDTH / 2), int(self.HEIGHT / 2))

        # set focus to snippet list
        sl.get_child().grab_focus()

    def da_draw_event(self, da, cr):
        if self.current_snippet is None:
            return False

        alloc = da.get_allocation()
        self.current_snippet.draw_func(cr, alloc.width, alloc.height)

        return True

    def create_text_view(self):
        sw = Gtk.ScrolledWindow()
        sw.set_property('shadow-type', Gtk.ShadowType.IN)
        sw.set_policy(hscrollbar_policy=Gtk.PolicyType.AUTOMATIC,
                      vscrollbar_policy=Gtk.PolicyType.AUTOMATIC)

        text_view = Gtk.TextView()
        sw.add(text_view)
        # set a fixed width font, so any tabs line up
        text_view.override_font(Pango.FontDescription.from_string("Fixed"))

        self.text_buffer = text_view.get_buffer()

        return sw

    def cb_selection_changed(self, tselection, data=None):
        model, iter = tselection.get_selected()
        if iter:
            self.current_snippet = model[iter][1]
            self.text_buffer.set_text(self.current_snippet.code)
            self.da.queue_draw()

    def create_snippet_list(self):
        sw = Gtk.ScrolledWindow()
        sw.set_property('shadow-type', Gtk.ShadowType.IN)
        sw.set_policy(hscrollbar_policy=Gtk.PolicyType.NEVER,
                      vscrollbar_policy=Gtk.PolicyType.AUTOMATIC)

        snippets = get_snippets()
        model = Gtk.ListStore(str, object)
        for name, s in snippets.items():
            model.append(row=(name, s))

        tree_view = Gtk.TreeView(model)
        sw.add(tree_view)
        tree_view.set_property('headers-visible', False)
        tree_view.set_property('search-column', 0)
        tree_view.set_property('rules-hint', False)

        tselection = tree_view.get_selection()
        tselection.connect("changed", self.cb_selection_changed)
        tselection.set_mode(Gtk.SelectionMode.BROWSE)

        cr = Gtk.CellRendererText()
        tvc = Gtk.TreeViewColumn(None, cr, text=0)
        tree_view.append_column(tvc)

        tselection.select_path(0,)  # select first item

        return sw


if __name__ == '__main__':
    app = Window()
    app.connect('destroy', Gtk.main_quit)
    app.show_all()
    Gtk.main()
