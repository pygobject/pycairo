import os
import io
import sys
import tempfile
import base64
import zlib
import shutil

import cairo
import pytest


def test_get_include() -> None:
    include = cairo.get_include()
    assert isinstance(include, str)
    assert os.path.exists(include)
    assert os.path.isdir(include)


def test_version() -> None:
    cairo.cairo_version()
    cairo.cairo_version_string()

    assert cairo.CAIRO_VERSION == cairo.cairo_version()
    assert cairo.CAIRO_VERSION_STRING == cairo.cairo_version_string()
    ver_tuple = (cairo.CAIRO_VERSION_MAJOR, cairo.CAIRO_VERSION_MINOR,
                 cairo.CAIRO_VERSION_MICRO)
    assert tuple(map(int, cairo.CAIRO_VERSION_STRING.split("."))) == \
        ver_tuple


def test_show_unicode_text() -> None:
    width, height = 300, 300
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
    ctx = cairo.Context(surface)

    ctx.scale(width, height)
    ctx.set_line_width(0.04)

    ctx.select_font_face(
        "Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_NORMAL)
    ctx.set_font_size(0.20)
    ctx.move_to(0.05, 0.5)
    ctx.show_text("ēxāmple.")


def test_unicode_filenames() -> None:
    # FIXME: cairo does not support wchar on Windows and byte support is
    # missing under Python 3

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    dirname = tempfile.mkdtemp()
    old_dir = os.getcwd()
    try:
        os.chdir(dirname)
        surface.write_to_png("foobar")
        new = cairo.ImageSurface.create_from_png("foobar")
        assert surface.get_data() == new.get_data()
    finally:
        os.chdir(old_dir)
        shutil.rmtree(dirname)


def test_surface_has_show_text_glyphs() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    assert not surface.has_show_text_glyphs()
    surface.finish()
    with pytest.raises(cairo.Error):
        surface.has_show_text_glyphs()


def test_context() -> None:
    f, w, h = cairo.FORMAT_ARGB32, 100, 100
    s = cairo.ImageSurface(f, w, h)
    ctx = cairo.Context(s)
    ctx.set_source_rgb(1.0, 1.0, 1.0)
    ctx.set_operator(cairo.OPERATOR_SOURCE)
    ctx.paint()


def test_surface() -> None:
    # TypeError: The Surface type cannot be instantiated
    with pytest.raises(TypeError):
        cairo.Surface()

    format, w, h = cairo.FORMAT_ARGB32, 100, 100
    s = cairo.ImageSurface(format, w, h)
    assert s.get_format() == format
    assert s.get_width() == w
    assert s.get_height() == h

    with tempfile.TemporaryFile(mode='w+b') as f:
        cairo.PDFSurface(f, 100, 100)

    with tempfile.TemporaryFile(mode='w+b') as f:
        cairo.PSSurface(f, 100, 100)

    cairo.RecordingSurface(cairo.CONTENT_COLOR, None)
    cairo.RecordingSurface(cairo.CONTENT_COLOR, cairo.Rectangle(1, 1, 10, 10))
    cairo.RecordingSurface(cairo.CONTENT_COLOR, (1, 1, 10, 10))  # type: ignore

    with tempfile.TemporaryFile(mode='w+b') as f:
        cairo.SVGSurface(f, 100, 100)


def test_surface_destroy_before_context() -> None:
    for kind in [cairo.PDFSurface, cairo.PSSurface]:
        surface = kind(io.BytesIO(), 1, 1)
        ctx = cairo.Context(surface)
        del surface
        ctx.paint()


def test_surface_destroy_before_surface_pattern() -> None:
    surface = cairo.PDFSurface(io.BytesIO(), 1, 1)
    pattern = cairo.SurfacePattern(surface)
    del surface
    ctx = cairo.Context(pattern.get_surface())
    ctx.paint()


def test_recording_surface_get_extents() -> None:
    surface = cairo.RecordingSurface(cairo.CONTENT_COLOR, None)
    assert surface.get_extents() is None

    surface = cairo.RecordingSurface(cairo.CONTENT_COLOR, (1, 2, 3, 4))  # type: ignore
    assert surface.get_extents() == (1, 2, 3, 4)

    surface = cairo.RecordingSurface(cairo.CONTENT_COLOR, (1, 2, 3, 4))  # type: ignore
    surface.finish()
    assert surface.get_extents() == (1, 2, 3, 4)


def test_image_surface_get_data() -> None:
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
    ctx.set_source_rgba(1, 1, 1, 1)
    ctx.paint()
    surface.flush()
    assert newbuf[0:1] == b"\xff"


def test_surface_file_obj_error() -> None:

    class Fail:

        def write(self, data: bytes) -> int:
            if data:
                raise OSError
            return 0

    # We don't have these annotated to accept any object with write()
    # method, so we need to ignore the type error.
    cairo.PDFSurface(Fail(), 100, 100)  # type: ignore
    cairo.PSSurface(Fail(), 100, 100)  # type: ignore


def test_region() -> None:
    a = cairo.Region()
    assert a.is_empty() is True
    assert a.num_rectangles() == 0

    b = cairo.RectangleInt(1, 2, 10, 12)
    with pytest.raises(TypeError):
        hash(b)
    assert repr(b) == "cairo.RectangleInt(x=1, y=2, width=10, height=12)"
    assert eval(repr(b)) == b
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

    r = cairo.Region(d)
    c = cairo.Region((b, e))
    c.subtract(r)
    assert c.num_rectangles() == 2
    assert c.get_rectangle(0) == cairo.RectangleInt(1, 13, 10, 1)

    c = cairo.Region((b, e))
    c.union(r)
    assert c.num_rectangles() == 2
    assert c == cairo.Region([
        cairo.RectangleInt(1, 1, 10, 13),
        cairo.RectangleInt(1, 14, 8, 1),
    ])

    c = cairo.Region((b, e))
    c.xor(r)
    assert c.num_rectangles() == 3
    assert c == cairo.Region([
        cairo.RectangleInt(1, 1, 10, 1),
        cairo.RectangleInt(1, 14, 8, 1),
        cairo.RectangleInt(1, 13, 10, 1),
    ])


def test_constants() -> None:
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


@pytest.mark.skipif(not hasattr(sys, "getrefcount"), reason="PyPy")
def test_surface_get_set_mime_data_references() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    v = memoryview(b"bla")
    x = v[:1]
    recfcount_v = sys.getrefcount(v)
    recfcount_x = sys.getrefcount(x)
    assert recfcount_v == recfcount_x
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
    assert sys.getrefcount(v) == recfcount_v
    assert sys.getrefcount(x) == recfcount_x


def test_surface_mime_data_for_pdf() -> None:
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
