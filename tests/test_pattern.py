import cairo


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
