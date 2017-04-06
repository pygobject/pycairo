#!/usr/bin/env python
"""test cairo.ImageSurface.create_for_data() with a Python array
"""

import array

import cairo

dir_ = "/tmp/"
width, height = 255, 255
data = array.array('B', [0] * width * height * 4)

for y in range(height):
    for x in range(width):
        offset = (x + (y * width)) * 4
        alpha = y

        # cairo.FORMAT_ARGB32 uses pre-multiplied alpha
        data[offset+0] = int(x * alpha/255.0) # B
        data[offset+1] = int(y * alpha/255.0) # G
        data[offset+2] = 0                    # R
        data[offset+3] = alpha                # A

surface = cairo.ImageSurface.create_for_data(data, cairo.FORMAT_ARGB32,
                                             width, height)
ctx = cairo.Context(surface)
surface.write_to_png(dir_ + 'for_data1.png')
