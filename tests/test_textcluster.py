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
