# -*- coding: utf-8 -*-

import os
import array
import tempfile
import struct

import cairo
import pytest


@pytest.mark.skipif(not cairo.HAS_PNG_FUNCTIONS, reason="not png support")
def test_image_surface_create_for_data_array():
    width, height = 255, 255
    data = array.array('B', [0] * width * height * 4)

    for y in range(height):
        for x in range(width):
            offset = (x + (y * width)) * 4
            alpha = y

            b = int(x * alpha/255.0)
            g = int(y * alpha/255.0)
            # cairo.FORMAT_ARGB32 uses pre-multiplied alpha
            data[offset:offset+3] = array.array(
                "B", struct.pack("=I", b | g << 8 | alpha << 24))

    surface = cairo.ImageSurface.create_for_data(
        data, cairo.FORMAT_ARGB32, width, height)

    # for debugging
    fd, filename = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
    os.close(fd)
    surface.write_to_png(filename)
    os.unlink(filename)
