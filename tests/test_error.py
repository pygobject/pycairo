import cairo
import pytest


def test_error_check_status() -> None:
    check_status = cairo.Error._check_status  # type: ignore

    with pytest.raises(cairo.Error) as e1:
        check_status(cairo.Status.DEVICE_FINISHED)
    assert e1.value.status == cairo.Status.DEVICE_FINISHED
    assert str(e1.value) == "the target device has been finished"
    assert repr(e1.value) == ("Error('the target device has been finished', "
                              "cairo.Status.DEVICE_FINISHED)")

    with pytest.raises(cairo.Error) as e2:
        check_status(cairo.Status.NO_MEMORY)
    assert e2.value.status == cairo.Status.NO_MEMORY
    with pytest.raises(MemoryError) as e3:
        check_status(cairo.Status.NO_MEMORY)
    assert type(e3.value).__name__ == "cairo.MemoryError"

    with pytest.raises(cairo.Error):
        check_status(cairo.Status.READ_ERROR)
    with pytest.raises(IOError) as e5:
        check_status(cairo.Status.READ_ERROR)
    assert isinstance(e5.value, cairo.Error)
    assert e5.value.status == cairo.Status.READ_ERROR
    with pytest.raises(cairo.Error):
        check_status(cairo.Status.WRITE_ERROR)
    with pytest.raises(IOError) as e7:
        check_status(cairo.Status.WRITE_ERROR)
    assert isinstance(e7.value, cairo.Error)
    assert e7.value.status == cairo.Status.WRITE_ERROR
    assert type(e7.value).__name__ == "cairo.IOError"

    err = e7.value
    err.status = cairo.Status.DEVICE_FINISHED
    assert err.status == cairo.Status.DEVICE_FINISHED

    with pytest.raises((TypeError, AttributeError)):
        del err.status

    str(cairo.Error())


def test_error_context() -> None:
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
    ctx = cairo.Context(surface)
    with pytest.raises(cairo.Error) as excinfo:
        ctx.restore()
    error = excinfo.value
    assert error.status == cairo.STATUS_INVALID_RESTORE
    assert error.status == cairo.Status.INVALID_RESTORE
    assert isinstance(error.status, cairo.Status)
    assert str(error)


def test_error() -> None:
    with pytest.raises(cairo.Error) as excinfo:
        raise cairo.Error
    assert excinfo.value.status is None

    with pytest.raises(cairo.Error) as excinfo:
        raise cairo.Error("foo")
    assert excinfo.value.status is None
    assert excinfo.value.args[0] == "foo"

    with pytest.raises(cairo.Error) as excinfo:
        raise cairo.Error("foo", 42)
    assert excinfo.value.status == 42

    class Foo(cairo.Error):
        pass

    Foo("foo", 42)


def test_error_alias() -> None:
    assert cairo.Error is cairo.CairoError
