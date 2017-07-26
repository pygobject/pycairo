# -*- coding: utf-8 -*-

import math

import pytest
import cairo

pytest.importorskip("hypothesis")
from hypothesis import given, strategies, assume
from hypothesis.strategies import floats, integers


@given(strategies.floats(), strategies.floats())
def test_surface_set_device_scale(x_scale, y_scale):
    assume(not any(math.isnan(v) for v in [x_scale, y_scale]))

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    try:
        surface.set_device_scale(x_scale, y_scale)
    except cairo.Error as e:
        assert e.status == cairo.Status.INVALID_MATRIX
    else:
        assert surface.get_device_scale() == (x_scale, y_scale)


@given(strategies.floats(), strategies.floats())
def test_surface_set_device_offset(x_offset, y_offset):
    assume(not any(math.isnan(v) for v in [x_offset, y_offset]))

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    surface.set_device_offset(x_offset, y_offset)


@given(strategies.floats(), strategies.floats(), strategies.floats(),
       strategies.floats())
def test_surface_create_for_rectangle(x, y, w, h):
    assume(not any(math.isnan(v) for v in [x, y, w, h]))

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    try:
        surface.create_for_rectangle(x, y, w, h)
    except cairo.Error as e:
        assert e.status == cairo.Status.INVALID_SIZE


@given(integers(), floats(allow_nan=False), floats(allow_nan=False))
def test_glyph(index, x, y):
    try:
        g = cairo.Glyph(index, x, y)
    except OverflowError:
        pass
    else:
        assert g.index == index
        assert g.x == x
        assert g.y == y


@given(floats(allow_nan=False), floats(allow_nan=False),
       floats(allow_nan=False), floats(allow_nan=False))
def test_rectangle(x, y, width, height):
    r = cairo.Rectangle(x, y, width, height)
    assert r.x == x
    assert r.y == y
    assert r.width == width
    assert r.height == height


@given(integers(), integers())
def test_text_cluster(num_bytes, num_glyphs):
    try:
        tc = cairo.TextCluster(num_bytes, num_glyphs)
    except OverflowError:
        pass
    else:
        assert tc.num_bytes == num_bytes
        assert tc.num_glyphs == num_glyphs


@given(floats(allow_nan=False), floats(allow_nan=False),
       floats(allow_nan=False), floats(allow_nan=False),
       floats(allow_nan=False), floats(allow_nan=False))
def test_text_extents(x_bearing, y_bearing, width, height, x_advance,
                      y_advance):
    te = cairo.TextExtents(x_bearing, y_bearing, width, height, x_advance,
                           y_advance)
    assert te.x_bearing == x_bearing
    assert te.y_bearing == y_bearing
    assert te.width == width
    assert te.height == height
    assert te.x_advance == x_advance
    assert te.y_advance == y_advance


@given(integers(), integers(), integers(), integers())
def test_rect_int(x, y, width, height):
    try:
        r = cairo.RectangleInt(x, y, width, height)
    except OverflowError:
        pass
    else:
        assert r.x == x
        assert r.y == y
        assert r.width == width
        assert r.height == height


@given(integers())
def test_enums(value):
    try:
        e = cairo.Antialias(value)
    except OverflowError:
        pass
    else:
        assert e == value


@given(integers())
def test_context_get_set_operator(value):
    try:
        op = cairo.Operator(value)
    except OverflowError:
        return

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    context = cairo.Context(surface)
    try:
        context.set_operator(op)
    except OverflowError:
        return
    assert context.get_operator() == op
