# -*- coding: utf-8 -*-

import cairo
import pytest


def test_get_device():
    surface = cairo.ImageSurface(cairo.Format.ARGB32, 10, 10)
    assert surface.get_device() is None


def test_type():
    assert cairo.Device
    with pytest.raises(TypeError):
        cairo.Device()
