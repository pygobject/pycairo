import cairo
import pytest


def test_region_contains_rectangle():
    rect = cairo.RectangleInt(1, 2, 10, 13)
    region = cairo.Region()
    assert region.contains_rectangle(rect) == cairo.RegionOverlap.OUT
    assert isinstance(region.contains_rectangle(rect), cairo.RegionOverlap)


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

    rect = cairo.RectangleInt(1, 2, 10, 13)
    same = cairo.RectangleInt(1, 2, 10, 13)
    other = cairo.RectangleInt(2, 2, 10, 13)
    with pytest.raises(TypeError):
        hash(rect)

    assert rect == same
    assert rect != other
