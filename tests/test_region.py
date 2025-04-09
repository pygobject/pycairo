import ctypes

import cairo
import pytest


def test_region() -> None:
    with pytest.raises(TypeError):
        cairo.Region(object())  # type: ignore

    with pytest.raises(TypeError):
        cairo.Region(object(), object())  # type: ignore

    with pytest.raises(TypeError):
        cairo.Region([object()])  # type: ignore


def test_get_rectangle() -> None:
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    with pytest.raises(ValueError):
        r.get_rectangle(-1)
    with pytest.raises(ValueError):
        r.get_rectangle(1)
    assert r.get_rectangle(0) == rect
    with pytest.raises(TypeError):
        r.get_rectangle(object())  # type: ignore


def test_rectangles_limits() -> None:
    max_int = 2 ** (ctypes.sizeof(ctypes.c_int()) * 8 - 1) - 1
    min_int = -max_int - 1

    rect = cairo.RectangleInt(max_int, min_int, max_int, min_int)
    assert rect.x == max_int
    assert rect.y == min_int
    assert rect.width == max_int
    assert rect.height == min_int

    with pytest.raises(OverflowError):
        cairo.RectangleInt(max_int + 1, min_int, max_int, min_int)
    with pytest.raises(OverflowError):
        cairo.RectangleInt(min_int, min_int, max_int, min_int - 1)


def test_contains_point() -> None:
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    assert r.contains_point(0, 0)
    assert not r.contains_point(0, 20)
    with pytest.raises(TypeError):
        r.contains_point(0, object())  # type: ignore


def test_intersect() -> None:
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.intersect(r)
    r.intersect(rect)
    with pytest.raises(TypeError):
        r.intersect(object())  # type: ignore
    with pytest.raises(TypeError):
        r.intersect()  # type: ignore

    assert r.__eq__(object()) == NotImplemented
    assert rect.__eq__(object()) == NotImplemented


def test_equal() -> None:
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    assert r.equal(r)
    with pytest.raises(TypeError):
        r.equal(object())  # type: ignore
    with pytest.raises(TypeError):
        r.equal()  # type: ignore


def test_subtract() -> None:
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.subtract(r)
    with pytest.raises(TypeError):
        r.subtract(object())  # type: ignore
    with pytest.raises(TypeError):
        r.subtract()  # type: ignore


def test_union() -> None:
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.union(r)
    r.union(rect)
    with pytest.raises(TypeError):
        r.union(object())  # type: ignore
    with pytest.raises(TypeError):
        r.union()  # type: ignore


def test_xor() -> None:
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.xor(r)
    r.xor(rect)
    with pytest.raises(TypeError):
        r.xor(object())  # type: ignore
    with pytest.raises(TypeError):
        r.xor()  # type: ignore


def test_translate() -> None:
    r = cairo.Region()
    r.translate(1, 1)
    with pytest.raises(TypeError):
        r.translate(1, object())  # type: ignore


def test_region_contains_rectangle() -> None:
    rect = cairo.RectangleInt(1, 2, 10, 13)
    region = cairo.Region()
    assert region.contains_rectangle(rect) == cairo.RegionOverlap.OUT
    assert isinstance(region.contains_rectangle(rect), cairo.RegionOverlap)
    with pytest.raises(TypeError):
        region.contains_rectangle(object())  # type: ignore


def test_region_cmp_hash() -> None:
    region = cairo.Region()
    other_region = cairo.Region()
    differ = cairo.Region(cairo.RectangleInt(0, 0, 10, 10))
    with pytest.raises(TypeError):
        hash(region)
    assert region == region
    assert region == other_region
    assert not region != other_region
    assert region != differ

    with pytest.raises(TypeError):
        region < region  # type: ignore

    with pytest.raises(TypeError):
        region > region  # type: ignore

    rect = cairo.RectangleInt(1, 2, 10, 13)
    same = cairo.RectangleInt(1, 2, 10, 13)
    other_rect = cairo.RectangleInt(2, 2, 10, 13)
    with pytest.raises(TypeError):
        hash(rect)

    assert rect == same
    assert rect != other_rect

    with pytest.raises(TypeError):
        rect < same  # type: ignore

    with pytest.raises(TypeError):
        rect > same  # type: ignore
