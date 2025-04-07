import ctypes

import cairo
import pytest


def test_type():
    assert cairo.TextCluster
    assert issubclass(cairo.TextCluster, tuple)

    with pytest.raises(TypeError):
        cairo.TextCluster()

    r = cairo.TextCluster(2, 1)
    assert hash(r) == hash(cairo.TextCluster(2, 1))
    assert isinstance(r, tuple)
    assert r == (2, 1)
    assert r == cairo.TextCluster(2, 1)
    assert r[1] == 1
    assert r.num_bytes == 2
    assert r.num_glyphs == 1

    with pytest.raises(AttributeError):
        assert r.z

    assert repr(r) == "cairo.TextCluster(num_bytes=2, num_glyphs=1)"
    assert str(r) == "cairo.TextCluster(num_bytes=2, num_glyphs=1)"
    assert eval(repr(r)) == r

    assert cairo.TextCluster.num_bytes
    assert cairo.TextCluster.num_glyphs


def test_text_cluster_limits():
    max_int = 2 ** (ctypes.sizeof(ctypes.c_int()) * 8 - 1) - 1
    min_int = -max_int - 1

    tc = cairo.TextCluster(max_int, min_int)
    assert tc.num_bytes == max_int
    assert tc.num_glyphs == min_int

    with pytest.raises(OverflowError):
        cairo.TextCluster(max_int + 1, min_int)
    with pytest.raises(OverflowError):
        cairo.RectangleInt(min_int, min_int - 1)
