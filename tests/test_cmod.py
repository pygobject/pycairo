# -*- coding: utf-8 -*-

from __future__ import absolute_import

import cairo

from . import cmod


def test_foo():
    surface = cmod.create_image_surface()
    assert isinstance(surface, cairo.ImageSurface)
