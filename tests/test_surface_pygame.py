# -*- coding: utf-8 -*-

import cairo
import pytest

pygame = pytest.importorskip("pygame")


def test_image_surface_to_pygame_image():
    width, height = 100, 100
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
    buf = surface.get_data()
    image = pygame.image.frombuffer(buf, (width, height), "ARGB")
    assert image
