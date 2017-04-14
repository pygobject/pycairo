# -*- coding: utf-8 -*-

from __future__ import division
from __future__ import absolute_import
from __future__ import print_function

import tempfile as tfi

import cairo
import pytest
import py.test as test


def test_error_context():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    ctx = cairo.Context(surface)
    with pytest.raises(cairo.Error) as excinfo:
        ctx.restore()
    error = excinfo.value
    assert error.status == cairo.STATUS_INVALID_RESTORE
    assert str(error)


def test_error():
    with pytest.raises(cairo.Error) as excinfo:
        raise cairo.Error
    assert excinfo.value.status is None

    with pytest.raises(cairo.Error) as excinfo:
        raise cairo.Error("foo")
    assert excinfo.value.status is None
    assert excinfo.value.args[0] == "foo"

    with pytest.raises(cairo.Error) as excinfo:
        raise cairo.Error("foo", 42)
    assert excinfo.value.status == 42

    class Foo(cairo.Error):
        pass

    Foo("foo", 42)


def test_error_alias():
    assert cairo.Error is cairo.CairoError


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

    with pytest.raises(TypeError):
        m * 42

    with pytest.raises(TypeError):
        m + 42

    assert m != 42
    assert m == m
    assert m != cairo.Matrix()


def test_path():
    # AttributeError: 'module' object has no attribute 'Path'
    test.raises(AttributeError, "p = cairo.Path()")
    # see examples/warpedtext.py


def test_pattern():
    # TypeError: The Pattern type cannot be instantiated
    test.raises(TypeError, "p = cairo.Pattern()")

    r, g, b, a = 0.1, 0.2, 0.3, 0.4
    p = cairo.SolidPattern(r, g, b, a)
    assert p.get_rgba() == (r, g, b, a)

    # SurfacePattern

    # TypeError: The Gradient type cannot be instantiated
    test.raises(TypeError, "p = cairo.Gradient()")

    x0, y0, x1, y1 = 0.0, 0.0, 0.0, 1.0
    p = cairo.LinearGradient(x0, y0, x1, y1)
    assert p.get_linear_points() == (x0, y0, x1, y1)
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

    if cairo.HAS_RECORDING_SURFACE:
        s = cairo.RecordingSurface(cairo.CONTENT_COLOR, None)
        s = cairo.RecordingSurface(cairo.CONTENT_COLOR, (1, 1, 10, 10))

    if cairo.HAS_SVG_SURFACE:
        f, w, h = tfi.TemporaryFile(mode='w+b'), 100, 100
        s = cairo.SVGSurface(f, w, h)


def test_image_surface_get_data():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 3, 3)
    ctx = cairo.Context(surface)
    ctx.paint()
    surface.flush()
    buf = surface.get_data()
    assert buf
    assert len(buf) == 4 * 3 * 3
    assert len(bytes(buf)) == len(buf)
    buf[0:1] = b"\x42"

    newbuf = surface.get_data()
    assert newbuf[0:1] == b"\x42"
    ctx.paint()
    surface.flush()
    assert newbuf[0:1] == b"\x00"


def test_image_surface_create_for_data():
    format_ = cairo.FORMAT_ARGB32
    surface = cairo.ImageSurface(format_, 3, 3)
    ctx = cairo.Context(surface)
    ctx.paint()
    surface.flush()
    buf = surface.get_data()

    new = cairo.ImageSurface.create_for_data(buf, format_, 3, 3)
    assert new.get_data() == buf

    with pytest.raises(ValueError):
        cairo.ImageSurface.create_for_data(buf, format_, 3, -1)
    with pytest.raises(ValueError):
        cairo.ImageSurface.create_for_data(buf, format_, -1, 3)

    with pytest.raises(cairo.Error) as excinfo:
        cairo.ImageSurface.create_for_data(buf, format_, 3, 3, 3)

    assert excinfo.value.status == cairo.STATUS_INVALID_STRIDE


def test_surface_file_obj_error():
    class Fail(object):

        def write(*args):
            raise IOError

    cairo.PDFSurface(Fail(), 100, 100)
    cairo.PSSurface(Fail(), 100, 100)


def test_text():
    pass


def test_region():
    a = cairo.Region()
    assert a.is_empty() is True
    assert a.num_rectangles() == 0

    b = cairo.RectangleInt(1, 2, 10, 12)
    assert repr(b) == "cairo.RectangleInt(1, 2, 10, 12)"
    assert isinstance(repr(b), str)
    d = cairo.RectangleInt(1, 1, 10, 12)
    e = cairo.RectangleInt(1, 3, 8, 12)
    assert (b.x, b.y, b.width, b.height) == (1, 2, 10, 12)
    c = cairo.Region((b, e))
    assert not c.is_empty()
    assert c.num_rectangles() == 2
    assert c.get_rectangle(1).y == 14

    ex = c.get_extents()
    assert ex == cairo.RectangleInt(1, 2, 10, 13)
    assert c.contains_rectangle(d) == cairo.REGION_OVERLAP_PART

    c.translate(10, 20)
    assert c.contains_rectangle(d) == cairo.REGION_OVERLAP_OUT
    assert c.get_rectangle(1) == cairo.RectangleInt(11, 34, 8, 1)

    cp = c.copy()
    assert c.num_rectangles() == cp.num_rectangles()
    assert c.get_rectangle(0) == cp.get_rectangle(0)
    assert c == cp
    assert 3 != c
    assert c != "test"

    c = cairo.Region((b, e))
    c.intersect(d)
    assert c.num_rectangles() == 1
    assert c.get_rectangle(0) == cairo.RectangleInt(1, 2, 10, 11)

    c = cairo.Region((b, e))
    c.subtract(d)
    assert c.num_rectangles() == 2
    assert c == cairo.Region([
        cairo.RectangleInt(1, 13, 10, 1),
        cairo.RectangleInt(1, 14, 8, 1),
    ])

    d = cairo.Region(d)
    c = cairo.Region((b, e))
    c.subtract(d)
    assert c.num_rectangles() == 2
    assert c.get_rectangle(0) == cairo.RectangleInt(1, 13, 10, 1)

    c = cairo.Region((b, e))
    c.union(d)
    assert c.num_rectangles() == 2
    assert c == cairo.Region([
        cairo.RectangleInt(1, 1, 10, 13),
        cairo.RectangleInt(1, 14, 8, 1),
    ])

    c = cairo.Region((b, e))
    c.xor(d)
    assert c.num_rectangles() == 3
    assert c == cairo.Region([
        cairo.RectangleInt(1, 1, 10, 1),
        cairo.RectangleInt(1, 14, 8, 1),
        cairo.RectangleInt(1, 13, 10, 1),
    ])


def test_constants():
    assert cairo.REGION_OVERLAP_IN == 0
    assert cairo.REGION_OVERLAP_OUT == 1
    assert cairo.REGION_OVERLAP_PART == 2

    assert cairo.ANTIALIAS_FAST == 4
    assert cairo.ANTIALIAS_GOOD == 5
    assert cairo.ANTIALIAS_BEST == 6

    assert cairo.OPERATOR_MULTIPLY == 14
    assert cairo.OPERATOR_SCREEN == 15
    assert cairo.OPERATOR_OVERLAY == 16
    assert cairo.OPERATOR_DARKEN == 17
    assert cairo.OPERATOR_LIGHTEN == 18
    assert cairo.OPERATOR_COLOR_DODGE == 19
    assert cairo.OPERATOR_COLOR_BURN == 20
    assert cairo.OPERATOR_HARD_LIGHT == 21
    assert cairo.OPERATOR_SOFT_LIGHT == 22
    assert cairo.OPERATOR_DIFFERENCE == 23
    assert cairo.OPERATOR_EXCLUSION == 24
    assert cairo.OPERATOR_HSL_HUE == 25
    assert cairo.OPERATOR_HSL_SATURATION == 26
    assert cairo.OPERATOR_HSL_COLOR == 27
    assert cairo.OPERATOR_HSL_LUMINOSITY == 28

    assert cairo.FORMAT_INVALID == -1
    assert cairo.FORMAT_RGB30 == 5
