import cairo
import pytest


def test_matrix() -> None:
    m = cairo.Matrix()
    m.rotate(10)
    m.scale(1.5, 2.5)
    m.translate(10, 20)

    with pytest.raises(TypeError):
        m * 42  # type: ignore

    with pytest.raises(TypeError):
        m + 42  # type: ignore

    with pytest.raises(TypeError):
        cairo.Matrix(object())  # type: ignore

    assert m != 42
    assert m == m
    assert m != cairo.Matrix()

    assert repr(cairo.Matrix()) == "cairo.Matrix(1, 0, 0, 1, 0, 0)"


def test_init_rotate() -> None:
    r = cairo.Matrix.init_rotate(0)
    assert cairo.Matrix() == r

    with pytest.raises(TypeError):
        cairo.Matrix.init_rotate(object())  # type: ignore


def test_invert() -> None:
    m = cairo.Matrix(1, 1)
    m.invert()
    assert m == cairo.Matrix(1, -1, -0, 1, 0, 0)


def test_matrix_properties() -> None:
    m = cairo.Matrix(*range(6))
    assert [m.xx, m.yx, m.xy, m.yy, m.x0, m.y0] == list(range(6))
    m.xx = 42
    assert m.xx == 42
    m.scale(2, 2)
    assert m.xx == 84


def test_get_item() -> None:
    m = cairo.Matrix(1, 2, 3, 4, 5, 6)
    for i in range(6):
        assert m[i] == i + 1
    with pytest.raises(IndexError):
        m[6]
    with pytest.raises(IndexError):
        m[-1]


def test_multiply() -> None:
    with pytest.raises(TypeError):
        cairo.Matrix().multiply(object())  # type: ignore

    m = cairo.Matrix(1, 1, 0, 1)
    assert m.multiply(m) == cairo.Matrix(1, 2, 0, 1, 0, 0)
    assert m * m == m.multiply(m)


def test_translate() -> None:
    m = cairo.Matrix()
    m.translate(1, 1)
    assert m == cairo.Matrix(1, 0, 0, 1, 1, 1)
    with pytest.raises(TypeError):
        m.translate(1, object())  # type: ignore


def test_rotate() -> None:
    m = cairo.Matrix()
    with pytest.raises(TypeError):
        m.rotate(object())  # type: ignore


def test_scale() -> None:
    m = cairo.Matrix()
    with pytest.raises(TypeError):
        m.scale(object())  # type: ignore
    m.scale(2, 2)
    assert m != cairo.Matrix()
    m.scale(0.5, 0.5)
    assert m == cairo.Matrix()


def test_transform_distance() -> None:
    m = cairo.Matrix()
    assert m.transform_distance(1, 1) == (1, 1)
    with pytest.raises(TypeError):
        m.transform_distance(1, object())  # type: ignore


def test_transform_point() -> None:
    m = cairo.Matrix()
    assert m.transform_point(1, 1) == (1, 1)
    with pytest.raises(TypeError):
        m.transform_point(1, object())  # type: ignore
