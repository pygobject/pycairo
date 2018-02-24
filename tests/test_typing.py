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
    out, err, status = mypy.run(["--strict", cairo.__path__[0]])
    if status != 0:
        raise Exception("\n" + "\n".join([out, err]))


def test_typing():
    mod = types.ModuleType("cairo")
    stub = os.path.join(cairo.__path__[0], "__init__.pyi")
    with io.open(stub, "r", encoding="utf-8") as h:
        code = compile(h.read(), stub, "exec")
        exec(code, mod.__dict__)

    allowed_dunder = [
        "__init__", "__module__", "__dict__", "__weakref__", "__doc__",
        "__annotations__"]

    def collect_names(t, stub):
        names = set()
        for key, value in vars(t).items():
            if key in ["XlibSurface", "XCBSurface"]:
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
                        if stub and k not in allowed_dunder:
                            raise Exception("%s has %s" % (key, k))
                        continue
                    names.add(name)
            else:
                names.add(key)
        return names

    assert collect_names(mod, True) == collect_names(cairo, False)
