import cairo


def test_get_extend():
    pattern = cairo.SolidPattern(1, 2, 4)
    assert pattern.get_extend() == cairo.Extend.PAD
    assert isinstance(pattern.get_extend(), cairo.Extend)


def test_get_filter():
    pattern = cairo.SolidPattern(1, 2, 4)
    assert pattern.get_filter() == cairo.Filter.GOOD
