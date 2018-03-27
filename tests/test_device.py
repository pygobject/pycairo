# -*- coding: utf-8 -*-

import os
import io
import tempfile

import cairo
import pytest


def test_context_manager():
    f = io.BytesIO()
    with cairo.ScriptDevice(f) as dev:
        dev.acquire()
        dev.release()

    with pytest.raises(cairo.Error) as excinfo:
        dev.acquire()
    assert excinfo.value.status == cairo.Status.DEVICE_FINISHED


def test_cmp_hash():
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    surface = cairo.ScriptSurface(dev, cairo.Content.COLOR_ALPHA, 42, 10)
    other = surface.get_device()
    assert dev == other
    assert not dev != other
    assert hash(dev) == hash(other)
    assert dev != object()


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

    with pytest.raises(TypeError):
        cairo.ScriptDevice()

    with pytest.raises((ValueError, TypeError)):
        cairo.ScriptDevice("\x00")


def test_script_device_mode():
    assert hasattr(cairo, "ScriptMode")
    assert cairo.ScriptMode.ASCII != cairo.ScriptMode.BINARY

    dev = cairo.ScriptDevice(io.BytesIO())
    mode = dev.get_mode()
    assert isinstance(mode, cairo.ScriptMode)
    assert mode == cairo.ScriptMode.ASCII
    dev.set_mode(cairo.ScriptMode.BINARY)
    assert dev.get_mode() == cairo.ScriptMode.BINARY
    with pytest.raises(TypeError):
        dev.set_mode(object())


def test_script_device_write_comment():
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    dev.write_comment("pycairo foo")
    dev.write_comment(u"pycairo bar")
    dev.flush()
    assert b"pycairo foo" in f.getvalue()
    assert b"pycairo bar" in f.getvalue()
    with pytest.raises(TypeError):
        dev.write_comment(object())


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


def test_device_acquire():
    f = io.BytesIO()
    dev = cairo.ScriptDevice(f)
    dev.acquire()
    dev.release()


def test_script_device_to_path():
    fd, fname = tempfile.mkstemp()
    os.close(fd)
    try:
        cairo.ScriptDevice(fname).finish()
    finally:
        os.unlink(fname)
