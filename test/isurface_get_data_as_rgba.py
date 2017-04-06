#!/usr/bin/env python
"""
Test ImageSurface.get_data_as_rgba() - read a pycairo surface into a PIL image.
"""

import cairo
import Image

filename_base = "/tmp/file"

def test_get_data_as_rgba (surface, filename):
    # read cairo surface into PIL and save as PNG
    width, height = surface.get_width(), surface.get_height()
    data = surface.get_data_as_rgba()
    im = Image.frombuffer ("RGBA", (width, height), data, "raw", "RGBA", 0,1)
    im.save (filename)

width, height = 150, 100
surface = cairo.ImageSurface (cairo.FORMAT_ARGB32, width, height)
ctx = cairo.Context (surface)
ctx.scale (width, height)

ctx.move_to (0.0, 0.0)
ctx.line_to (1.0, 1.0)
ctx.set_line_width (0.05)
ctx.stroke()

ctx.select_font_face ("Sans", cairo.FONT_SLANT_NORMAL,
                     cairo.FONT_WEIGHT_BOLD)
ctx.set_font_size (0.08)
ctx.move_to (0.05, 0.5)
ctx.set_source_rgba (0.0, 0.0, 1.0, 1.0)
ctx.show_text ("get_data_as_rgba test")

# save surface from cairo and PIL
surface.write_to_png (filename_base + '_cairo.png')
test_get_data_as_rgba (surface, filename_base + '_pil.png')
