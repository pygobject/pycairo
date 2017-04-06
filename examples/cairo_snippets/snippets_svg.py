#!/usr/bin/env python

from __future__ import division
from math import pi as M_PI  # used by many snippets
import sys

import cairo
if not cairo.HAS_SVG_SURFACE:
    raise SystemExit ('cairo was not compiled with SVG support')

from snippets import snip_list, snippet_normalize


width_in_inches, height_in_inches = 2, 2
width_in_points, height_in_points = width_in_inches * 72, height_in_inches * 72
width, height = width_in_points, height_in_points # used by snippet_normalize()


def do_snippet (snippet):
    if verbose_mode:
        print 'processing %s' % snippet,

    filename = 'snippets/%s.svg' % snippet
    surface = cairo.SVGSurface (filename, width_in_points, height_in_points)
    cr = cairo.Context (surface)

    cr.save()
    try:
        execfile ('snippets/%s.py' % snippet, globals(), locals())
    except:
        exc_type, exc_value = sys.exc_info()[:2]
        print >> sys.stderr, exc_type, exc_value
    else:
        cr.restore()
        cr.show_page()
        surface.finish()

    if verbose_mode:
        print

if __name__ == '__main__':
    verbose_mode = True
    if len(sys.argv) > 1 and sys.argv[1] == '-s':
        verbose_mode = False
        del sys.argv[1]

    if len(sys.argv) > 1: # do specified snippets
        snippet_list = sys.argv[1:]
    else:                 # do all snippets
        snippet_list = snip_list

    for s in snippet_list:
        do_snippet (s)
