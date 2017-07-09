# -*- coding: utf-8 -*-

import io
import cairo
import pytest


def test_get_device():
    surface = cairo.ImageSurface(cairo.Format.ARGB32, 10, 10)
    assert surface.get_device() is None


def test_type():
    assert cairo.Device
    with pytest.raises(TypeError):
        cairo.Device()


def test_has():
    assert hasattr(cairo, "HAS_SCRIPT_SURFACE")


def test_script_device():
    dev = cairo.ScriptDevice(io.BytesIO())
    assert dev
    assert issubclass(cairo.ScriptDevice, cairo.Device)
    assert isinstance(dev, cairo.ScriptDevice)

    with pytest.raises(TypeError):
        cairo.ScriptDevice(None)


def test_script_device_mode():
    assert hasattr(cairo, "ScriptMode")
    assert cairo.ScriptMode.ASCII != cairo.ScriptMode.BINARY

    dev = cairo.ScriptDevice(io.BytesIO())
    mode = dev.get_mode()
    assert isinstance(mode, cairo.ScriptMode)
    assert mode == cairo.ScriptMode.ASCII
    dev.set_mode(cairo.ScriptMode.BINARY)
    assert dev.get_mode() == cairo.ScriptMode.BINARY


def test_script_device_write_comment():
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    dev.write_comment("pycairo foo")
    dev.write_comment(u"pycairo bar")
    dev.flush()
    assert b"pycairo foo" in f.getvalue()
    assert b"pycairo bar" in f.getvalue()


def test_from_recording_surface():
    s = cairo.RecordingSurface(cairo.CONTENT_COLOR, None)
    ctx = cairo.Context(s)
    ctx.paint()
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    dev.from_recording_surface(s)
    dev.flush()
    assert b"paint" in f.getvalue()

    # already finished
    dev.finish()
    with pytest.raises(cairo.Error):
        dev.from_recording_surface(s)

    # only recording surfaces allowed
    image = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
    with pytest.raises(TypeError):
        dev.from_recording_surface(image)

    # No None allowed
    with pytest.raises(TypeError):
        dev.from_recording_surface(None)
