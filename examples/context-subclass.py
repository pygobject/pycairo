#!/usr/bin/env python
"""test/example of subclassing cairo.Context
"""

import cairo

WIDTH, HEIGHT = 400, 400


class MyContext(cairo.Context):
    def triangle(self, x, y, size):
        self.new_path()
        self.move_to(x, y)
        self.rel_line_to(size, 2*size)
        self.rel_line_to(-2*size, 0)
        self.close_path()

    def bowtie(self, x, y, size):
        self.new_path()
        self.move_to(x, y)
        self.rel_line_to(2*size, 2*size)
        self.rel_line_to(-2*size, 0)
        self.rel_line_to(2*size, -2*size)
        self.close_path()

    def draw(self):
        self.rectangle (0, 0, WIDTH, HEIGHT)
        self.set_rgb_color (1, 1, 1)
        self.fill()

        self.triangle (WIDTH/2, 20, 50)
        self.set_rgb_color (1, 0, 0)
        self.set_line_width(6)
        self.fill()

        self.bowtie (WIDTH/2, 150, 50)
        self.set_rgb_color (0, 1, 0)
        self.stroke()
        
try:
    fileObject = file('context-subclass.png', 'wb')
except IOError, exc:
    raise SystemExit("%s: %s" % (exc.filename, exc.strerror))

ctx = MyContext()
ctx.set_target_png (fileObject, cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
ctx.draw()
ctx.show_page()
