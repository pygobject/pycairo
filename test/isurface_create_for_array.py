#!/usr/bin/env python
"""test ImageSurface.create_for_array()
"""

import cairo
import Numeric

array = Numeric.zeros((80,80,4), Numeric.UInt8)
for i in range(80):
    for j in range(80):
        array[i][j][0] = chr(255-3*i)        # B
        array[i][j][1] = chr(255-3*abs(i-j)) # G
        array[i][j][2] = chr(255-3*j)        # R
        array[i][j][3] = 255                 # Alpha

surface = cairo.ImageSurface.create_for_array(array)
ctx = cairo.Context(surface)
surface.write_to_png('isurface_create_for_array.png')
