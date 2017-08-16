import cairo
import pytest


@pytest.fixture
def context():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 42, 42)
    return cairo.Context(surface)


def test_path():
    assert cairo.Path

    with pytest.raises(TypeError):
        cairo.Path()


def test_path_str(context):
    p = context.copy_path()
    assert isinstance(p, cairo.Path)
    assert str(p) == ""

    context.line_to(1, 2)
    p = context.copy_path()
    assert str(p) == "move_to 1.000000 2.000000"

    context.line_to(1, 2)
    p = context.copy_path()
    assert str(p) == "move_to 1.000000 2.000000\nline_to 1.000000 2.000000"

    context.new_path()
    context.curve_to(0, 1, 2, 3, 4, 5)
    p = context.copy_path()
    assert str(p) == (
        "move_to 0.000000 1.000000\n"
        "curve_to 0.000000 1.000000 2.000000 3.000000 4.000000 5.000000")

    context.new_path()
    context.line_to(1, 2)
    context.close_path()
    p = context.copy_path()
    assert str(p) == (
        "move_to 1.000000 2.000000\n"
        "close path\n"
        "move_to 1.000000 2.000000")


def test_path_compare_hash(context):
    p = context.copy_path()
    assert p == p
    hash(p)
    assert not p != p
    assert p != object()
    assert not p < p
    assert p <= p
    assert p >= p
    assert not p > p


def test_path_iter(context):
    context.line_to(1, 2)
    context.line_to(2, 3)
    context.curve_to(0, 1, 2, 3, 4, 5)
    context.close_path()
    p = context.copy_path()
    i = iter(p)
    assert list(i) == [
        (0, (1.0, 2.0)),
        (1, (2.0, 3.0)),
        (2, (0.0, 1.0, 2.0, 3.0, 4.0, 5.0)),
        (3, ()),
        (0, (1.0, 2.0)),
    ]
