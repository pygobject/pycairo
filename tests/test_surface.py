# -*- coding: utf-8 -*-

import io
import os
import sys
import array
import tempfile
import struct

import cairo
import pytest


def test_surface_cmp_hash():
    main = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    ctx = cairo.Context(main)
    assert ctx.get_target() == main
    assert not ctx.get_target() != main
    assert main != object()
    assert hash(ctx.get_target()) == hash(main)

    # we implement some hackery to change the underlying object after unmap
    # which would change the hash. Make sure this type is unhashable.
    mapped = main.map_to_image(None)
    with pytest.raises(TypeError):
        hash(mapped)
    assert mapped == mapped
    main.unmap_image(mapped)


def test_surface_map_to_image():
    main = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    image = main.map_to_image(None)

    with pytest.raises(TypeError):
        type(image)()

    other = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    with pytest.raises(ValueError):
        other.unmap_image(image)

    with pytest.raises(TypeError):
        main.unmap_image(other)

    with pytest.raises(RuntimeError):
        image.finish()

    main.unmap_image(image)

    with pytest.raises(RuntimeError):
        main.unmap_image(image)

    # from here on everything should fail
    with pytest.raises(cairo.Error):
        cairo.Context(image)

    with pytest.raises(TypeError):
        main.map_to_image(object())

    gced = main.map_to_image(None)
    del gced


def test_surface_map_to_image_data():
    main = cairo.ImageSurface(cairo.Format.RGB24, 2, 1)

    main.flush()
    assert bytes(main.get_data()) == b"\x00\x00\x00\x00\x00\x00\x00\x00"
    image = main.map_to_image(None)
    ctx = cairo.Context(image)
    ctx.set_source_rgb(1, 1, 1)
    ctx.paint()
    main.unmap_image(image)
    main.flush()
    assert bytes(main.get_data()) == b"\xff\xff\xff\xff\xff\xff\xff\xff"

    main = cairo.ImageSurface(cairo.Format.RGB24, 2, 1)
    image = main.map_to_image(cairo.RectangleInt(0, 0, 1, 1))
    ctx = cairo.Context(image)
    ctx.set_source_rgb(1, 1, 1)
    ctx.paint()
    main.unmap_image(image)
    main.flush()
    assert bytes(main.get_data()) == b"\xff\xff\xff\xff\x00\x00\x00\x00"


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


def test_pdf_set_size():
    fileobj = io.BytesIO()
    surface = cairo.PDFSurface(fileobj, 128, 128)
    surface.set_size(10, 10)
    with pytest.raises(TypeError):
        surface.set_size(10, object())


def test_pdf_surface():
    fd, fname = tempfile.mkstemp()
    os.close(fd)
    try:
        cairo.PDFSurface(fname, 10, 10).finish()
    finally:
        os.unlink(fname)

    with pytest.raises(TypeError):
        cairo.PDFSurface()

    with pytest.raises(TypeError):
        cairo.PDFSurface(object(), 100, 100)


def test_ps_surface_misc():
    surface = cairo.PSSurface(None, 10, 10)
    surface.dsc_begin_page_setup()
    surface.dsc_begin_setup()


def test_ps_surface_dsc_comment():
    surface = cairo.PSSurface(None, 10, 10)
    surface.dsc_comment("%%Title: My excellent document")
    with pytest.raises(cairo.Error):
        surface.dsc_comment("")
    with pytest.raises(TypeError):
        surface.dsc_comment(object())


def test_ps_get_eps():
    surface = cairo.PSSurface(None, 10, 10)
    assert isinstance(surface.get_eps(), bool)
    surface.set_eps(True)
    assert surface.get_eps()


def test_ps_set_size():
    surface = cairo.PSSurface(None, 10, 10)
    surface.set_size(10, 10)
    with pytest.raises(TypeError):
        surface.set_size(10, object())


def test_ps_restrict_to_level():
    surface = cairo.PSSurface(None, 10, 10)
    surface.restrict_to_level(cairo.PSLevel.LEVEL_2)
    with pytest.raises(TypeError):
        surface.restrict_to_level(object())


def test_ps_surface_level_to_string():
    level_id = cairo.PSSurface.level_to_string(cairo.PS_LEVEL_2)
    assert isinstance(level_id, str)
    assert cairo.PSSurface.ps_level_to_string(cairo.PS_LEVEL_2) == level_id
    with pytest.raises(ValueError):
        cairo.PSSurface.level_to_string(-1)


def test_ps_surface_get_levels():
    levels = cairo.PSSurface.get_levels()
    assert isinstance(levels, list)
    assert all(isinstance(v, cairo.PSLevel) for v in levels)


def test_ps_surface():
    assert isinstance(cairo.PSSurface(None, 10, 10), cairo.PSSurface)

    fd, fname = tempfile.mkstemp()
    os.close(fd)
    try:
        cairo.PSSurface(fname, 10, 10).finish()
    finally:
        os.unlink(fname)

    with pytest.raises(TypeError):
        cairo.PSSurface()

    with pytest.raises(TypeError):
        cairo.PSSurface(object(), 100, 100)


def test_scg_surface():
    fd, fname = tempfile.mkstemp()
    os.close(fd)
    try:
        cairo.SVGSurface(fname, 10, 10).finish()
    finally:
        os.unlink(fname)

    with pytest.raises(TypeError):
        cairo.SVGSurface()

    with pytest.raises(TypeError):
        cairo.SVGSurface(object(), 100, 100)


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


def test_script_device_device_ref():
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    surface = cairo.ScriptSurface(dev, cairo.Content.COLOR_ALPHA, 42, 10)
    del dev
    for i in range(10):
        surface.get_device()


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


def test_misc():
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    surface.copy_page()
    surface.mark_dirty()
    surface.show_page()


@pytest.fixture
def surface():
    return cairo.ImageSurface(cairo.Format.ARGB32, 10, 10)


@pytest.fixture
def image_surface():
    return cairo.ImageSurface(cairo.Format.ARGB32, 10, 10)


def test_create_similar(surface):
    similar = surface.create_similar(cairo.Content.COLOR, 10, 10)
    assert isinstance(similar, cairo.Surface)
    with pytest.raises(TypeError):
        surface.create_similar()


def test_get_device_offset(surface):
    surface.set_device_offset(1, 1)
    assert surface.get_device_offset() == (1, 1)
    with pytest.raises(TypeError):
        surface.set_device_offset(1, object())


def test_get_fallback_resolution(surface):
    surface.set_fallback_resolution(42, 42)
    assert surface.get_fallback_resolution() == (42, 42)
    with pytest.raises(TypeError):
        surface.set_fallback_resolution(42, object())


def test_mark_dirty_rectangle(surface):
    surface.mark_dirty_rectangle(0, 0, 10, 10)
    with pytest.raises(TypeError):
        surface.mark_dirty_rectangle(0, 0, 10, object())


def test_write_to_png(image_surface):
    with pytest.raises(TypeError):
        image_surface.write_to_png()


def test_image_surface_create_for_data():
    format_ = cairo.FORMAT_ARGB32
    surface = cairo.ImageSurface(format_, 3, 3)
    ctx = cairo.Context(surface)
    ctx.paint()
    surface.flush()
    buf = surface.get_data()

    new = cairo.ImageSurface.create_for_data(buf, format_, 3, 3)
    assert new.get_data() == buf

    with pytest.raises(ValueError):
        cairo.ImageSurface.create_for_data(buf, format_, 3, -1)
    with pytest.raises(ValueError):
        cairo.ImageSurface.create_for_data(buf, format_, -1, 3)

    with pytest.raises(ValueError):
        cairo.ImageSurface.create_for_data(buf, format_, 0, 0, -1)

    with pytest.raises(cairo.Error) as excinfo:
        cairo.ImageSurface.create_for_data(buf, format_, 3, 3, 3)

    assert excinfo.value.status == cairo.STATUS_INVALID_STRIDE


def test_image_surface_stride_for_width():
    v = cairo.ImageSurface.format_stride_for_width(cairo.Format.ARGB32, 10)
    assert v == 40

    with pytest.raises(TypeError):
        cairo.ImageSurface.format_stride_for_width(
            cairo.Format.ARGB32, object())


def test_image_surface_get_stride(image_surface):
    assert image_surface.get_stride() == 40


def test_recording_surface():
    with pytest.raises(TypeError):
        cairo.RecordingSurface(cairo.CONTENT_COLOR, object())

    surface = cairo.RecordingSurface(cairo.CONTENT_COLOR, None)
    assert surface.ink_extents() == (0.0, 0.0, 0.0, 0.0)
