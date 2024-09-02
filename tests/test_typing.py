import os
import types

import cairo


def test_typing():
    mod = types.ModuleType("cairo")
    stub = os.path.join(os.path.dirname(cairo.__file__), "__init__.pyi")
    with open(stub, encoding="utf-8") as h:
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
                    if k.startswith("_"):
                        continue
                    name = f"{key}.{k}"
                    names.add(name)
            else:
                names.add(key)
        return names

    assert collect_names(cairo) <= collect_names(mod)
