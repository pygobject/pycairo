#!/usr/bin/env python

# Possible improvements:
# - Zoom-in/out toolbutton

from __future__ import division
import os
import sys

import gtk
import cairo
import cairo.svg
import cairo.gtk


action_list = [
    ('FileMenu',   None,                 '_File'),
    ('Open',       gtk.STOCK_OPEN,       '_Open',       '<CTL>O',      'Open a file',       'cb_open'),
    ('Quit',       gtk.STOCK_QUIT,       '_Quit',       '<CTL>Q',      'Quit application',  'cb_quit'),
    ]

ui_string = """<ui>
  <menubar name='MenuBar'>
    <menu action='FileMenu'>
      <menuitem action='Open'/>
      <separator name='s1'/>
      <menuitem action='Quit'/>
    </menu>
  </menubar>
</ui>"""


def fix_actions (actions, instance):
    "UIManager Helper function to map method strings to an instance method"
    retval = []
    
    for action in actions:
        if len (action) >= 6: # action[5] is the callcack function as a string
            action = action[0:5] + (getattr (instance, action[5]),) + \
                     action[6:]
        retval += [action]
    return retval


def gdkcolor_to_rgb (gdkcolor):
    return gdkcolor.red/65535, gdkcolor.green/65535, gdkcolor.blue/65535


class Window (gtk.Window):
    def __init__ (self, title=None, type=gtk.WINDOW_TOPLEVEL):
        gtk.Window.__init__ (self, type)
        if title:
            self.set_title (title)
        self.set_default_size(300, 200)

        self.af     = None
        self.da     = None

        self.vbox = gtk.VBox()
        self.add (self.vbox)

        # create UIManager menus
        ag = gtk.ActionGroup ('WindowActions')
        actions        = fix_actions (action_list, self)
        ag.add_actions (actions)
        
        self.ui = gtk.UIManager()
        self.ui.insert_action_group (ag, 0)
        self.add_accel_group (self.ui.get_accel_group())

        try:
            self.ui.add_ui_from_string (ui_string)
        except gobject.GError, exc:
            print 'uimanager.add_ui_from_string() error:', exc
        else:
            path = '/MenuBar'
            menubar = self.ui.get_widget (path)
            if menubar:
                self.vbox.pack_start (menubar, expand=False)
            else:
                print "Error: uimanager.get_widget('%s') failed" % path

        self.fileselect = MyFileChooserDialog(parent=self)


    def create_da (self):
        """add AspectFrame and DrawingArea widgets
        """
        af = gtk.AspectFrame()
        self.vbox.pack_start(af)
        
        da = gtk.DrawingArea()
        af.add(da)
        da.connect ('expose-event', self.cb_da_expose_event)
        da.set_double_buffered(False)
    
        af.show_all()

        da.realize()
        self.rgb_bg = gdkcolor_to_rgb (da.style.bg[gtk.STATE_NORMAL])

        self.af, self.da = af, da


    def cb_da_expose_event (self, da, event, data=None):
        width, height = da.allocation.width, da.allocation.height
        pixmap = gtk.gdk.Pixmap (da.window, width, height)
    
        ctx = cairo.gtk.gdk_cairo_create (pixmap)
    
        # draw to pixmap
        ctx.rectangle(0,0,width,height)
        ctx.set_source_rgb(*self.rgb_bg)
        ctx.fill()
    
        svg_width, svg_height = self.svg.get_size()
        matrix = cairo.Matrix (xx=width/svg_width, yy=height/svg_height)
        ctx.set_matrix (matrix)
        self.svg.render (ctx)

        # draw pixmap to gdk.window
        da.window.draw_drawable(gtk.gdk.GC(da.window), pixmap, 0,0, 0,0, -1,-1)


    def cb_open (self, action, data=None):
        """Open svg file (if one is selected) and render to an off-screen
        pixmap
        """
        filename = self.fileselect.get_filename_from_user()
        if filename:
            self.load_file (filename)


    def load_file (self, filename):
        """parse the svg file
        """
        self.svg = cairo.svg.Context()
        try:
            self.svg.parse (filename)
        except cairo.svg.Error:
            exc_type, exc_value = sys.exc_info()[:2] 
            print >>sys.stderr, '%s: %s' % (exc_type, exc_value)

        else:
            if self.af is None:
                self.create_da()
            
            width, height = self.svg.get_size()
            self.af.set (xalign=0.5, yalign=0.5,
                         ratio=width/height, obey_child=False)
            self.da.queue_draw()


    def cb_quit (self, action, data=None):  
        gtk.main_quit()


class MyFileChooserDialog (gtk.FileChooserDialog):
    """A custom GtkFileSelection class that gets a filename from a user and
    remembers the current dir the next time the fileselection is used.
    """
    def __init__ (self,
                  title   = 'Select a file',
                  parent  = None,
                  action  = gtk.FILE_CHOOSER_ACTION_OPEN,
                  buttons = (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                             gtk.STOCK_OPEN,   gtk.RESPONSE_OK),
                  backend = '',
                  path    = None):
       super (MyFileChooserDialog, self).__init__ (title, parent, action,
                                                   buttons, backend)

       if path: self.path = path
       else:    self.path = os.getcwd() + os.sep

       ffilter = gtk.FileFilter()
       ffilter.set_name ("SVG files")
       ffilter.add_pattern ("*.svg")
       self.add_filter (ffilter)

       ffilter = gtk.FileFilter()
       ffilter.set_name ("All files")
       ffilter.add_pattern ("*")
       self.add_filter (ffilter)

       # previews
       self.preview = gtk.Image()
       self.set_preview_widget (self.preview)
       self.connect ("update-preview", self.update_preview_cb)

    def update_preview_cb(self, widget, data=None):
        filename = self.get_preview_filename()
        if filename is None or not os.path.isfile(filename):
            self.set_preview_widget_active (False)
            return

        try:
            pixbuf = gtk.gdk.pixbuf_new_from_file_at_size (filename, 128, 128)
            self.preview.set_from_pixbuf (pixbuf)
            self.set_preview_widget_active (True)
        except Exception, exc:
            self.set_preview_widget_active (False)
            

    def get_filename_from_user (self, path=None, title=None):
        if path:  self.path = path
        if title: self.set_title (title)
        if self.path.endswith (os.sep):
            self.set_current_folder (self.path)
        else:
            self.set_filename (self.path)
                
        filename = None
        if self.run() == gtk.RESPONSE_OK:
            self.path = filename = self.get_filename()
        self.hide()
        return filename


if __name__ == '__main__':
    filename = None
    if len(sys.argv) == 2:
        filename = sys.argv[1]
    
    app = Window (title='SVGView')
    app.connect('destroy', gtk.main_quit)
    if filename:
        app.load_file (filename)
    
    app.show_all()
    gtk.main()
