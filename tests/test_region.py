import cairo


def test_region_contains_rectangle():
    rect = cairo.RectangleInt(1, 2, 10, 13)
    region = cairo.Region()
    assert region.contains_rectangle(rect) == cairo.RegionOverlap.OUT
    assert isinstance(region.contains_rectangle(rect), cairo.RegionOverlap)
