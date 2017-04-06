#!/usr/bin/env python
"""Usage: svg2png.py file [files...]

Convert svg files to png\
"""

import sys

import cairo
import cairo.svg


if len(sys.argv) <= 1:
    raise SystemExit(__doc__)

for filename in sys.argv[1:]:
    if not filename.endswith('.svg'):
        print 'skipping "%s": it is not an svg file' % filename
        continue

    file_out = filename.replace('.svg', '.png')

    svg = cairo.svg.Context()
    svg.parse (filename)
    width, height = svg.get_size()

    surface = cairo.ImageSurface (cairo.FORMAT_ARGB32, width, height)
    ctx = cairo.Context(surface)
    svg.render (ctx)
    surface.write_to_png (file_out)
    print 'saved', file_out
