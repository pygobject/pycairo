import sys
import os
import tempfile
import shutil
import platform

import pytest
import cairo


@pytest.fixture(scope='module')
def tempdir_path():
    dir_ = tempfile.mkdtemp()
    try:
        yield dir_
    finally:
        shutil.rmtree(dir_)


class _PathLike:

    def __init__(self, value):
        self._value = value

    def __fspath__(self):
        return self._value


def test_fspaths(tempdir_path) -> None:
    def write(path):
        surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 10, 10)
        surface.write_to_png(path)

    def get_temp_path(filename):
        filename = os.fspath(filename)
        if isinstance(filename, bytes):
            return os.path.join(os.fsencode(tempdir_path), filename)
        else:
            return os.path.join(tempdir_path, filename)

    def to_test_paths(filenames):
        for f in filenames:
            yield get_temp_path(f)
            yield _PathLike(get_temp_path(f))

    def can_encode(filename: str) -> bool:
        try:
            filename.encode(sys.getfilesystemencoding())
        except UnicodeEncodeError:
            return False
        return True

    FAIL = [
        "foo\x00bar",
        b"foo\x00bar",
    ]
    OK = [
        "foo",
        b"foo",
    ]
    CAIROFAIL = []

    if sys.platform == "linux":
        OK += [
            b"\xFF\xFF\xFF",
            os.fsdecode(b"\xFF\xFF\xFF"),
        ]

    for text in ["\u2764\uFE0F", "öäüß"]:
        if can_encode(text):
            OK += [
                text,
                os.fsencode(text),
            ]

    # https://github.com/pypy/pypy/issues/5259
    if os.name == "nt" and sys.getfilesystemencoding() == "utf-8" and \
            platform.python_implementation() != "PyPy":
        CAIROFAIL += [
            "\uD800",
            os.fsencode("\uD800"),
        ]

    # Supported by Python, but not by cairo
    for p in to_test_paths(CAIROFAIL):
        assert not os.path.exists(p)
        with pytest.raises(ValueError):
            write(p)
        assert not os.path.exists(p)
        with open(p, "wb"):
            pass
        assert os.path.exists(p)
        os.unlink(p)

    # Not supported by Python, and not by cairo
    for p in to_test_paths(FAIL):
        assert not os.path.exists(p)
        with pytest.raises(ValueError):
            write(p)
        # TypeError to work around pypy bug:
        # https://github.com/pypy/pypy/commit/0ecdacc5bde4c6cd79e2
        with pytest.raises((ValueError, TypeError)):
            with open(p, "wb"):
                pass
        assert not os.path.exists(p)

    # Supported by Python, and by cairo
    for p in to_test_paths(OK):
        assert not os.path.exists(p)
        write(p)
        assert os.path.exists(p)
        os.unlink(p)
