import sys
import ctypes

import cairo
import pytest


def test_type() -> None:
    assert hasattr(cairo, "Glyph")
    assert issubclass(cairo.Glyph, tuple)

    with pytest.raises(TypeError):
        cairo.Glyph()  # type: ignore

    g = cairo.Glyph(0, 0.5, 0.25)
    assert hash(g) == hash(cairo.Glyph(0, 0.5, 0.25))
    assert isinstance(g, tuple)
    assert g == (0, 0.5, 0.25)
    assert g == cairo.Glyph(0, 0.5, 0.25)
    assert g[1] == 0.5
    assert g.index == 0
    assert g.x == 0.5
    assert g.y == 0.25

    with pytest.raises(AttributeError):
        assert g.z  # type: ignore

    assert repr(cairo.Glyph(0, 0, 0)) == \
        "cairo.Glyph(index=0, x=0.0, y=0.0)"
    assert str(cairo.Glyph(0, 0, 0)) == \
        "cairo.Glyph(index=0, x=0.0, y=0.0)"
    assert eval(repr(g)) == g

    assert cairo.Glyph.index
    assert cairo.Glyph.x
    assert cairo.Glyph.y


def test_context() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)

    # We allow tuple[int, float, float] too, but don't expose
    # in the the annotations
    assert context.glyph_extents([(0, 0, 0)])  # type: ignore
    context.glyph_path([(0, 0, 0)])  # type: ignore
    context.show_glyphs([(0, 0, 0)])  # type: ignore

    g = cairo.Glyph(0, 0.5, 0.25)
    assert context.glyph_extents([g])
    context.glyph_path([g])
    context.show_glyphs([cairo.Glyph(0, 0, 0)])

    with pytest.warns(DeprecationWarning, match="num_glyphs.*glyph_extents"):
        context.glyph_extents([], 0)  # type: ignore

    with pytest.warns(DeprecationWarning, match="num_glyphs.*glyph_path"):
        context.glyph_path([], 0)  # type: ignore

    with pytest.raises(TypeError):
        context.glyph_path([object()])  # type: ignore


def test_glyph_limits() -> None:
    max_ulong = 2 ** (ctypes.sizeof(ctypes.c_ulong()) * 8) - 1

    g = cairo.Glyph(max_ulong, sys.float_info.max, -sys.float_info.max)
    assert g.index == max_ulong
    assert g.x == sys.float_info.max
    assert g.y == -sys.float_info.max

    with pytest.raises(OverflowError):
        cairo.Glyph(-1, 0, 0)

    with pytest.raises(OverflowError):
        g = cairo.Glyph(max_ulong + 1, 0, 0)
