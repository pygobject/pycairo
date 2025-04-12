import cairo
import pytest


@pytest.fixture
def font_options() -> cairo.FontOptions:
    surface = cairo.ImageSurface(cairo.Format(0), 10, 10)
    return surface.get_font_options()


@pytest.fixture
def font_face() -> cairo.FontFace:
    surface = cairo.ImageSurface(cairo.Format(0), 10, 10)
    context = cairo.Context(surface)
    return context.get_font_face()


@pytest.fixture
def scaled_font(font_face: cairo.FontFace, font_options: cairo.FontOptions) -> cairo.ScaledFont:
    return cairo.ScaledFont(
        font_face, cairo.Matrix(), cairo.Matrix(), font_options)


@pytest.mark.skipif(not hasattr(cairo.FontOptions, "set_custom_palette_color"),
                    reason="too old cairo")
def test_font_options_custom_palette_color(font_options: cairo.FontOptions) -> None:
    font_options.set_custom_palette_color(42, 0.25, 0.5, 0.75, 1.0)
    with pytest.raises(cairo.Error) as exc_info:
        font_options.get_custom_palette_color(24)
    assert exc_info.value.status == cairo.Status.INVALID_INDEX
    assert font_options.get_custom_palette_color(42) == (0.25, 0.5, 0.75, 1.0)
    assert isinstance(font_options.get_custom_palette_color(42), tuple)


@pytest.mark.skipif(not hasattr(cairo.FontOptions, "set_color_mode"),
                    reason="too old cairo")
def test_font_options_set_color_mode(font_options: cairo.FontOptions) -> None:
    font_options.set_color_mode(cairo.ColorMode.COLOR)
    assert font_options.get_color_mode() == cairo.ColorMode.COLOR
    with pytest.raises(TypeError):
        font_options.set_color_mode(object())  # type: ignore


@pytest.mark.skipif(not hasattr(cairo.FontOptions, "get_color_mode"),
                    reason="too old cairo")
def test_font_options_get_color_mode(font_options: cairo.FontOptions) -> None:
    assert font_options.get_color_mode() == cairo.ColorMode.DEFAULT
    assert isinstance(font_options.get_color_mode(), cairo.ColorMode)


@pytest.mark.skipif(not hasattr(cairo.FontOptions, "set_color_palette"),
                    reason="too old cairo")
def test_font_options_set_color_palette(font_options: cairo.FontOptions) -> None:
    font_options.set_color_palette(42)
    assert font_options.get_color_palette() == 42


@pytest.mark.skipif(not hasattr(cairo.FontOptions, "get_color_palette"),
                    reason="too old cairo")
def test_font_options_get_color_palette(font_options: cairo.FontOptions) -> None:
    assert font_options.get_color_palette() == cairo.COLOR_PALETTE_DEFAULT


@pytest.mark.skipif(not hasattr(cairo.FontOptions, "get_variations"),
                    reason="too old cairo")
def test_font_options_variations(font_options: cairo.FontOptions) -> None:
    assert font_options.get_variations() is None
    font_options.set_variations("foo")
    assert font_options.get_variations() == "foo"
    font_options.set_variations(None)
    assert font_options.get_variations() is None

    with pytest.raises(TypeError):
        font_options.set_variations(1)  # type: ignore

    with pytest.raises(TypeError):
        font_options.set_variations("foo", "bar")  # type: ignore

    with pytest.raises(TypeError):
        font_options.set_variations()  # type: ignore


def test_font_options() -> None:
    assert isinstance(cairo.FontOptions(), cairo.FontOptions)
    with pytest.raises(TypeError):
        cairo.FontOptions(object())  # type: ignore


def test_font_options_copy_equal() -> None:
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
        font_options.equal(object())  # type: ignore


def test_font_options_hash() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    font_options = surface.get_font_options()
    assert font_options.hash() == font_options.hash()
    assert isinstance(font_options.hash(), int)


def test_font_options_merge() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 1, 1)
    font_options = surface.get_font_options()
    font_options.set_hint_metrics(cairo.HINT_METRICS_DEFAULT)
    new = font_options.copy()
    new.set_hint_metrics(cairo.HINT_METRICS_ON)
    font_options.merge(new)
    assert font_options.get_hint_metrics() == cairo.HINT_METRICS_ON
    with pytest.raises(TypeError):
        font_options.merge(object())  # type: ignore


def test_font_options_hashable_protocol() -> None:
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
    with pytest.raises(TypeError):
        font_options < font_options  # type: ignore
    assert font_options.__gt__(font_options) is NotImplemented  # type: ignore


def test_font_options_set_antialias(font_options: cairo.FontOptions) -> None:
    font_options.set_antialias(cairo.Antialias.GRAY)
    assert font_options.get_antialias() == cairo.Antialias.GRAY
    with pytest.raises(TypeError):
        font_options.set_antialias(object())  # type: ignore


def test_font_options_set_hint_metrics(font_options: cairo.FontOptions) -> None:
    font_options.set_hint_metrics(cairo.HintMetrics.OFF)
    assert font_options.get_hint_metrics() == cairo.HintMetrics.OFF
    with pytest.raises(TypeError):
        font_options.set_hint_metrics(object())  # type: ignore


def test_font_options_set_hint_style(font_options: cairo.FontOptions) -> None:
    font_options.set_hint_style(cairo.HintStyle.SLIGHT)
    assert font_options.get_hint_style() == cairo.HintStyle.SLIGHT
    with pytest.raises(TypeError):
        font_options.set_hint_style(object())  # type: ignore


def test_font_options_set_subpixel_order(font_options: cairo.FontOptions) -> None:
    font_options.set_subpixel_order(cairo.SubpixelOrder.VRGB)
    assert font_options.get_subpixel_order() == cairo.SubpixelOrder.VRGB
    with pytest.raises(TypeError):
        font_options.set_subpixel_order(object())  # type: ignore


def test_font_face(font_face: cairo.FontFace) -> None:
    with pytest.raises(TypeError):
        cairo.FontFace()  # type: ignore

    assert font_face == font_face
    assert font_face != object()


def test_font_face_cmp_hash() -> None:
    surface = cairo.ImageSurface(cairo.Format(0), 10, 10)
    context = cairo.Context(surface)
    ff = context.get_font_face()
    other = context.get_font_face()
    assert ff == other
    assert not ff != other
    assert hash(ff) == hash(other)

    sf = context.get_scaled_font()
    other_sf = context.get_scaled_font()
    assert sf == other_sf
    assert not sf != other_sf
    assert hash(sf) == hash(other_sf)

    fo = context.get_font_options()
    # FontOptions compare by their content and they are mutable, so not
    # hashable.
    with pytest.raises(TypeError):
        hash(fo)  # type: ignore


def test_scaled_font(scaled_font: cairo.ScaledFont) -> None:
    with pytest.raises(TypeError):
        cairo.ScaledFont()  # type: ignore

    assert scaled_font == scaled_font
    assert scaled_font != object()


def test_scaled_font_extents(scaled_font: cairo.ScaledFont) -> None:
    assert isinstance(scaled_font.extents(), tuple)


def test_scaled_font_get_font_face(scaled_font: cairo.ScaledFont) -> None:
    assert isinstance(scaled_font.get_font_face(), cairo.FontFace)


def test_scaled_font_get_scale_matrix(scaled_font: cairo.ScaledFont) -> None:
    assert isinstance(scaled_font.get_scale_matrix(), cairo.Matrix)


def test_scaled_font_text_extents(scaled_font: cairo.ScaledFont) -> None:
    with pytest.raises(TypeError):
        scaled_font.text_extents(object())  # type: ignore


def test_scaled_font_glyph_extents(scaled_font: cairo.ScaledFont) -> None:

    scaled_font.glyph_extents([cairo.Glyph(0, 0.5, 0.25)])

    with pytest.warns(DeprecationWarning, match="num_glyphs.*glyph_extents"):
        scaled_font.glyph_extents([], 0)  # type: ignore

    with pytest.raises(TypeError):
        scaled_font.glyph_extents(object())  # type: ignore
    with pytest.raises(TypeError):
        scaled_font.glyph_extents([object()])  # type: ignore
    with pytest.raises(TypeError):
        scaled_font.glyph_extents()  # type: ignore


def test_toy_font_face() -> None:
    with pytest.raises(TypeError):
        cairo.ToyFontFace(object())  # type: ignore


def test_toy_font_get_family() -> None:
    font_face = cairo.ToyFontFace("")
    assert isinstance(font_face.get_family(), str)
    font_face = cairo.ToyFontFace("serif")
    assert isinstance(font_face.get_family(), str)
    font_face = cairo.ToyFontFace("sans-serif")
    assert isinstance(font_face.get_family(), str)


def test_toy_font_get_slant() -> None:
    font_face = cairo.ToyFontFace("")
    assert font_face.get_slant() == cairo.FontSlant.NORMAL
    assert isinstance(font_face.get_slant(), cairo.FontSlant)


def test_toy_font_get_weight() -> None:
    font_face = cairo.ToyFontFace("")
    assert font_face.get_weight() == cairo.FontWeight.NORMAL
    assert isinstance(font_face.get_weight(), cairo.FontWeight)


def test_font_options_get_antialias(font_options: cairo.FontOptions) -> None:
    assert font_options.get_antialias() == cairo.Antialias.DEFAULT
    assert isinstance(font_options.get_antialias(), cairo.Antialias)


def test_font_options_get_hint_metrics(font_options: cairo.FontOptions) -> None:
    assert font_options.get_hint_metrics() == cairo.HintMetrics.ON
    assert isinstance(font_options.get_hint_metrics(), cairo.HintMetrics)


def test_font_options_get_hint_style(font_options: cairo.FontOptions) -> None:
    assert font_options.get_hint_style() == cairo.HintStyle.DEFAULT
    assert isinstance(font_options.get_hint_style(), cairo.HintStyle)


def test_font_options_get_subpixel_order(font_options: cairo.FontOptions) -> None:
    assert font_options.get_subpixel_order() == cairo.SubpixelOrder.DEFAULT
    assert isinstance(font_options.get_subpixel_order(), cairo.SubpixelOrder)


def test_scaled_font_get_ctm() -> None:
    surface = cairo.ImageSurface(cairo.Format(0), 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    matrix = sf.get_ctm()
    assert isinstance(matrix, cairo.Matrix)


def test_scaled_font_get_font_matrix() -> None:
    surface = cairo.ImageSurface(cairo.Format(0), 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    matrix = sf.get_font_matrix()
    assert isinstance(matrix, cairo.Matrix)


def test_scaled_font_get_font_options() -> None:
    surface = cairo.ImageSurface(cairo.Format(0), 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    font_options = sf.get_font_options()
    assert isinstance(font_options, cairo.FontOptions)


def test_scaled_font_text_to_glyphs() -> None:
    surface = cairo.ImageSurface(cairo.Format(0), 10, 10)
    ctx = cairo.Context(surface)
    sf = ctx.get_scaled_font()
    assert sf.text_to_glyphs(0, 0, "") == ([], [], 0)
    glyphs, clusters, flags = sf.text_to_glyphs(0, 0, "a")
    assert sf.text_to_glyphs(0, 0, "a", True) == (glyphs, clusters, flags)
    assert len(glyphs) == 1
    assert isinstance(glyphs[0], cairo.Glyph)
    assert len(clusters) == 1
    assert isinstance(clusters[0], cairo.TextCluster)
    assert flags == 0
    assert sf.text_to_glyphs(0, 0, "a", False) == glyphs
    glyphs, clusters, flags = sf.text_to_glyphs(0, 0, "a b")
    assert len(glyphs) == 3
    assert glyphs[0] != glyphs[1]
    assert len(clusters) == 3

    with pytest.raises(TypeError):
        sf.text_to_glyphs(object())  # type: ignore
