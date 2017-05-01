#!/usr/bin/env python

"""Create a PNG file for each example"""

from __future__ import print_function

import os
import sys
import cairo

from snippets import get_snippets


def do_snippet(snippet):
    if verbose_mode:
        print('processing %s' % snippet.name)

    width, height = 256, 256

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
    cr = cairo.Context(surface)

    cr.save()
    snippet.draw_func(cr, width, height)

    cr.restore()

    try:
        os.makedirs(os.path.join("_build", "png"))
    except EnvironmentError:
        pass
    filename = os.path.join("_build", "png", "%s.png" % snippet.name)

    surface.write_to_png(filename)


if __name__ == '__main__':
    if not(cairo.HAS_IMAGE_SURFACE and cairo.HAS_PNG_FUNCTIONS):
        raise SystemExit(
            'cairo was not compiled with ImageSurface and PNG support')

    verbose_mode = True
    if len(sys.argv) > 1 and sys.argv[1] == '-s':
        verbose_mode = False
        del sys.argv[1]

    snippets = get_snippets()

    if len(sys.argv) > 1:
        # do specified snippets
        selected = [snippets[n] for n in sys.argv[1:]]
    else:
        # do all snippets
        selected = snippets.values()

    for s in selected:
        do_snippet(s)
