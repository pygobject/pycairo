import cairo
import pytest


@pytest.fixture
def context() -> cairo.Context:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 42, 42)
    return cairo.Context(surface)


def test_path() -> None:
    assert hasattr(cairo, "Path")

    with pytest.raises(TypeError):
        cairo.Path()


def test_path_str(context: cairo.Context) -> None:
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


def test_path_compare_hash(context: cairo.Context) -> None:
    p = context.copy_path()
    assert p == p
    hash(p)
    assert not p != p
    assert p != object()
    assert not p < p
    assert p <= p
    assert p >= p
    assert not p > p


def test_path_iter(context: cairo.Context) -> None:
    context.line_to(1, 2)
    context.line_to(2, 3)
    context.curve_to(0, 1, 2, 3, 4, 5)
    context.close_path()
    p = context.copy_path()
    items = list(iter(p))
    assert isinstance(items[0][0], cairo.PathDataType)
    assert items == [
        (cairo.PathDataType.MOVE_TO, (1.0, 2.0)),
        (cairo.PathDataType.LINE_TO, (2.0, 3.0)),
        (cairo.PathDataType.CURVE_TO, (0.0, 1.0, 2.0, 3.0, 4.0, 5.0)),
        (cairo.PathDataType.CLOSE_PATH, ()),
        (cairo.PathDataType.MOVE_TO, (1.0, 2.0)),
    ]
