import cairo
import pytest


@pytest.fixture
def context():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 42, 42)
    return cairo.Context(surface)


def text_cmp_hash(context):
    other = cairo.Context(context.get_target())
    assert context != other
    hash(context)


def test_get_antialias(context):
    assert context.get_antialias() == cairo.Antialias.DEFAULT
    assert isinstance(context.get_antialias(), cairo.Antialias)


def test_get_fill_rule(context):
    assert context.get_fill_rule() == cairo.FillRule.WINDING
    assert isinstance(context.get_fill_rule(), cairo.FillRule)


def test_get_line_cap(context):
    assert context.get_line_cap() == cairo.LineCap.BUTT
    assert isinstance(context.get_line_cap(), cairo.LineCap)


def test_get_line_join(context):
    assert context.get_line_join() == cairo.LineJoin.MITER
    assert isinstance(context.get_line_join(), cairo.LineJoin)


def test_get_operator(context):
    assert context.get_operator() == cairo.Operator.OVER
    assert isinstance(context.get_operator(), cairo.Operator)


def test_show_text_glyphs():
    surface = cairo.PDFSurface(None, 300, 300)
    context = cairo.Context(surface)
    context.scale(300, 300)
    context.select_font_face("Sans", cairo.FONT_SLANT_NORMAL,
                             cairo.FONT_WEIGHT_NORMAL)
    context.set_font_size(0.08)
    context.set_line_width(0.09)
    sf = context.get_scaled_font()
    glyphs, clusters, flags = sf.text_to_glyphs(0.5, 0.5, "foobar")
    context.show_text_glyphs("foobar", glyphs, clusters, flags)

    glyphs, clusters, flags = sf.text_to_glyphs(0.5, 0.5, "")
    context.show_text_glyphs("", glyphs, clusters, flags)

    with pytest.raises(TypeError):
        context.show_text_glyphs(object(), glyphs, clusters, flags)

    with pytest.raises(TypeError):
        context.show_text_glyphs("", [object()], clusters, flags)

    with pytest.raises(TypeError):
        context.show_text_glyphs("", object(), clusters, flags)

    with pytest.raises(TypeError):
        context.show_text_glyphs("", glyphs, [object()], flags)

    with pytest.raises(TypeError):
        context.show_text_glyphs("", glyphs, object(), flags)
