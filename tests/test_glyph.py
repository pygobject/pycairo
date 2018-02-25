import cairo
import pytest


def test_type():
    assert cairo.Glyph
    assert issubclass(cairo.Glyph, tuple)

    with pytest.raises(TypeError):
        cairo.Glyph()

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
        assert g.z

    assert repr(cairo.Glyph(0, 0, 0)) == \
        "cairo.Glyph(index=0, x=0.0, y=0.0)"
    assert str(cairo.Glyph(0, 0, 0)) == \
        "cairo.Glyph(index=0, x=0.0, y=0.0)"
    assert eval(repr(g)) == g

    assert cairo.Glyph.index
    assert cairo.Glyph.x
    assert cairo.Glyph.y


def test_context():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)
    assert context.glyph_extents([(0, 0, 0)])
    context.glyph_path([(0, 0, 0)])
    context.show_glyphs([(0, 0, 0)])

    g = cairo.Glyph(0, 0.5, 0.25)
    assert context.glyph_extents([g])
    context.glyph_path([g])
    context.show_glyphs([(0, 0, 0)])

    with pytest.raises(TypeError):
        context.glyph_path([object()])
