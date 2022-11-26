.. currentmodule:: cairo

Build Integration
=================

To compile a Python extension using Pycairo you need to know where Pycairo and
cairo are located and what flags to pass to the compiler and linker.

1. Variant:

    Similar to variant 2 but doesn't require pycairo to be loaded, which can be
    problematic on Windows where by default cairo isn't in the dll search path
    since Python 3.8.

    .. code:: python

        def get_include_path():
            import os
            from importlib.util import find_spec
            spec = find_spec("cairo")
            assert spec is not None
            return os.path.join(os.path.dirname(spec.origin), 'include')

    Compiler Flags:
        * ``python -c "get_include_path()"``
        * ``pkg-config --cflags cairo``

    Linker Flags:
        * ``pkg-config --libs cairo``

2. Variant:

    Only available since version 1.16.0.

    While Pycairo installs a pkg-config file, in case of virtualenvs,
    installation to the user directory or when using wheels/eggs, pkg-config
    will not be able to locate the .pc file. The :func:`get_include` function
    should work in all cases, as long as Pycairo is in your Python search path.

    Compiler Flags:
        * ``python -c "import cairo; print(cairo.get_include())"``
        * ``pkg-config --cflags cairo``

    Linker Flags:
        * ``pkg-config --libs cairo``

3. Variant:

    This works with older versions, but with the limitations mentioned above.
    Use it as a fallback if you want to support older versions or if your
    module does not require virtualenv/pip support.

    Compiler Flags:
        * ``pkg-config --cflags py3cairo``

    Linker Flags:
        * ``pkg-config --libs py3cairo``
