#!/usr/bin/env python

"""Create a SVG file for each example"""

from __future__ import print_function

import os
import sys
import cairo

from snippets import get_snippets


def do_snippet(snippet):
    if verbose_mode:
        print('processing %s' % snippet.name)

    width_in_inches, height_in_inches = 2, 2
    width_in_points, height_in_points = \
        width_in_inches * 72, height_in_inches * 72
    width, height = width_in_points, height_in_points

    try:
        os.makedirs(os.path.join("_build", "svg"))
    except EnvironmentError:
        pass
    filename = os.path.join("_build", "svg", "%s.svg" % snippet.name)

    surface = cairo.SVGSurface(filename, width_in_points, height_in_points)
    cr = cairo.Context(surface)

    cr.save()
    snippet.draw_func(cr, width, height)
    cr.restore()
    cr.show_page()
    surface.finish()


if __name__ == '__main__':
    if not cairo.HAS_SVG_SURFACE:
        raise SystemExit('cairo was not compiled with SVG support')

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
