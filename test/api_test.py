'''test pycairo API
- can be expanded later as required.
- is not able to test the quality of images created. We assume cairo itself
  tests for this.
'''
from __future__ import division        # new in 2.2, redundant in 3.0
from __future__ import absolute_import # new in 2.5, redundant in 2.7/3.0
from __future__ import print_function  # new in 2.6, redundant in 3.0

import tempfile as tfi

import cairo
import py.test as test


def test_context():
    pass

def test_matrix():
    pass

def test_path():
    pass

def test_patterns():
    pass


def test_surfaces():
    # The Surface type cannot be instantiated
    test.raises(TypeError, "s = cairo.Surface()")

    if cairo.HAS_IMAGE_SURFACE:
        f, w, h = cairo.FORMAT_ARGB32, 100, 100
        s = cairo.ImageSurface(f, w, h)
        assert s.get_format() == f
        assert s.get_width() == w
        assert s.get_height() == h

    if cairo.HAS_PDF_SURFACE:
        f, w, h = tfi.TemporaryFile(mode='w+b'), 100, 100
        s = cairo.PDFSurface(f, w, h)

    if cairo.HAS_PS_SURFACE:
        f, w, h = tfi.TemporaryFile(mode='w+b'), 100, 100
        s = cairo.PSSurface(f, w, h)

    if cairo.HAS_SVG_SURFACE:
        f, w, h = tfi.TemporaryFile(mode='w+b'), 100, 100
        s = cairo.SVGSurface(f, w, h)


def test_text():
    pass
