# -*- coding: utf-8 -*-

import io
import os
import sys
import array
import tempfile
import struct

import cairo
import pytest


def test_tee_surface():
    main = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    tee = cairo.TeeSurface(main)
    assert isinstance(tee, cairo.TeeSurface)

    # the API is horrible, passing a wrong arg sets the surface to an error
    # state instead of returning the status.
    s1 = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    tee.add(s1)
    assert tee.index(0)
    assert tee.index(1)
    tee.remove(s1)


def test_image_surface_get_data_refcount():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    assert sys.getrefcount(surface) == 2
    d = surface.get_data()
    assert sys.getrefcount(surface) == 3
    del d
    assert sys.getrefcount(surface) == 2


def test_image_surface_get_data_crasher():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    try:
        assert 0, surface.get_data()
    except Exception:
        pass


def test_surface_get_content():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    assert surface.get_content() == cairo.Content.COLOR_ALPHA
    assert isinstance(surface.get_content(), cairo.Content)


def test_surface_get_format():
    surface = cairo.ImageSurface(cairo.Format.ARGB32, 10, 10)
    assert surface.get_format() == cairo.Format.ARGB32
    assert isinstance(surface.get_format(), cairo.Format)


def test_pdf_get_versions():
    versions = cairo.PDFSurface.get_versions()
    assert isinstance(versions, list)
    assert all(isinstance(v, cairo.PDFVersion) for v in versions)


def test_ps_surface_get_levels():
    levels = cairo.PSSurface.get_levels()
    assert isinstance(levels, list)
    assert all(isinstance(v, cairo.PSLevel) for v in levels)


def test_svg_surface_get_versions():
    versions = cairo.SVGSurface.get_versions()
    assert isinstance(versions, list)
    assert all(isinstance(v, cairo.SVGVersion) for v in versions)


def test_surface_get_device_scale():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    device_scale = surface.get_device_scale()
    assert all(isinstance(s, float) for s in device_scale)


def test_surface_set_device_scale():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    ret = surface.set_device_scale(5.0, 3.0)
    assert ret is None

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    with pytest.raises(cairo.Error):
        surface.set_device_scale(1, 0)


@pytest.mark.skipif(not cairo.HAS_PNG_FUNCTIONS, reason="not png support")
def test_image_surface_create_for_data_array():
    width, height = 255, 255
    data = array.array('B', [0] * width * height * 4)

    for y in range(height):
        for x in range(width):
            offset = (x + (y * width)) * 4
            alpha = y

            b = int(x * alpha / 255.0)
            g = int(y * alpha / 255.0)
            # cairo.FORMAT_ARGB32 uses pre-multiplied alpha
            data[offset:offset + 3] = array.array(
                "B", struct.pack("=I", b | g << 8 | alpha << 24))

    surface = cairo.ImageSurface.create_for_data(
        data, cairo.FORMAT_ARGB32, width, height)

    # for debugging
    fd, filename = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
    os.close(fd)
    surface.write_to_png(filename)
    os.unlink(filename)


@pytest.mark.skipif(not cairo.HAS_PNG_FUNCTIONS, reason="not png support")
def test_image_surface_write_to_png_filename_and_obj_compare():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 128, 128)
    fd, filename = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
    os.close(fd)
    surface.write_to_png(filename)

    fileobj = io.BytesIO()
    surface.write_to_png(fileobj)

    with open(filename, "rb") as h:
        assert h.read() == fileobj.getvalue()

    os.unlink(filename)


@pytest.mark.skipif(not cairo.HAS_PNG_FUNCTIONS, reason="not png support")
def test_image_surface_png_obj_roundtrip():
    fileobj = io.BytesIO()
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 128, 128)
    surface.write_to_png(fileobj)
    fileobj.seek(0)
    new_surface = cairo.ImageSurface.create_from_png(fileobj)
    assert surface.get_data() == new_surface.get_data()


@pytest.mark.skipif(not cairo.HAS_PNG_FUNCTIONS, reason="not png support")
def test_image_surface_png_file_roundtrip():
    fd, filename = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
    os.close(fd)

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 128, 128)
    surface.write_to_png(filename)

    new_surface = cairo.ImageSurface.create_from_png(filename)
    assert surface.get_data() == new_surface.get_data()
    os.unlink(filename)


@pytest.mark.skipif(not cairo.HAS_PNG_FUNCTIONS, reason="not png support")
def test_image_surface_write_to_png_error():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 128, 128)
    with pytest.raises(TypeError):
        surface.write_to_png(42)


def test_surface_from_stream_closed_before_finished():
    for Kind in [cairo.PDFSurface, cairo.PSSurface, cairo.SVGSurface]:
        fileobj = io.BytesIO()
        surface = cairo.PDFSurface(fileobj, 128, 128)
        fileobj.close()
        with pytest.raises(IOError):
            surface.finish()


def test_script_surface():
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    surface = cairo.ScriptSurface(dev, cairo.Content.COLOR_ALPHA, 42, 10)
    assert isinstance(surface, cairo.ScriptSurface)
    cairo.Context(surface).paint()
    dev.flush()
    assert b"42" in f.getvalue()
    assert b"paint" in f.getvalue()


def test_script_surface_create_for_target():
    # paint the script proxy
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    script = cairo.ScriptSurface.create_for_target(dev, surface)
    assert isinstance(script, cairo.ScriptSurface)
    ctx = cairo.Context(script)
    ctx.set_source_rgb(0.25, 0.5, 1.0)
    ctx.paint()
    assert b"paint" in f.getvalue()
    surface.flush()
    image_data = bytes(surface.get_data())

    # check if the result is the same
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    ctx = cairo.Context(surface)
    ctx.set_source_rgb(0.25, 0.5, 1.0)
    ctx.paint()
    surface.flush()
    assert bytes(surface.get_data()) == image_data
