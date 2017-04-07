#!/usr/bin/env python
"""
Test ImageSurface.get_data()
"""
import tempfile

import cairo
import numpy

if not (cairo.HAS_IMAGE_SURFACE and cairo.HAS_PNG_FUNCTIONS):
  raise SystemExit ('cairo was not compiled with ImageSurface and PNG support')

w, h = 128, 128

surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, w, h)
ctx = cairo.Context(surface)

ctx.set_source_rgb(1, 1, 1)  # white
ctx.set_operator(cairo.OPERATOR_SOURCE)
ctx.paint()

# Draw out the triangle using absolute coordinates
ctx.move_to(w/2, h/3)
ctx.line_to(2*w/3, 2*h/3)
ctx.rel_line_to(-1*w/3, 0)
ctx.close_path()

ctx.set_source_rgb(0, 0, 0)  # black
ctx.set_line_width(15)
ctx.stroke()
_, outFileName = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
surface.write_to_png(outFileName)
print "see %s output file" % outFileName

# modify surface using numpy
buf = surface.get_data()
# alternative which should work (?) but reports
# TypeError: buffer is read-only
# - is a Python bug?
#buf = buffer (surface1)

a = numpy.ndarray(shape=(w,h,4), dtype=numpy.uint8, buffer=buf)

# draw a vertical line
a[:,40,0] = 255  # byte 0 is blue on little-endian systems
a[:,40,1] = 0
a[:,40,2] = 0
_, outFileName = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
surface.write_to_png(outFileName)
print "see %s output file" % outFileName
