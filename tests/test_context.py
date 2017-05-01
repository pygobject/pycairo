import cairo


def test_get_antialias():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)
    assert context.get_antialias() == cairo.Antialias.DEFAULT
    assert isinstance(context.get_antialias(), cairo.Antialias)
