# -*- coding: utf-8 -*-

from __future__ import division
from __future__ import absolute_import
from __future__ import print_function

import os
import io
import sys
import tempfile as tfi
import base64
import zlib
import shutil

import cairo
import pytest
import py.test as test

try:
    long
except NameError:
    long = int


def test_show_unicode_text():
    width, height = 300, 300
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
    ctx = cairo.Context(surface)

    ctx.scale(width, height)
    ctx.set_line_width(0.04)

    ctx.select_font_face(
        "Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_NORMAL)
    ctx.set_font_size(0.20)
    ctx.move_to(0.05, 0.5)
    ctx.show_text(u"ēxāmple.")


def test_unicode_filenames():
    # FIXME: cairo does not support wchar on Windows and byte support is
    # missing under Python 3

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    dirname = tfi.mkdtemp()
    old_dir = os.getcwd()
    try:
        os.chdir(dirname)
        surface.write_to_png("foobar")
        new = cairo.ImageSurface.create_from_png(u"foobar")
        assert surface.get_data() == new.get_data()
    finally:
        os.chdir(old_dir)
        shutil.rmtree(dirname)


def test_scaled_font_get_ctm():
    surface = cairo.ImageSurface(0, 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    matrix = sf.get_ctm()
    assert isinstance(matrix, cairo.Matrix)


def test_scaled_font_get_font_matrix():
    surface = cairo.ImageSurface(0, 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    matrix = sf.get_font_matrix()
    assert isinstance(matrix, cairo.Matrix)


def test_scaled_font_get_font_options():
    surface = cairo.ImageSurface(0, 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    font_options = sf.get_font_options()
    assert isinstance(font_options, cairo.FontOptions)


def test_ps_surface_level_to_string():
    level_id = cairo.PSSurface.level_to_string(cairo.PS_LEVEL_2)
    assert isinstance(level_id, str)
    assert cairo.PSSurface.ps_level_to_string(cairo.PS_LEVEL_2) == level_id


def test_surface_has_show_text_glyphs():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    assert not surface.has_show_text_glyphs()
    surface.finish()
    with pytest.raises(cairo.Error):
        surface.has_show_text_glyphs()


def test_surface_create_for_rectangle():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    new = surface.create_for_rectangle(0, 0, 10, 10)
    assert new
    assert isinstance(new, cairo.Surface)

    with pytest.raises(cairo.Error) as excinfo:
        surface.create_for_rectangle(0, 0, 10, -1)
    assert excinfo.value.status == cairo.STATUS_INVALID_SIZE


def test_context_in_clip():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    context = cairo.Context(surface)
    assert context.in_clip(50, 50)
    context.clip()
    assert not context.in_clip(50, 50)
    context.reset_clip()
    assert context.in_clip(50, 50)


def test_surface_create_similar_image():
    surface = cairo.PDFSurface(None, 1, 1)
    image = surface.create_similar_image(cairo.FORMAT_ARGB32, 24, 42)
    assert image
    assert isinstance(image, cairo.ImageSurface)
    del surface
    assert image.get_width() == 24
    assert image.get_height() == 42


def test_pdf_surface_restrict_to_version():
    surface = cairo.PDFSurface(None, 10, 10)
    surface.restrict_to_version(cairo.PDF_VERSION_1_4)
    surface.finish()
    with pytest.raises(cairo.Error):
        surface.restrict_to_version(cairo.PDF_VERSION_1_5)


def test_pdf_version_to_string():
    ver = cairo.PDFSurface.version_to_string(cairo.PDF_VERSION_1_4)
    assert ver and isinstance(ver, str)
    with pytest.raises(ValueError):
        cairo.PDFSurface.version_to_string(-1)


def test_error_context():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    ctx = cairo.Context(surface)
    with pytest.raises(cairo.Error) as excinfo:
        ctx.restore()
    error = excinfo.value
    assert error.status == cairo.STATUS_INVALID_RESTORE
    assert error.status == cairo.Status.INVALID_RESTORE
    assert isinstance(error.status, cairo.Status)
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


def test_matrix_properties():
    m = cairo.Matrix(*range(6))
    assert [m.xx, m.yx, m.xy, m.yy, m.x0, m.y0] == list(range(6))
    m.xx = 42
    assert m.xx == 42
    m.scale(2, 2)
    assert m.xx == 84


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


def test_pattern_filter():
    pattern = cairo.SolidPattern(1, 2, 3)
    assert pattern.get_filter() == cairo.FILTER_GOOD
    pattern.set_filter(cairo.FILTER_NEAREST)
    assert pattern.get_filter() == cairo.FILTER_NEAREST


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


def test_surface_destroy_before_context():
    for kind in [cairo.PDFSurface, cairo.PSSurface]:
        surface = kind(io.BytesIO(), 1, 1)
        ctx = cairo.Context(surface)
        del surface
        ctx.paint()


def test_surface_destroy_before_surface_pattern():
    surface = cairo.PDFSurface(io.BytesIO(), 1, 1)
    pattern = cairo.SurfacePattern(surface)
    del surface
    ctx = cairo.Context(pattern.get_surface())
    ctx.paint()


def test_recording_surface_get_extents():
    surface = cairo.RecordingSurface(cairo.CONTENT_COLOR, None)
    assert surface.get_extents() is None

    surface = cairo.RecordingSurface(cairo.CONTENT_COLOR, (1, 2, 3, 4))
    assert surface.get_extents() == (1, 2, 3, 4)

    surface = cairo.RecordingSurface(cairo.CONTENT_COLOR, (1, 2, 3, 4))
    surface.finish()
    assert surface.get_extents() == (1, 2, 3, 4)


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

    assert cairo.MIME_TYPE_JPEG == "image/jpeg"

    assert cairo.SVG_VERSION_1_1 == 0
    assert cairo.SVG_VERSION_1_2 == 1


def test_surface_get_set_mime_data():
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    assert surface.get_mime_data("foo") is None
    assert surface.get_mime_data(cairo.MIME_TYPE_JPEG) is None

    surface.set_mime_data("foo", b"bar")
    assert surface.get_mime_data("foo") == b"bar"
    surface.set_mime_data("foo", None)
    assert surface.get_mime_data("foo") is None

    surface.set_mime_data(cairo.MIME_TYPE_JPEG, b"\x00quux\x00")
    assert surface.get_mime_data(cairo.MIME_TYPE_JPEG)[:] == b"\x00quux\x00"
    surface.set_mime_data(cairo.MIME_TYPE_JPEG, None)
    assert surface.get_mime_data(cairo.MIME_TYPE_JPEG) is None


def test_surface_get_set_mime_data_references():
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    if sys.version_info[0] == 2:
        v = buffer(b"bla")
        x = buffer(v, 0, 1)
    else:
        v = memoryview(b"bla")
        x = v[:1]
    assert sys.getrefcount(v) == 2
    assert sys.getrefcount(x) == 2
    surface.set_mime_data("foo", v)
    surface.set_mime_data("foo2", v)
    surface.set_mime_data("foo3", x)
    assert surface.get_mime_data("foo") is v
    assert surface.get_mime_data("foo2") is v
    assert surface.get_mime_data("foo3") is x
    surface.set_mime_data("foo", None)
    surface.set_mime_data("foo2", None)
    surface.set_mime_data("foo3", None)
    surface.finish()
    assert sys.getrefcount(v) == 2
    assert sys.getrefcount(x) == 2


def test_supports_mime_type():
    surface = cairo.PDFSurface(None, 3, 3)
    assert surface.supports_mime_type(cairo.MIME_TYPE_JPEG)
    assert not surface.supports_mime_type("nope")


def test_font_options_copy_equal():
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    font_options = surface.get_font_options()
    font_options.set_hint_metrics(cairo.HINT_METRICS_DEFAULT)
    new = font_options.copy()
    assert font_options.equal(new)
    assert new.get_hint_metrics() == cairo.HINT_METRICS_DEFAULT
    font_options.set_hint_metrics(cairo.HINT_METRICS_ON)
    assert not font_options.equal(new)
    assert new.get_hint_metrics() == cairo.HINT_METRICS_DEFAULT


def test_font_options_hash():
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    font_options = surface.get_font_options()
    assert font_options.hash() == font_options.hash()
    assert isinstance(font_options.hash(), long)


def test_font_options_merge():
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    font_options = surface.get_font_options()
    font_options.set_hint_metrics(cairo.HINT_METRICS_DEFAULT)
    new = font_options.copy()
    new.set_hint_metrics(cairo.HINT_METRICS_ON)
    font_options.merge(new)
    assert font_options.get_hint_metrics() == cairo.HINT_METRICS_ON


def test_font_options_hashable_protocol():
    # make sure __eq__ and __ne__ work
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    font_options = surface.get_font_options()
    assert font_options == font_options.copy()
    assert not font_options != font_options.copy()
    font_options.set_hint_metrics(cairo.HINT_METRICS_DEFAULT)
    different = font_options.copy()
    different.set_hint_metrics(cairo.HINT_METRICS_ON)
    assert font_options != different
    assert not font_options == different
    assert font_options != object()

    # make sure the other operators are undefined
    if sys.version_info[0] == 3:
        with pytest.raises(TypeError):
            font_options < font_options
    assert font_options.__gt__(font_options) is NotImplemented


def test_surface_mime_data_for_pdf():
    jpeg_bytes = zlib.decompress(base64.b64decode(
        b'eJz7f+P/AwYBLzdPNwZGRkYGDyBk+H+bwRnEowj8P8TAzcHACDJHkOH/EQYRIBsV'
        b'cP6/xcDBCBJlrLcHqRBAV8EAVcHIylSPVwGbPQEFjPaK9XDrBAipBSq4CQB9jiS0'
    ))

    file_like = io.BytesIO()
    surface = cairo.PDFSurface(file_like, 3, 3)
    context = cairo.Context(surface)
    image = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    image.set_mime_data(cairo.MIME_TYPE_JPEG, jpeg_bytes)
    context.set_source_surface(image, 0, 0)
    context.paint()
    surface.finish()
    assert jpeg_bytes in file_like.getvalue()


def test_svg_version_to_string():
    ver = cairo.SVGSurface.version_to_string(cairo.SVG_VERSION_1_1)
    assert ver and isinstance(ver, str)
    with pytest.raises(ValueError):
        cairo.SVGSurface.version_to_string(-1)


def test_svg_surface_restrict_to_version():
    surface = cairo.SVGSurface(None, 10, 10)
    surface.restrict_to_version(cairo.SVG_VERSION_1_1)
    surface.finish()
    with pytest.raises(cairo.Error):
        surface.restrict_to_version(cairo.SVG_VERSION_1_2)
