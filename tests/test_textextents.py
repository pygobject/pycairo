import sys

import cairo
import pytest


def test_type() -> None:
    assert hasattr(cairo, "TextExtents")
    assert issubclass(cairo.TextExtents, tuple)

    with pytest.raises(TypeError):
        cairo.TextExtents()  # type: ignore

    r = cairo.TextExtents(0.0, 0.5, 0.25, 0.75, 0.5, 0.125)
    assert hash(r) == hash(cairo.TextExtents(0.0, 0.5, 0.25, 0.75, 0.5, 0.125))
    assert isinstance(r, tuple)
    assert r == (0.0, 0.5, 0.25, 0.75, 0.5, 0.125)
    assert r == cairo.TextExtents(0.0, 0.5, 0.25, 0.75, 0.5, 0.125)
    assert r[1] == 0.5
    assert r.width == 0.25
    assert r.height == 0.75
    assert r.x_bearing == 0.0
    assert r.y_bearing == 0.5

    with pytest.raises(AttributeError):
        assert r.z  # type: ignore

    assert repr(r) == \
        "cairo.TextExtents(x_bearing=0.0, y_bearing=0.5, " \
        "width=0.25, height=0.75, x_advance=0.5, " \
        "y_advance=0.125)"
    assert str(r) == repr(r)
    assert eval(repr(r)) == r

    assert cairo.TextExtents.x_bearing
    assert cairo.TextExtents.y_bearing
    assert cairo.TextExtents.width
    assert cairo.TextExtents.height
    assert cairo.TextExtents.x_advance
    assert cairo.TextExtents.y_advance


def test_methods() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)

    extents = context.glyph_extents([cairo.Glyph(0, 0, 0)])
    assert isinstance(extents, cairo.TextExtents)

    extents = context.text_extents("foo")
    assert isinstance(extents, cairo.TextExtents)

    sf = context.get_scaled_font()
    extents = sf.text_extents("foo")
    assert isinstance(extents, cairo.TextExtents)

    extents = sf.glyph_extents([cairo.Glyph(0, 0, 0)])
    assert isinstance(extents, cairo.TextExtents)


def test_text_extents_limits() -> None:
    max_val = sys.float_info.max
    min_val = sys.float_info.min

    te = cairo.TextExtents(max_val, min_val, max_val, min_val,
                           max_val, min_val)
    assert te.x_bearing == max_val
    assert te.y_bearing == min_val
    assert te.width == max_val
    assert te.height == min_val
    assert te.x_advance == max_val
    assert te.y_advance == min_val
