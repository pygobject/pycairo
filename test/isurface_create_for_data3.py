#!/usr/bin/env python
"""
use ImageSurface.create_for_data() to access numarray, using the buffer
protocol
"""
import math

import cairo
import numarray

width, height = 30*16, 30*9

data = numarray.zeros ((width,height,4), numarray.UInt8)
stride = width * 4
surface = cairo.ImageSurface.create_for_data (data, cairo.FORMAT_ARGB32,
                                              width, height, stride);
del data  # to test that create_for_data() references 'data'
ctx = cairo.Context (surface)

ctx.scale (width, height)

pat = cairo.LinearGradient (0.0, 0.0, 0.0, 1.0)
pat.add_color_stop_rgba (1, 0, 0, 0, 1)
pat.add_color_stop_rgba (0, 1, 1, 1, 1)

ctx.rectangle (0,0,1,1)
ctx.set_source (pat)
ctx.fill ()

pat = cairo.RadialGradient (0.45, 0.4, 0.1,
                            0.4,  0.4, 0.5)
pat.add_color_stop_rgba (0, 1, 1, 1, 1)
pat.add_color_stop_rgba (1, 0, 0, 0, 1)

ctx.set_source (pat)
ctx.arc (0.5, 0.5, 0.3, 0, 2 * math.pi)
ctx.fill ()

surface.write_to_png ('for_data3.png')
