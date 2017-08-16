import cairo
import pytest


def test_region():
    with pytest.raises(TypeError):
        cairo.Region(object())

    with pytest.raises(TypeError):
        cairo.Region(object(), object())

    with pytest.raises(TypeError):
        cairo.Region([object()])


def test_get_rectangle():
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    with pytest.raises(ValueError):
        r.get_rectangle(-1)
    with pytest.raises(ValueError):
        r.get_rectangle(1)
    assert r.get_rectangle(0) == rect
    with pytest.raises(TypeError):
        r.get_rectangle(object())


def test_contains_point():
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    assert r.contains_point(0, 0)
    assert not r.contains_point(0, 20)
    with pytest.raises(TypeError):
        r.contains_point(0, object())


def test_intersect():
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.intersect(r)
    r.intersect(rect)
    with pytest.raises(TypeError):
        r.intersect(object())
    with pytest.raises(TypeError):
        r.intersect()

    assert r.__eq__(object()) == NotImplemented
    assert rect.__eq__(object()) == NotImplemented


def test_equal():
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    assert r.equal(r)
    with pytest.raises(TypeError):
        r.equal(object())
    with pytest.raises(TypeError):
        r.equal()


def test_subtract():
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.subtract(r)
    with pytest.raises(TypeError):
        r.subtract(object())
    with pytest.raises(TypeError):
        r.subtract()


def test_union():
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.union(r)
    r.union(rect)
    with pytest.raises(TypeError):
        r.union(object())
    with pytest.raises(TypeError):
        r.union()


def test_xor():
    rect = cairo.RectangleInt(0, 0, 10, 10)
    r = cairo.Region(rect)
    r.xor(r)
    r.xor(rect)
    with pytest.raises(TypeError):
        r.xor(object())
    with pytest.raises(TypeError):
        r.xor()


def test_translate():
    r = cairo.Region()
    r.translate(1, 1)
    with pytest.raises(TypeError):
        r.translate(1, object())


def test_region_contains_rectangle():
    rect = cairo.RectangleInt(1, 2, 10, 13)
    region = cairo.Region()
    assert region.contains_rectangle(rect) == cairo.RegionOverlap.OUT
    assert isinstance(region.contains_rectangle(rect), cairo.RegionOverlap)
    with pytest.raises(TypeError):
        region.contains_rectangle(object())


def test_region_cmp_hash():
    region = cairo.Region()
    other = cairo.Region()
    differ = cairo.Region(cairo.RectangleInt(0, 0, 10, 10))
    with pytest.raises(TypeError):
        hash(region)
    assert region == region
    assert region == other
    assert not region != other
    assert region != differ

    with pytest.raises(TypeError):
        region < region

    with pytest.raises(TypeError):
        region > region

    rect = cairo.RectangleInt(1, 2, 10, 13)
    same = cairo.RectangleInt(1, 2, 10, 13)
    other = cairo.RectangleInt(2, 2, 10, 13)
    with pytest.raises(TypeError):
        hash(rect)

    assert rect == same
    assert rect != other

    with pytest.raises(TypeError):
        rect < same

    with pytest.raises(TypeError):
        rect > same
