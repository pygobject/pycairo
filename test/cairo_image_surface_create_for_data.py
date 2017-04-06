#!/usr/bin/env python

import array
import math

import cairo

WIDTH, HEIGHT = 256, 256

data = array.array('c', 'a' * WIDTH * HEIGHT * 4)
stride = WIDTH * 4
surface = cairo.ImageSurface.create_for_data(data, cairo.FORMAT_ARGB32,
                                             WIDTH, HEIGHT, stride);
ctx = cairo.Context()
ctx.set_target_surface(surface)

ctx.scale (WIDTH/1.0, HEIGHT/1.0)

pat = cairo.Pattern.create_linear(0.0, 0.0, 0.0, 1.0)
pat.add_color_stop (1, 0, 0, 0, 1)
pat.add_color_stop (0, 1, 1, 1, 1)

ctx.rectangle (0,0,1,1)
ctx.set_pattern (pat)
ctx.fill ()

pat = cairo.Pattern.create_radial (0.45, 0.4, 0.1,
                                   0.4,  0.4, 0.5)
pat.add_color_stop (0, 1, 1, 1, 1)
pat.add_color_stop (1, 0, 0, 0, 1)

ctx.set_pattern (pat)
ctx.arc (0.5, 0.5, 0.3, 0, 2 * math.pi)
ctx.fill ()

try:
    fileObject = file('cairo_image_surface_create_for_data.png', 'wb')
except IOError, exc:
    raise SystemExit("%s: %s" % (exc.filename, exc.strerror))

surface.write_png(fileObject)
