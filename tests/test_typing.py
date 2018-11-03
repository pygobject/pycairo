import os
import io
import sys
import types

import cairo
import pytest

mypy = pytest.importorskip("mypy.api")
pytestmark = pytest.mark.skipif(
    sys.version_info[:2] < (3, 6), reason="Py3 only")
pytestmark


def test_mypy():
    out, err, status = mypy.run([os.path.dirname(cairo.__path__[0])])
    if status != 0:
        raise Exception("\n" + "\n".join([out, err]))


def test_typing():
    mod = types.ModuleType("cairo")
    stub = os.path.join(cairo.__path__[0], "__init__.pyi")
    with io.open(stub, "r", encoding="utf-8") as h:
        code = compile(h.read(), stub, "exec")
        exec(code, mod.__dict__)

    def collect_names(t):
        names = set()
        for key, value in vars(t).items():
            if key in ["XlibSurface", "XCBSurface", "Win32PrintingSurface",
                       "Win32Surface"]:
                continue
            if key.startswith("_"):
                continue
            if key.startswith("__") and key.endswith("__"):
                continue
            if getattr(value, "__module__", "") == "typing" or key == "Text":
                continue
            if isinstance(value, type):
                names.add(key)

                for k, v in vars(value).items():
                    name = key + "." + k
                    if k.startswith("_"):
                        continue
                    names.add(name)
            else:
                names.add(key)
        return names

    # We expose all potential API in the typing stubs, so only check
    # if it exactly matches the Python module with a new enough cairo
    cairo_version = tuple(map(int, cairo.cairo_version_string().split(".")))
    if cairo_version >= (1, 16, 0):
        assert collect_names(cairo) == collect_names(mod)
    else:
        assert collect_names(cairo) <= collect_names(mod)
