# -*- coding: utf-8 -*-

import math

import pytest
import cairo

pytest.importorskip("hypothesis")
from hypothesis import given, strategies, assume


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
