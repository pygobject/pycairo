import cairo
import pytest


def test_type():
    assert cairo.Rectangle
    assert issubclass(cairo.Rectangle, tuple)

    with pytest.raises(TypeError):
        cairo.Rectangle()

    r = cairo.Rectangle(0.0, 0.5, 0.25, 0.75)
    assert hash(r) == hash(cairo.Rectangle(0.0, 0.5, 0.25, 0.75))
    assert isinstance(r, tuple)
    assert r == (0.0, 0.5, 0.25, 0.75)
    assert r == cairo.Rectangle(0.0, 0.5, 0.25, 0.75)
    assert r[1] == 0.5
    assert r.width == 0.25
    assert r.height == 0.75
    assert r.x == 0.0
    assert r.y == 0.5

    with pytest.raises(AttributeError):
        assert r.z

    assert repr(r) == "cairo.Rectangle(x=0.0, y=0.5, width=0.25, height=0.75)"
    assert str(r) == repr(r)
    assert eval(repr(r)) == r

    assert cairo.Rectangle.x
    assert cairo.Rectangle.y
    assert cairo.Rectangle.width
    assert cairo.Rectangle.height


def test_context():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)
    context.line_to(0, 5)
    context.line_to(5, 5)
    context.line_to(5, 0)
    context.line_to(0, 0)
    context.clip()

    rects = context.copy_clip_rectangle_list()
    assert rects
    assert len(rects) == 1
    assert isinstance(rects[0], cairo.Rectangle)


def test_recording_surface():
    surface = cairo.RecordingSurface(
        cairo.CONTENT_COLOR, cairo.Rectangle(1, 1, 10, 10))

    assert isinstance(surface.get_extents(), cairo.Rectangle)
