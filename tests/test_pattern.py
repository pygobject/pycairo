import cairo
import pytest


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


def test_gradient_get_color_stops():
    pattern = cairo.LinearGradient(1, 2, 4, 5)
    assert pattern.get_color_stops_rgba() == []
    pattern.add_color_stop_rgb(0.125, 0.25, 0.5, 0.75)
    assert pattern.get_color_stops_rgba() == [(0.125, 0.25, 0.5, 0.75, 1.0)]
    pattern.add_color_stop_rgba(1.0, 0.75, 0.5, 0.25, 0.125)
    assert pattern.get_color_stops_rgba() == \
        [(0.125, 0.25, 0.5, 0.75, 1.0), (1.0, 0.75, 0.5, 0.25, 0.125)]


def test_mesh_pattern():
    mesh = cairo.MeshPattern()
    assert isinstance(mesh, cairo.MeshPattern)
    assert issubclass(cairo.MeshPattern, cairo.Pattern)


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
