# -*- coding: utf-8 -*-

import os
import tempfile

import cairo
import pytest

numpy = pytest.importorskip("numpy")


def test_image_surface_create_for_data_numpy_array():
    width, height = 255, 255
    data = numpy.ndarray(shape=(height, width), dtype=numpy.uint32)

    for y in range(height):
        for x in range(width):
            alpha = y
            b = int(x * alpha / 255.0)
            g = int(y * alpha / 255.0)
            data[y][x] = (b | g << 8 | alpha << 24)

    surface = cairo.ImageSurface.create_for_data(
        data, cairo.FORMAT_ARGB32, width, height)

    # for debugging
    fd, filename = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
    os.close(fd)
    surface.write_to_png(filename)
    os.unlink(filename)


def test_image_surface_get_data_to_numpy_array():
    w, h = 128, 128
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, w, h)
    ctx = cairo.Context(surface)

    ctx.set_source_rgb(1, 1, 1)  # white
    ctx.set_operator(cairo.OPERATOR_SOURCE)
    ctx.paint()

    # Draw out the triangle using absolute coordinates
    ctx.move_to(w / 2, h / 3)
    ctx.line_to(2 * w / 3, 2 * h / 3)
    ctx.rel_line_to(-1 * w / 3, 0)
    ctx.close_path()

    ctx.set_source_rgb(0, 0, 0)  # black
    ctx.set_line_width(15)
    ctx.stroke()

    buf = surface.get_data()

    a = numpy.ndarray(shape=(w, h), dtype=numpy.uint32, buffer=buf)

    # draw a vertical blue line
    a[:, 40] = 0xff | 0xff << 24

    # for debugging
    fd, filename = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
    os.close(fd)
    surface.write_to_png(filename)
    os.unlink(filename)
