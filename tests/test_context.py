import cairo
import pytest


@pytest.fixture
def context():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 42, 42)
    return cairo.Context(surface)


def test_get_antialias(context):
    assert context.get_antialias() == cairo.Antialias.DEFAULT
    assert isinstance(context.get_antialias(), cairo.Antialias)


def test_get_fill_rule(context):
    assert context.get_fill_rule() == cairo.FillRule.WINDING
    assert isinstance(context.get_fill_rule(), cairo.FillRule)


def test_get_line_cap(context):
    assert context.get_line_cap() == cairo.LineCap.BUTT
    assert isinstance(context.get_line_cap(), cairo.LineCap)


def test_get_line_join(context):
    assert context.get_line_join() == cairo.LineJoin.MITER
    assert isinstance(context.get_line_join(), cairo.LineJoin)


def test_get_operator(context):
    assert context.get_operator() == cairo.Operator.OVER
    assert isinstance(context.get_operator(), cairo.Operator)
