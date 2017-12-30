# -*- coding: utf-8 -*-

import math
import os
import sys
import tempfile
import shutil

import pytest
import cairo

pytest.importorskip("hypothesis")
from hypothesis import given, strategies, assume, settings
from hypothesis.strategies import floats, integers

from .hypothesis_fspaths import fspaths


@pytest.fixture(scope='module')
def tempdir_path():
    dir_ = tempfile.mkdtemp()
    try:
        yield dir_
    finally:
        shutil.rmtree(dir_)


def _to_temp_path(tempdir_path, p):
    basename = os.path.basename(p)
    if sys.version_info[0] == 3 and isinstance(basename, bytes):
        tempdir_path = os.fsencode(tempdir_path)
    res = os.path.join(tempdir_path, basename)
    if not isinstance(p, (type(u""), type(b""))):
        res = type(p)(res)
    return res


def cairo_ver():
    return tuple(map(int, cairo.cairo_version_string().split(".")))


@given(path=fspaths())
@settings(max_examples=500)
def test_fspaths(tempdir_path, path):
    p = _to_temp_path(tempdir_path, path)

    assert not os.listdir(tempdir_path)
    # filter out "."
    if os.path.exists(p):
        return

    if cairo_ver() >= (1, 15, 10):
        def path_encode(p):
            return p.encode("utf-8")
    else:
        def path_encode(p):
            new = temp.encode("mbcs")
            if new.decode("mbcs") != p:
                raise ValueError
            return new

    # cairo up to 1.15.8 uses fopen, which only supports ANSI paths under
    # Windows. 1.15.10+ uses utf-8 like glib.
    is_valid = True
    if os.name == "nt":
        temp = os.path.join(p)
        if isinstance(temp, type(b"")):
            if sys.version_info[0] == 3:
                temp = os.fsdecode(temp)
            else:
                temp = temp.decode(sys.getfilesystemencoding(), "strict")
        if isinstance(temp, type(u"")):
            try:
                path_encode(temp)
            except ValueError:
                is_valid = False

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    try:
        surface.write_to_png(p)
    except (TypeError, ValueError):
        assert not is_valid
        assert not os.path.exists(p)
    except cairo.Error:
        # Under Python 2 we can't produce utf-8 without surrogates
        # and cairo 1.15.10+ errors out in that case.
        # And for some reason writing to "\x01" fails
        assert is_valid
        assert not os.path.exists(p)
    else:
        assert is_valid
        assert os.path.exists(p), os.listdir(tempdir_path)
        os.unlink(p)


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
