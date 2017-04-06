#!/usr/bin/env python
"""Python version of cairo-demo/cairo_snippets/cairo_snippets_ps.c
create a file for each example rather than one large file for all examples
"""
from __future__ import division
from math import pi as M_PI  # used by many snippets
import sys

import cairo

from snippets import snip_list, snippet_normalize, snippet_set_bg_svg

X_inches, Y_inches = 2, 2
width, height = X_inches * 72, Y_inches * 72  # used by snippet_normalize()

Verbose_mode = True

def do_snippet (snippet):
    if Verbose_mode:
        print 'processing %s' % snippet,
    
    filename = 'snippets/%s.ps' % snippet
    surface = cairo.PSSurface (filename, width, height)
    cr = cairo.Context (surface)

    cr.save()
    try:    
        execfile ('snippets/%s.py' % snippet, globals(), locals())
    except:
        exc_type, exc_value = sys.exc_info()[:2] 
        print exc_type, exc_value
    else:
        cr.restore()
        cr.show_page()
    
    if Verbose_mode:
        print

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == '-s':
        Verbose_mode=False
        del sys.argv[1]
    
    if len(sys.argv) > 1: # do specified snippets
        snippet_list = sys.argv[1:]
    else:                 # do all snippets
        snippet_list = snip_list

    for s in snippet_list:
        do_snippet (s)
