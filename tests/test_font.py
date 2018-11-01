import sys
import platform

import cairo
import pytest


try:
    long
except NameError:
    long = int


@pytest.fixture
def font_options():
    surface = cairo.ImageSurface(0, 10, 10)
    return surface.get_font_options()


@pytest.fixture
def font_face():
    surface = cairo.ImageSurface(0, 10, 10)
    context = cairo.Context(surface)
    return context.get_font_face()


@pytest.fixture
def scaled_font(font_face, font_options):
    return cairo.ScaledFont(
        font_face, cairo.Matrix(), cairo.Matrix(), font_options)


@pytest.mark.skipif(not hasattr(cairo.FontOptions, "get_variations"),
                    reason="too old cairo")
def test_font_options_variations(font_options):
    assert font_options.get_variations() is None
    font_options.set_variations("foo")
    assert font_options.get_variations() == "foo"
    font_options.set_variations(None)
    assert font_options.get_variations() is None

    with pytest.raises(TypeError):
        font_options.set_variations(1)

    with pytest.raises(TypeError):
        font_options.set_variations("foo", "bar")

    with pytest.raises(TypeError):
        font_options.set_variations()


def test_font_options():
    assert isinstance(cairo.FontOptions(), cairo.FontOptions)
    with pytest.raises(TypeError):
        cairo.FontOptions(object())


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
    with pytest.raises(TypeError):
        font_options.equal(object())


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
    with pytest.raises(TypeError):
        font_options.merge(object())


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


def test_font_options_set_antialias(font_options):
    font_options.set_antialias(cairo.Antialias.GRAY)
    assert font_options.get_antialias() == cairo.Antialias.GRAY
    with pytest.raises(TypeError):
        font_options.set_antialias(object())


def test_font_options_set_hint_metrics(font_options):
    font_options.set_hint_metrics(cairo.HintMetrics.OFF)
    assert font_options.get_hint_metrics() == cairo.HintMetrics.OFF
    with pytest.raises(TypeError):
        font_options.set_hint_metrics(object())


def test_font_options_set_hint_style(font_options):
    font_options.set_hint_style(cairo.HintStyle.SLIGHT)
    assert font_options.get_hint_style() == cairo.HintStyle.SLIGHT
    with pytest.raises(TypeError):
        font_options.set_hint_style(object())


def test_font_options_set_subpixel_order(font_options):
    font_options.set_subpixel_order(cairo.SubpixelOrder.VRGB)
    assert font_options.get_subpixel_order() == cairo.SubpixelOrder.VRGB
    with pytest.raises(TypeError):
        font_options.set_subpixel_order(object())


def test_font_face(font_face):
    with pytest.raises(TypeError):
        cairo.FontFace()

    assert font_face == font_face
    assert font_face != object()


def test_font_face_cmp_hash():
    surface = cairo.ImageSurface(0, 10, 10)
    context = cairo.Context(surface)
    ff = context.get_font_face()
    other = context.get_font_face()
    assert ff == other
    assert not ff != other
    assert hash(ff) == hash(other)

    sf = context.get_scaled_font()
    other = context.get_scaled_font()
    assert sf == other
    assert not sf != other
    assert hash(sf) == hash(other)

    fo = context.get_font_options()
    # FontOptions compare by their content and they are mutable, so not
    # hashable.
    with pytest.raises(TypeError):
        hash(fo)


def test_scaled_font(scaled_font):
    with pytest.raises(TypeError):
        cairo.ScaledFont()

    assert scaled_font == scaled_font
    assert scaled_font != object()


def test_scaled_font_extents(scaled_font):
    assert isinstance(scaled_font.extents(), tuple)


def test_scaled_font_get_font_face(scaled_font):
    assert isinstance(scaled_font.get_font_face(), cairo.FontFace)


def test_scaled_font_get_scale_matrix(scaled_font):
    assert isinstance(scaled_font.get_scale_matrix(), cairo.Matrix)


# https://bitbucket.org/pypy/pypy/issues/2741
@pytest.mark.skipif(platform.python_implementation() == "PyPy", reason="PyPy")
def test_scaled_font_text_extents(scaled_font):
    with pytest.raises(TypeError):
        scaled_font.text_extents(object())


def test_scaled_font_glyph_extents(scaled_font):
    with pytest.raises(TypeError):
        scaled_font.glyph_extents(object())
    with pytest.raises(TypeError):
        scaled_font.glyph_extents([object()])
    with pytest.raises(TypeError):
        scaled_font.glyph_extents()


# https://bitbucket.org/pypy/pypy/issues/2741
@pytest.mark.skipif(platform.python_implementation() == "PyPy", reason="PyPy")
def test_toy_font_face():
    with pytest.raises(TypeError):
        cairo.ToyFontFace(object())


def test_toy_font_get_family():
    font_face = cairo.ToyFontFace("")
    assert isinstance(font_face.get_family(), str)


def test_toy_font_get_slant():
    font_face = cairo.ToyFontFace("")
    assert font_face.get_slant() == cairo.FontSlant.NORMAL
    assert isinstance(font_face.get_slant(), cairo.FontSlant)


def test_toy_font_get_weight():
    font_face = cairo.ToyFontFace("")
    assert font_face.get_weight() == cairo.FontWeight.NORMAL
    assert isinstance(font_face.get_weight(), cairo.FontWeight)


def test_font_options_get_antialias(font_options):
    assert font_options.get_antialias() == cairo.Antialias.DEFAULT
    assert isinstance(font_options.get_antialias(), cairo.Antialias)


def test_font_options_get_hint_metrics(font_options):
    assert font_options.get_hint_metrics() == cairo.HintMetrics.ON
    assert isinstance(font_options.get_hint_metrics(), cairo.HintMetrics)


def test_font_options_get_hint_style(font_options):
    assert font_options.get_hint_style() == cairo.HintStyle.DEFAULT
    assert isinstance(font_options.get_hint_style(), cairo.HintStyle)


def test_font_options_get_subpixel_order(font_options):
    assert font_options.get_subpixel_order() == cairo.SubpixelOrder.DEFAULT
    assert isinstance(font_options.get_subpixel_order(), cairo.SubpixelOrder)


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


def test_scaled_font_text_to_glyphs():
    surface = cairo.ImageSurface(0, 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    assert sf.text_to_glyphs(0, 0, u"") == ([], [], 0)
    glyphs, clusters, flags = sf.text_to_glyphs(0, 0, u"a")
    assert sf.text_to_glyphs(0, 0, u"a", True) == (glyphs, clusters, flags)
    assert len(glyphs) == 1
    assert isinstance(glyphs[0], cairo.Glyph)
    assert len(clusters) == 1
    assert isinstance(clusters[0], cairo.TextCluster)
    assert flags == 0
    assert sf.text_to_glyphs(0, 0, u"a", False) == glyphs
    glyphs, clusters, flags = sf.text_to_glyphs(0, 0, u"a b")
    assert len(glyphs) == 3
    assert glyphs[0] != glyphs[1]
    assert len(clusters) == 3

    with pytest.raises(TypeError):
        sf.text_to_glyphs(object())
