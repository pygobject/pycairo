#!/usr/bin/env python
"""\
Usage: svgconvert.py format files
format - 'pdf', 'png' or 'ps'
files  - one or more svg files to convert

Convert multiple svg files to
- multiple png files, or
- a multipage pdf/ps file\
"""

import sys

import cairo
import cairo.svg

## TODO: add options like pagesize and scale (for PS/PDF)


def convert_ps (svg_files, save_file):
    if not save_file.endswith ('.ps'):
        save_file += '.ps'
    svgs = []
    width, height = 0,0
    for filename in svg_files:
        svg = cairo.svg.Context()
        svg.parse (filename)
        w,h = svg.get_size()
        width = max(width,w)
        height= max(height,h)
        svgs.append(svg)

    ctx = cairo.Context (cairo.PSSurface (save_file, width, height))
    for svg in svgs:
        ## TODO: center the contents on the page when it is smaller
        svg.render (ctx)
        ctx.show_page()

def convert_pdf (svg_files, save_file):
    if not save_file.endswith ('.pdf'):
        save_file += '.pdf'
    svgs = []
    width, height = 0,0
    for filename in svg_files:
        svg = cairo.svg.Context()
        svg.parse (filename)
        w,h = svg.get_size()
        width = max(width,w)
        height= max(height,h)
        svgs.append(svg)

    ctx = cairo.Context (cairo.PDFSurface (save_file, width, height))
    for svg in svgs:
        ## TODO: center the contents on the page when it is smaller
        svg.render (ctx)
        ctx.show_page()

def convert_png (svg_files):
    for filename in svg_files:
        svg = cairo.svg.Context()
        svg.parse (filename)
        width, height = svg.get_size()

        surface = cairo.ImageSurface (cairo.FORMAT_ARGB32, width, height)
        ctx = cairo.Context(surface)
        svg.render (ctx)

        save_file = filename.replace('.svg', '.png')
        surface.write_to_png (save_file)


if len(sys.argv) < 3:
    raise SystemExit(__doc__)

format = sys.argv[1]
svg_files = sys.argv[2:]
save_file = 'svgconvert'  # TODO - allow this to be set, and to be stdout

if format == 'png':
    convert_png (svg_files)
elif format == 'ps':
    convert_ps (svg_files, save_file)
elif format == 'pdf':
    convert_pdf (svg_files, save_file)
else:
    raise SystemExit(__doc__)
