import cairo
import pytest


def test_matrix():
    m = cairo.Matrix()
    m.rotate(10)
    m.scale(1.5, 2.5)
    m.translate(10, 20)

    with pytest.raises(TypeError):
        m * 42

    with pytest.raises(TypeError):
        m + 42

    with pytest.raises(TypeError):
        cairo.Matrix(object())

    assert m != 42
    assert m == m
    assert m != cairo.Matrix()

    assert repr(cairo.Matrix()) == "cairo.Matrix(1, 0, 0, 1, 0, 0)"


def test_init_rotate():
    r = cairo.Matrix.init_rotate(0)
    assert cairo.Matrix() == r

    with pytest.raises(TypeError):
        cairo.Matrix.init_rotate(object())


def test_invert():
    m = cairo.Matrix(1, 1)
    m.invert()
    assert m == cairo.Matrix(1, -1, -0, 1, 0, 0)


def test_matrix_properties():
    m = cairo.Matrix(*range(6))
    assert [m.xx, m.yx, m.xy, m.yy, m.x0, m.y0] == list(range(6))
    m.xx = 42
    assert m.xx == 42
    m.scale(2, 2)
    assert m.xx == 84


def test_get_item():
    m = cairo.Matrix(1, 2, 3, 4, 5, 6)
    for i in range(6):
        assert m[i] == i + 1
    with pytest.raises(IndexError):
        m[6]
    with pytest.raises(IndexError):
        m[-1]


def test_multiply():
    with pytest.raises(TypeError):
        cairo.Matrix().multiply(object())

    m = cairo.Matrix(1, 1, 0, 1)
    assert m.multiply(m) == cairo.Matrix(1, 2, 0, 1, 0, 0)
    assert m * m == m.multiply(m)


def test_translate():
    m = cairo.Matrix()
    m.translate(1, 1)
    assert m == cairo.Matrix(1, 0, 0, 1, 1, 1)
    with pytest.raises(TypeError):
        m.translate(1, object())


def test_rotate():
    m = cairo.Matrix()
    with pytest.raises(TypeError):
        m.rotate(object())


def test_scale():
    m = cairo.Matrix()
    with pytest.raises(TypeError):
        m.scale(object())
    m.scale(2, 2)
    assert m != cairo.Matrix()
    m.scale(0.5, 0.5)
    assert m == cairo.Matrix()


def test_transform_distance():
    m = cairo.Matrix()
    assert m.transform_distance(1, 1) == (1, 1)
    with pytest.raises(TypeError):
        m.transform_distance(1, object())


def test_transform_point():
    m = cairo.Matrix()
    assert m.transform_point(1, 1) == (1, 1)
    with pytest.raises(TypeError):
        m.transform_point(1, object())
