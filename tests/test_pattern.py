import cairo
import pytest


def test_raster_source():
    pattern = cairo.RasterSourcePattern(cairo.Content.COLOR, 2, 2)
    assert isinstance(pattern, cairo.RasterSourcePattern)
    assert issubclass(cairo.RasterSourcePattern, cairo.Pattern)

    with pytest.raises(TypeError):
        cairo.RasterSourcePattern(object())

    was_called = []

    def acquire_callback(target, extents):
        surface = target.create_similar_image(
            cairo.FORMAT_ARGB32, extents.width, extents.height)
        surface.set_device_offset(extents.x, extents.y)
        context = cairo.Context(surface)
        context.set_source_rgb(1, 0, 0)
        context.paint()
        was_called.append("acquire")
        return surface

    def release_callback(surface):
        was_called.append("release")
        return None

    with pytest.raises(TypeError):
        pattern.set_acquire()

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
        pattern.set_acquire(None, object())

    with pytest.raises(TypeError):
        pattern.set_acquire(object(), None)


def test_cmp_hash():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)
    pattern = context.get_source()
    other = context.get_source()
    assert pattern == other
    assert not pattern != other


def test_get_extend():
    pattern = cairo.SolidPattern(1, 2, 4)
    assert pattern.get_extend() == cairo.Extend.PAD
    assert isinstance(pattern.get_extend(), cairo.Extend)


def test_get_filter():
    pattern = cairo.SolidPattern(1, 2, 4)
    assert pattern.get_filter() == cairo.Filter.GOOD


def test_linear_gradient():
    with pytest.raises(TypeError):
        cairo.LinearGradient()


def test_radial_gradient():
    with pytest.raises(TypeError):
        cairo.RadialGradient()


def test_gradient_get_color_stops():
    pattern = cairo.LinearGradient(1, 2, 4, 5)
    assert pattern.get_color_stops_rgba() == []
    pattern.add_color_stop_rgb(0.125, 0.25, 0.5, 0.75)
    assert pattern.get_color_stops_rgba() == [(0.125, 0.25, 0.5, 0.75, 1.0)]
    pattern.add_color_stop_rgba(1.0, 0.75, 0.5, 0.25, 0.125)
    assert pattern.get_color_stops_rgba() == \
        [(0.125, 0.25, 0.5, 0.75, 1.0), (1.0, 0.75, 0.5, 0.25, 0.125)]


def test_gradient_add_color_stop_rgb():
    pattern = cairo.LinearGradient(1, 2, 4, 5)
    with pytest.raises(TypeError):
        pattern.add_color_stop_rgb()


def test_gradient_add_color_stop_rgba():
    pattern = cairo.LinearGradient(1, 2, 4, 5)
    with pytest.raises(TypeError):
        pattern.add_color_stop_rgba()


def test_solid_pattern():
    with pytest.raises(TypeError):
        cairo.SolidPattern()


def test_mesh_pattern():
    mesh = cairo.MeshPattern()
    assert isinstance(mesh, cairo.MeshPattern)
    assert issubclass(cairo.MeshPattern, cairo.Pattern)
    with pytest.raises(TypeError):
        cairo.MeshPattern(object())


def test_mesh_pattern_example1():
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


def test_mesh_pattern_curve_to():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.curve_to(object())


def test_mesh_pattern_get_control_point():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.get_control_point(object())


def test_mesh_pattern_get_corner_color_rgba():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.get_corner_color_rgba(object())


def test_mesh_pattern_get_path():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.get_path(object())


def test_mesh_pattern_line_to():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.line_to(object())


def test_mesh_pattern_move_to():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.move_to(object())


def test_mesh_pattern_set_control_point():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.set_control_point(object())


def test_mesh_pattern_set_corner_color_rgb():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.set_corner_color_rgb(object())


def test_mesh_pattern_set_corner_color_rgba():
    pattern = cairo.MeshPattern()
    with pytest.raises(TypeError):
        pattern.set_corner_color_rgba(object())


def test_mesh_pattern_example2():
    pattern = cairo.MeshPattern()
    pattern.begin_patch()
    pattern.move_to(100, 100)
    pattern.line_to(130, 130)
    pattern.line_to(130, 70)
    pattern.set_corner_color_rgb(0, 1, 0, 0)
    pattern.set_corner_color_rgb(1, 0, 1, 0)
    pattern.set_corner_color_rgb(2, 0, 0, 1)
    pattern.end_patch()


def test_mesh_pattern_rest():
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


def test_mesh_pattern_error_states():
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


def test_get_matrix():
    pattern = cairo.SolidPattern(1, 2, 4)
    assert isinstance(pattern.get_matrix(), cairo.Matrix)
    pattern.set_matrix(cairo.Matrix())
    with pytest.raises(TypeError):
        pattern.set_matrix(object())


def test_set_extend():
    pattern = cairo.SolidPattern(1, 2, 4)
    pattern.set_extend(42)
    assert pattern.get_extend() == 42
    with pytest.raises(TypeError):
        pattern.set_extend(object())


def test_set_filter():
    pattern = cairo.SolidPattern(1, 2, 4)
    with pytest.raises(TypeError):
        pattern.set_filter(object())


def test_pattern():
    with pytest.raises(TypeError):
        cairo.Pattern()

    r, g, b, a = 0.1, 0.2, 0.3, 0.4
    p = cairo.SolidPattern(r, g, b, a)
    assert p.get_rgba() == (r, g, b, a)

    assert not p == object()
    hash(p)

    with pytest.raises(TypeError):
        cairo.Gradient()

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


def test_surface_pattern():
    with pytest.raises(TypeError):
        cairo.SurfacePattern(object())
