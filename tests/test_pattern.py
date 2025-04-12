import cairo
import pytest


def test_raster_source() -> None:
    pattern = cairo.RasterSourcePattern(cairo.Content.COLOR, 2, 2)
    assert isinstance(pattern, cairo.RasterSourcePattern)
    assert issubclass(cairo.RasterSourcePattern, cairo.Pattern)

    with pytest.raises(TypeError):
        cairo.RasterSourcePattern(object())  # type: ignore

    was_called = []

    def acquire_callback(target: cairo.Surface, extents: cairo.RectangleInt) -> cairo.Surface:
        surface = target.create_similar_image(
            cairo.FORMAT_ARGB32, extents.width, extents.height)
        surface.set_device_offset(extents.x, extents.y)
        context = cairo.Context(surface)
        context.set_source_rgb(1, 0, 0)
        context.paint()
        was_called.append("acquire")
        return surface

    def release_callback(surface: cairo.Surface) -> None:
        was_called.append("release")
        return None

    with pytest.raises(TypeError):
        pattern.set_acquire()  # type: ignore

    pattern.set_acquire(None, release_callback)
    assert pattern.get_acquire() == (None, release_callback)

    pattern.set_acquire(acquire_callback, None)
    assert pattern.get_acquire() == (acquire_callback, None)

    pattern.set_acquire(None, None)
    assert pattern.get_acquire() == (None, None)

    pattern.set_acquire(acquire_callback, release_callback)
    assert pattern.get_acquire() == (acquire_callback, release_callback)

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)
    context.set_source(pattern)
    context.paint()
    assert was_called == ["acquire", "release"]

    with pytest.raises(TypeError):
        pattern.set_acquire(None, object())  # type: ignore

    with pytest.raises(TypeError):
        pattern.set_acquire(object(), None)  # type: ignore


def test_cmp_hash() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)
    pattern = context.get_source()
    other = context.get_source()
    assert pattern == other
    assert not pattern != other


def test_get_extend() -> None:
    pattern = cairo.SolidPattern(1, 2, 4)
    assert pattern.get_extend() == cairo.Extend.PAD
    assert isinstance(pattern.get_extend(), cairo.Extend)


def test_get_filter() -> None:
    pattern = cairo.SolidPattern(1, 2, 4)
    assert pattern.get_filter() == cairo.Filter.GOOD


def test_linear_gradient() -> None:
    with pytest.raises(TypeError):
        cairo.LinearGradient()  # type: ignore


def test_radial_gradient() -> None:
    with pytest.raises(TypeError):
        cairo.RadialGradient()  # type: ignore


def test_gradient_get_color_stops() -> None:
    pattern = cairo.LinearGradient(1, 2, 4, 5)
    assert pattern.get_color_stops_rgba() == []
    pattern.add_color_stop_rgb(0.125, 0.25, 0.5, 0.75)
    assert pattern.get_color_stops_rgba() == [(0.125, 0.25, 0.5, 0.75, 1.0)]
    pattern.add_color_stop_rgba(1.0, 0.75, 0.5, 0.25, 0.125)
    assert pattern.get_color_stops_rgba() == \
        [(0.125, 0.25, 0.5, 0.75, 1.0), (1.0, 0.75, 0.5, 0.25, 0.125)]


def test_gradient_add_color_stop_rgb() -> None:
    pattern = cairo.LinearGradient(1, 2, 4, 5)
    with pytest.raises(TypeError):
        pattern.add_color_stop_rgb()  # type: ignore


def test_gradient_add_color_stop_rgba() -> None:
    pattern = cairo.LinearGradient(1, 2, 4, 5)
    with pytest.raises(TypeError):
        pattern.add_color_stop_rgba()  # type: ignore


def test_solid_pattern() -> None:
    with pytest.raises(TypeError):
        cairo.SolidPattern()  # type: ignore


def test_mesh_pattern() -> None:
    mesh = cairo.MeshPattern()
    assert isinstance(mesh, cairo.MeshPattern)
    assert issubclass(cairo.MeshPattern, cairo.Pattern)
    with pytest.raises(TypeError):
        cairo.MeshPattern(object())  # type: ignore


def test_mesh_pattern_example1() -> None:
    pattern = cairo.MeshPattern()
    assert pattern.get_patch_count() == 0

    pattern.begin_patch()
    pattern.move_to(0, 0)
    pattern.curve_to(30, -30, 60, 30, 100, 0)
    pattern.curve_to(60, 30, 130, 60, 100, 100)
    pattern.curve_to(60, 70, 30, 130, 0, 100)
    pattern.curve_to(30, 70, -30, 30, 0, 0)
    pattern.set_corner_color_rgb(0, 1, 0, 0)
    pattern.set_corner_color_rgb(1, 0, 1, 0)
    pattern.set_corner_color_rgb(2, 0, 0, 1)
    pattern.set_corner_color_rgb(3, 1, 1, 0)
    pattern.end_patch()

    assert pattern.get_patch_count() == 1
    assert pattern.get_path(0)
    assert pattern.get_corner_color_rgba(0, 0) == (1, 0, 0, 1)

    with pytest.raises(cairo.Error):
        pattern.get_corner_color_rgba(1, 0)

    with pytest.raises(cairo.Error):
        pattern.get_corner_color_rgba(0, 9)

    with pytest.raises(cairo.Error):
        pattern.get_path(9)


def test_mesh_pattern_curve_to() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.curve_to(object())  # type: ignore


def test_mesh_pattern_get_control_point() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.get_control_point(object())  # type: ignore


def test_mesh_pattern_get_corner_color_rgba() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.get_corner_color_rgba(object())  # type: ignore


def test_mesh_pattern_get_path() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.get_path(object())  # type: ignore


def test_mesh_pattern_line_to() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.line_to(object())  # type: ignore


def test_mesh_pattern_move_to() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.move_to(object())  # type: ignore


def test_mesh_pattern_set_control_point() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.set_control_point(object())  # type: ignore


def test_mesh_pattern_set_corner_color_rgb() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.set_corner_color_rgb(object())  # type: ignore


def test_mesh_pattern_set_corner_color_rgba() -> None:
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.set_corner_color_rgba(object())  # type: ignore


def test_mesh_pattern_example2() -> None:
    pattern = cairo.MeshPattern()
    pattern.begin_patch()
    pattern.move_to(100, 100)
    pattern.line_to(130, 130)
    pattern.line_to(130, 70)
    pattern.set_corner_color_rgb(0, 1, 0, 0)
    pattern.set_corner_color_rgb(1, 0, 1, 0)
    pattern.set_corner_color_rgb(2, 0, 0, 1)
    pattern.end_patch()


def test_mesh_pattern_rest() -> None:
    pattern = cairo.MeshPattern()
    pattern.begin_patch()
    pattern.curve_to(0, 1, 2, 3, 4, 5)
    pattern.set_corner_color_rgba(0, 0.125, 0.25, 0.5, 0.75)
    pattern.set_control_point(0, 10, 12)
    pattern.end_patch()

    assert pattern.get_corner_color_rgba(0, 0) == (0.125, 0.25, 0.5, 0.75)
    assert pattern.get_control_point(0, 0) == (10, 12)

    pattern.get_control_point(0, 0)

    with pytest.raises(cairo.Error):
        pattern.get_control_point(1, 0)

    with pytest.raises(cairo.Error):
        pattern.get_control_point(0, 9)


def test_mesh_pattern_error_states() -> None:
    pattern = cairo.MeshPattern()
    pattern.begin_patch()
    with pytest.raises(cairo.Error):
        pattern.begin_patch()

    with pytest.raises(cairo.Error):
        cairo.MeshPattern().end_patch()

    with pytest.raises(cairo.Error):
        cairo.MeshPattern().curve_to(0, 1, 2, 3, 4, 5)

    with pytest.raises(cairo.Error):
        cairo.MeshPattern().line_to(130, 130)

    with pytest.raises(cairo.Error):
        cairo.MeshPattern().move_to(0, 0)

    with pytest.raises(cairo.Error):
        cairo.MeshPattern().set_corner_color_rgba(0, 0.125, 0.25, 0.5, 0.75)

    with pytest.raises(cairo.Error):
        cairo.MeshPattern().set_corner_color_rgb(0, 0.125, 0.25, 0.5)


def test_get_matrix() -> None:
    pattern = cairo.SolidPattern(1, 2, 4)
    assert isinstance(pattern.get_matrix(), cairo.Matrix)
    pattern.set_matrix(cairo.Matrix())
    with pytest.raises(TypeError):
        pattern.set_matrix(object())  # type: ignore


def test_set_extend() -> None:
    pattern = cairo.SolidPattern(1, 2, 4)
    pattern.set_extend(42)  # type: ignore
    assert pattern.get_extend() == 42
    with pytest.raises(TypeError):
        pattern.set_extend(object())  # type: ignore


def test_set_filter() -> None:
    pattern = cairo.SolidPattern(1, 2, 4)
    with pytest.raises(TypeError):
        pattern.set_filter(object())  # type: ignore


def test_pattern() -> None:
    with pytest.raises(TypeError):
        cairo.Pattern()  # type: ignore

    r, g, b, a = 0.1, 0.2, 0.3, 0.4
    solid = cairo.SolidPattern(r, g, b, a)
    assert solid.get_rgba() == (r, g, b, a)

    assert not solid == object()
    hash(solid)

    with pytest.raises(TypeError):
        cairo.Gradient()  # type: ignore

    x0, y0, x1, y1 = 0.0, 0.0, 0.0, 1.0
    linear = cairo.LinearGradient(x0, y0, x1, y1)
    assert linear.get_linear_points() == (x0, y0, x1, y1)
    linear.add_color_stop_rgba(1, 0, 0, 0, 1)
    linear.add_color_stop_rgba(0, 1, 1, 1, 1)

    cx0, cy0, radius0, cx1, cy1, radius1 = 1.0, 1.0, 1.0, 2.0, 2.0, 1.0
    radial = cairo.RadialGradient(cx0, cy0, radius0, cx1, cy1, radius1)
    assert radial.get_radial_circles() == (cx0, cy0, radius0, cx1, cy1, radius1)
    radial.add_color_stop_rgba(0, 1, 1, 1, 1)
    radial.add_color_stop_rgba(1, 0, 0, 0, 1)


def test_pattern_filter() -> None:
    pattern = cairo.SolidPattern(1, 2, 3)
    assert pattern.get_filter() == cairo.FILTER_GOOD
    pattern.set_filter(cairo.FILTER_NEAREST)
    assert pattern.get_filter() == cairo.FILTER_NEAREST


def test_surface_pattern() -> None:
    with pytest.raises(TypeError):
        cairo.SurfacePattern(object())  # type: ignore


@pytest.mark.skipif(not hasattr(cairo.Pattern, "set_dither"),
                    reason="too old cairo")
def test_pattern_dither() -> None:
    pattern = cairo.SolidPattern(1, 2, 3)
    pattern.get_dither() == cairo.Dither.DEFAULT
    pattern.set_dither(cairo.Dither.BEST)
    assert pattern.get_dither() == cairo.Dither.BEST
