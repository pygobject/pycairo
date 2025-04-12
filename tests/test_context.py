import cairo
import pytest
import ctypes


@pytest.fixture
def context() -> cairo.Context:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 42, 42)
    return cairo.Context(surface)


@pytest.mark.skipif(not hasattr(cairo.Context, "tag_begin"),
                    reason="too old cairo")
def test_tags(context: cairo.Context) -> None:
    context.tag_begin("foo", "bar=quux")
    context.tag_end("foo")


def test_cmp_hash(context: cairo.Context) -> None:
    other = cairo.Context(context.get_target())
    assert context != other
    hash(context)


def test_get_antialias(context: cairo.Context) -> None:
    assert context.get_antialias() == cairo.Antialias.DEFAULT
    assert isinstance(context.get_antialias(), cairo.Antialias)


def test_get_fill_rule(context: cairo.Context) -> None:
    assert context.get_fill_rule() == cairo.FillRule.WINDING
    assert isinstance(context.get_fill_rule(), cairo.FillRule)


def test_get_line_cap(context: cairo.Context) -> None:
    assert context.get_line_cap() == cairo.LineCap.BUTT
    assert isinstance(context.get_line_cap(), cairo.LineCap)


def test_get_line_join(context: cairo.Context) -> None:
    assert context.get_line_join() == cairo.LineJoin.MITER
    assert isinstance(context.get_line_join(), cairo.LineJoin)


def test_get_operator(context: cairo.Context) -> None:
    assert context.get_operator() == cairo.Operator.OVER
    assert isinstance(context.get_operator(), cairo.Operator)


def test_get_set_operator_limits(context: cairo.Context) -> None:
    max_int = 2 ** (ctypes.sizeof(ctypes.c_int()) * 8 - 1) - 1
    min_int = -max_int - 1

    for val in [-1, 0, max_int, min_int]:
        context.set_operator(val)
        assert context.get_operator() == val

    orig = context.get_operator()
    for val in [max_int + 1, min_int - 1]:
        with pytest.raises(OverflowError):
            context.set_operator(val)
        assert context.get_operator() == orig


def test_show_text_glyphs() -> None:
    surface = cairo.PDFSurface(None, 300, 300)
    context = cairo.Context(surface)
    context.scale(300, 300)
    context.select_font_face("Sans", cairo.FONT_SLANT_NORMAL,
                             cairo.FONT_WEIGHT_NORMAL)
    context.set_font_size(0.08)
    context.set_line_width(0.09)
    sf = context.get_scaled_font()
    res = sf.text_to_glyphs(0.5, 0.5, "foobar")
    assert isinstance(res, tuple)
    glyphs, clusters, flags = res
    context.show_text_glyphs("foobar", glyphs, clusters, flags)

    res = sf.text_to_glyphs(0.5, 0.5, "")
    assert isinstance(res, tuple)
    glyphs, clusters, flags = res
    context.show_text_glyphs("", glyphs, clusters, flags)

    with pytest.raises(TypeError):
        context.show_text_glyphs(object(), glyphs, clusters, flags)  # type: ignore

    with pytest.raises(TypeError):
        context.show_text_glyphs("", [object()], clusters, flags)  # type: ignore

    with pytest.raises(TypeError):
        context.show_text_glyphs("", object(), clusters, flags)  # type: ignore

    with pytest.raises(TypeError):
        context.show_text_glyphs("", glyphs, [object()], flags)  # type: ignore

    with pytest.raises(TypeError):
        context.show_text_glyphs("", glyphs, object(), flags)  # type: ignore


def test_append_path(context: cairo.Context) -> None:
    context.line_to(1, 2)
    p = context.copy_path()
    context.new_path()
    context.append_path(p)
    assert str(context.copy_path()) == str(p)
    with pytest.raises(TypeError):
        context.append_path(object())  # type: ignore


def test_arc(context: cairo.Context) -> None:
    assert not list(context.copy_path())
    context.arc(0, 0, 0, 0, 0)
    assert list(context.copy_path())
    with pytest.raises(TypeError):
        context.arc(object())  # type: ignore


def test_arc_negative(context: cairo.Context) -> None:
    assert not list(context.copy_path())
    context.arc_negative(0, 0, 0, 0, 0)
    assert list(context.copy_path())
    with pytest.raises(TypeError):
        context.arc_negative(object())  # type: ignore


def test_clip_extents(context: cairo.Context) -> None:
    assert context.clip_extents() == (0.0, 0.0, 42.0, 42.0)


def test_in_clip() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    context = cairo.Context(surface)
    assert context.in_clip(50, 50)
    context.clip()
    assert not context.in_clip(50, 50)
    context.reset_clip()
    assert context.in_clip(50, 50)

    with pytest.raises(TypeError):
        context.in_clip(None, None)  # type: ignore


def test_device_to_user(context: cairo.Context) -> None:
    assert context.device_to_user(0, 0) == (0, 0)
    with pytest.raises(TypeError):
        context.device_to_user(None, None)  # type: ignore


def test_device_to_user_distance(context: cairo.Context) -> None:
    assert context.device_to_user_distance(0, 0) == (0, 0)
    with pytest.raises(TypeError):
        context.device_to_user_distance(None, None)  # type: ignore


def test_fill_extents(context: cairo.Context) -> None:
    context.line_to(1, 1)
    context.line_to(1, 0)
    context.line_to(0, 0)
    context.line_to(0, 1)
    context.line_to(1, 1)
    assert context.fill_extents() == (0, 0, 1, 1)


def test_curve_to(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.curve_to(1, 2, 3, 4, 5, object())  # type: ignore


def test_set_get_dash(context: cairo.Context) -> None:
    assert context.get_dash() == ((), 0)
    assert context.get_dash_count() == 0

    context.set_dash([0, 1, 2, 3], 10)
    assert context.get_dash() == ((0.0, 1.0, 2.0, 3.0), 4.0)
    assert context.get_dash_count() == 4

    context.set_dash([1], 1.25)
    assert context.get_dash() == ((1.0,), 1.25)

    with pytest.raises(TypeError):
        context.set_dash()  # type: ignore

    with pytest.raises(TypeError):
        context.set_dash(1, 10)  # type: ignore

    with pytest.raises(TypeError):
        context.set_dash([object()], 1)  # type: ignore


def test_glyph_extents(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.glyph_extents(None)  # type: ignore
    with pytest.raises(TypeError):
        context.glyph_extents()  # type: ignore


def test_glyph_path(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.glyph_path(None)  # type: ignore
    with pytest.raises(TypeError):
        context.glyph_path()  # type: ignore


def test_in_stroke(context: cairo.Context) -> None:
    context.line_to(0, 0)
    context.line_to(1, 1)
    assert context.in_stroke(0, 0)
    assert not context.in_stroke(0, 2)
    with pytest.raises(TypeError):
        context.in_stroke(object(), 0)  # type: ignore


def test_current_point(context: cairo.Context) -> None:
    assert not context.has_current_point()
    assert context.get_current_point() == (0, 0)
    context.move_to(10, 10)
    assert context.has_current_point()
    assert context.get_current_point() == (10, 10)


def test_in_fill(context: cairo.Context) -> None:
    assert not context.in_fill(0.1, 0.1)
    with pytest.raises(TypeError):
        context.in_fill(0.1, object())  # type: ignore


def test_line_to(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.line_to(0.1, object())  # type: ignore


def test_mask(context: cairo.Context) -> None:
    pattern = cairo.SolidPattern(0, 0, 0)
    context.mask(pattern)
    with pytest.raises(TypeError):
        context.mask(object())  # type: ignore


def test_mask_surface(context: cairo.Context) -> None:
    context.mask_surface(context.get_target(), 0, 0)
    with pytest.raises(TypeError):
        context.mask_surface(object(), 0, 0)  # type: ignore


def test_paint_with_alpha(context: cairo.Context) -> None:
    context.paint_with_alpha(0.5)
    with pytest.raises(TypeError):
        context.paint_with_alpha(object())  # type: ignore


def test_path_extents(context: cairo.Context) -> None:
    context.line_to(1, 1)
    context.line_to(1, 0)
    context.line_to(0, 0)
    context.line_to(0, 1)
    context.line_to(1, 1)
    assert context.path_extents() == (0.0, 0.0, 1.0, 1.0)


def test_push_pop_group(context: cairo.Context) -> None:
    context.push_group()
    context.pop_group()

    context.push_group()
    context.pop_group_to_source()

    with pytest.raises(TypeError):
        context.push_group_with_content(object())  # type: ignore

    context.push_group_with_content(cairo.Content.COLOR)
    context.pop_group()

    with pytest.raises(cairo.Error):
        context.pop_group()


def test_rectangle(context: cairo.Context) -> None:
    context.rectangle(1, 2, 4, 5)
    assert context.path_extents() == (1.0, 2.0, 5.0, 7.0)
    with pytest.raises(TypeError):
        context.rectangle(1, 2, 3, object())  # type: ignore


def test_rotate(context: cairo.Context) -> None:
    context.rotate(0.3)
    with pytest.raises(TypeError):
        context.rotate(object())  # type: ignore


def test_rel_curve_to(context: cairo.Context) -> None:
    context.line_to(0, 0)
    context.rel_curve_to(0, 0, 0, 0, 0, 0)
    with pytest.raises(TypeError):
        context.rel_curve_to(object(), 0, 0, 0, 0, 0)  # type: ignore


def test_move_to(context: cairo.Context) -> None:
    context.move_to(10, 10)
    assert context.get_current_point() == (10, 10)
    with pytest.raises(TypeError):
        context.move_to(object(), 0)  # type: ignore


def test_rel_line_to(context: cairo.Context) -> None:
    context.line_to(0, 0)
    context.rel_line_to(1, 1)
    with pytest.raises(TypeError):
        context.rel_line_to(object(), 0)  # type: ignore


def test_rel_move_to(context: cairo.Context) -> None:
    context.line_to(0, 0)
    context.rel_move_to(1, 1)
    with pytest.raises(TypeError):
        context.rel_move_to(object(), 0)  # type: ignore


def test_save_restore(context: cairo.Context) -> None:
    context.save()
    context.restore()


def test_scale(context: cairo.Context) -> None:
    context.scale(2, 2)
    with pytest.raises(TypeError):
        context.scale(object(), 0)  # type: ignore


def test_select_font_face(context: cairo.Context) -> None:
    context.select_font_face("")
    with pytest.raises(TypeError):
        context.select_font_face(None)  # type: ignore


def test_set_antialias(context: cairo.Context) -> None:
    context.set_antialias(cairo.Antialias.SUBPIXEL)
    assert context.get_antialias() == cairo.Antialias.SUBPIXEL
    with pytest.raises(TypeError):
        context.set_antialias(object())  # type: ignore


def test_set_fill_rule(context: cairo.Context) -> None:
    context.set_fill_rule(cairo.FillRule.EVEN_ODD)
    assert context.get_fill_rule() == cairo.FillRule.EVEN_ODD
    with pytest.raises(TypeError):
        context.set_fill_rule(object())  # type: ignore


def test_set_font_face(context: cairo.Context) -> None:
    assert context.get_font_face()
    context.set_font_face(None)
    assert context.get_font_face()
    ff = context.get_font_face()
    context.set_font_face(ff)
    assert context.get_font_face() == ff
    with pytest.raises(TypeError):
        context.set_font_face(object())  # type: ignore


def test_set_font_matrix(context: cairo.Context) -> None:
    m = cairo.Matrix()
    context.set_font_matrix(m)
    assert context.get_font_matrix() == m
    with pytest.raises(TypeError):
        context.set_font_matrix(object())  # type: ignore


def test_set_line_cap(context: cairo.Context) -> None:
    context.set_line_cap(cairo.LineCap.SQUARE)
    assert context.get_line_cap() == cairo.LineCap.SQUARE
    with pytest.raises(TypeError):
        context.set_line_cap(object())  # type: ignore


def test_set_line_join(context: cairo.Context) -> None:
    context.set_line_join(cairo.LineJoin.BEVEL)
    assert context.get_line_join() == cairo.LineJoin.BEVEL
    with pytest.raises(TypeError):
        context.set_line_join(object())  # type: ignore


def test_set_line_width(context: cairo.Context) -> None:
    context.set_line_width(42)
    assert context.get_line_width() == 42
    with pytest.raises(TypeError):
        context.set_line_width(object())  # type: ignore


def test_set_matrix(context: cairo.Context) -> None:
    m = cairo.Matrix()
    context.set_matrix(m)
    assert context.get_matrix() == m
    with pytest.raises(TypeError):
        context.set_matrix(object())  # type: ignore


def test_set_miter_limit(context: cairo.Context) -> None:
    context.set_miter_limit(42)
    assert context.get_miter_limit() == 42
    with pytest.raises(TypeError):
        context.set_miter_limit(object())  # type: ignore


def test_set_scaled_font(context: cairo.Context) -> None:
    context.set_scaled_font(context.get_scaled_font())
    with pytest.raises(TypeError):
        context.set_scaled_font(object())  # type: ignore


def test_set_font_options(context: cairo.Context) -> None:
    context.set_font_options(context.get_font_options())
    with pytest.raises(TypeError):
        context.set_font_options(object())  # type: ignore


def test_set_font_size(context: cairo.Context) -> None:
    context.set_font_size(42)
    assert context.get_font_matrix() == cairo.Matrix(42, 0, 0, 42, 0, 0)
    with pytest.raises(TypeError):
        context.set_font_size(object())  # type: ignore


def test_set_source(context: cairo.Context) -> None:
    p = cairo.SolidPattern(0, 0, 0)
    context.set_source(p)
    assert context.get_source() == p
    with pytest.raises(TypeError):
        context.set_source(object())  # type: ignore


def test_set_source_rgb(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.set_source_rgb(1, 1, object())  # type: ignore


def test_get_source_rgba(context: cairo.Context) -> None:
    context.set_source_rgba(1, 1, 1)
    source = context.get_source()
    assert isinstance(source, cairo.SolidPattern)
    assert source.get_rgba() == (1, 1, 1, 1)
    context.set_source_rgba(1, 1, 1, 0.5)
    source = context.get_source()
    assert isinstance(source, cairo.SolidPattern)
    assert source.get_rgba() == (1, 1, 1, 0.5)
    with pytest.raises(TypeError):
        context.set_source_rgba(1, 1, object())  # type: ignore


def test_set_source_surface(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.set_source_surface(object())  # type: ignore


def test_set_tolerance(context: cairo.Context) -> None:
    context.set_tolerance(42)
    assert context.get_tolerance() == 42
    with pytest.raises(TypeError):
        context.set_tolerance(object())  # type: ignore


def test_show_glyphs(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.show_glyphs()  # type: ignore

    with pytest.raises(TypeError):
        context.show_glyphs(object())  # type: ignore

    with pytest.warns(DeprecationWarning, match="num_glyphs.*show_glyphs"):
        # the num_glyphs argument is not annotated
        context.show_glyphs([], 0)  # type: ignore


def test_show_text(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.show_text()  # type: ignore


def test_stroke_extents(context: cairo.Context) -> None:
    assert context.stroke_extents() == (0.0, 0.0, 0.0, 0.0)


def test_text_extents(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        context.text_extents()  # type: ignore


def test_text_path(context: cairo.Context) -> None:
    context.text_path("foo")
    with pytest.raises(TypeError):
        context.text_path(object())  # type: ignore


def test_transform(context: cairo.Context) -> None:
    context.transform(cairo.Matrix())
    with pytest.raises(TypeError):
        context.transform(object())  # type: ignore


def test_translate(context: cairo.Context) -> None:
    context.translate(0.5, 0.5)
    with pytest.raises(TypeError):
        context.translate(0.5, object())  # type: ignore


def test_user_to_device(context: cairo.Context) -> None:
    assert context.user_to_device(0, 0) == (0, 0)
    with pytest.raises(TypeError):
        context.user_to_device(0, object())  # type: ignore


def test_user_to_device_distance(context: cairo.Context) -> None:
    assert context.user_to_device_distance(0, 0) == (0, 0)
    with pytest.raises(TypeError):
        context.user_to_device_distance(0, object())  # type: ignore


def test_context(context: cairo.Context) -> None:
    with pytest.raises(TypeError):
        cairo.Context(None)  # type: ignore

    assert not context == object()


def test_simple(context: cairo.Context) -> None:
    context.clip_preserve()
    context.copy_page()
    context.copy_path_flat()
    context.fill()
    context.fill_preserve()
    context.font_extents()
    context.identity_matrix()
    context.new_sub_path()
    context.show_page()
    context.stroke_preserve()

    assert context.get_dash_count() == 0
    assert isinstance(context.get_font_matrix(), cairo.Matrix)

    assert context.get_group_target()
    context.get_line_width()

    assert isinstance(context.get_tolerance(), float)
    assert isinstance(context.get_miter_limit(), float)
    assert isinstance(context.get_matrix(), cairo.Matrix)


@pytest.mark.skipif(not hasattr(cairo.Context, "set_hairline"),
                    reason="too old cairo")
def test_hairline(context: cairo.Context) -> None:
    assert not context.get_hairline()
    context.set_hairline(True)
    assert isinstance(context.get_hairline(), bool)
    assert context.get_hairline()
