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
  if cairo.HAS_IMAGE_SURFACE:
    f, w, h = cairo.FORMAT_ARGB32, 100, 100
    s = cairo.ImageSurface(f, w, h)
    ctx = cairo.Context(s)
    ctx.set_source_rgb(1.0, 1.0, 1.0)
    ctx.set_operator(cairo.OPERATOR_SOURCE)
    ctx.paint()


def test_matrix():
  m = cairo.Matrix()
  m.rotate(10)
  m.scale(1.5, 2.5)
  m.translate(10, 20)


def test_path():
  # AttributeError: 'module' object has no attribute 'Path'
  test.raises(AttributeError, "p = cairo.Path()")
  # see examples/warpedtext.py


def test_pattern():
  # TypeError: The Pattern type cannot be instantiated
  test.raises(TypeError, "p = cairo.Pattern()")

  r,g,b,a = 0.1, 0.2, 0.3, 0.4
  p = cairo.SolidPattern(r,g,b,a)
  assert p.get_rgba() == (r,g,b,a)

  # SurfacePattern

  # TypeError: The Gradient type cannot be instantiated
  test.raises(TypeError, "p = cairo.Gradient()")

  x0,y0,x1,y1 = 0.0, 0.0, 0.0, 1.0
  p = cairo.LinearGradient(x0,y0,x1,y1)
  assert p.get_linear_points() == (x0,y0,x1,y1)
  p.add_color_stop_rgba(1, 0, 0, 0, 1)
  p.add_color_stop_rgba(0, 1, 1, 1, 1)

  cx0, cy0, radius0, cx1, cy1, radius1 = 1.0, 1.0, 1.0, 2.0, 2.0, 1.0
  p = cairo.RadialGradient(cx0, cy0, radius0, cx1, cy1, radius1)
  assert p.get_radial_circles() == (cx0, cy0, radius0, cx1, cy1, radius1)
  p.add_color_stop_rgba(0, 1, 1, 1, 1)
  p.add_color_stop_rgba(1, 0, 0, 0, 1)


def test_surface():
  # TypeError: The Surface type cannot be instantiated
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
