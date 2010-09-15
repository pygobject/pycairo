#!/usr/bin/env python
"""
Test Surface.write_to_png()
"""

import cStringIO
import math
import sys
import StringIO

import cairo


if not (cairo.HAS_IMAGE_SURFACE and cairo.HAS_PNG_FUNCTIONS):
  raise SystemExit ('cairo was not compiled with ImageSurface and PNG support')


class C(object):
  """a file-like object (for testing), it simulates sys.stdout
  """
  def __init__(self):
    self.closed = False

  def write(self, s):
    """just echo to stdout, without newlines"""
    if self.closed:
      raise ValueError("I/O operation on closed file")
    sys.stdout.write(s)

  def close(self):
    self.closed = True


WIDTH, HEIGHT  = 256, 256

surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
ctx = cairo.Context(surface)

ctx.scale(WIDTH/1.0, HEIGHT/1.0)

pat = cairo.LinearGradient(0.0, 0.0, 0.0, 1.0)
pat.add_color_stop_rgba(1, 0, 0, 0, 1)
pat.add_color_stop_rgba(0, 1, 1, 1, 1)

ctx.rectangle(0,0,1,1)
ctx.set_source(pat)
ctx.fill()

pat = cairo.RadialGradient(0.45, 0.4, 0.1,
                            0.4,  0.4, 0.5)
pat.add_color_stop_rgba(0, 1, 1, 1, 1)
pat.add_color_stop_rgba(1, 0, 0, 0, 1)

ctx.set_source(pat)
ctx.arc(0.5, 0.5, 0.3, 0, 2 * math.pi)
ctx.fill()

# a selection of possible args to surface.write_to_png()
#fo = '/tmp/f.png'
fo = file('/tmp/f.png', 'wb')
#fo = StringIO.StringIO()
#fo = cStringIO.StringIO()
#fo = sys.stdout
#fo = C()

#fo.close()  # this should cause: ValueError: I/O operation on closed file
surface.write_to_png(fo)

# for testing StringIO: get data and write to file
#string = fo.getvalue()
#f2 = file('/tmp/f.png', 'wb')
#f2.write(string)
#f2.close()
