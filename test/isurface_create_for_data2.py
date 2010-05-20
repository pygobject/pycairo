#!/usr/bin/env python
"""test cairo.ImageSurface.create_for_data() with a numpy array
"""
import tempfile

import cairo
import numpy

if not (cairo.HAS_IMAGE_SURFACE and cairo.HAS_PNG_FUNCTIONS):
  raise SystemExit ('cairo was not compiled with ImageSurface and PNG support')

h, fileName = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
width, height = 255, 255
data = numpy.ndarray (shape=(height,width,4), dtype=numpy.uint8)

for x in range(width):
  for y in range(height):
    alpha = y

    # cairo.FORMAT_ARGB32 uses pre-multiplied alpha
    data[y][x][0] = int(x * alpha/255.0) # B
    data[y][x][1] = int(y * alpha/255.0) # G
    data[y][x][2] = 0                    # R
    data[y][x][3] = alpha                # A

surface = cairo.ImageSurface.create_for_data (data, cairo.FORMAT_ARGB32,
                                              width, height)
ctx = cairo.Context(surface)
surface.write_to_png(fileName)
print "see %s output file" % fileName
