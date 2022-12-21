import os


if os.name == "nt" and hasattr(os, "add_dll_directory"):
    # In Windows, Python 3.8+ no longer searches the Path for DLLs for consistency and security

    # Try to search for DLLs in the PY_DLL_DIR if the environmental variable exists
    _dll_env = os.environ.get("PY_DLL_DIR", "")
    if os.path.isdir(_dll_env):
        with os.add_dll_directory(_dll_env):
            from ._cairo import *  # noqa: F401,F403

    else:
        try:
            from ._cairo import *  # noqa: F401,F403
        except ImportError:
            # ImportError: DLL load failed while importing _cairo
            from cairo.windows_init import _search_for_dlls_on_path

            _cairo_dll_dir = _search_for_dlls_on_path()
            if _cairo_dll_dir:
                with os.add_dll_directory(_cairo_dll_dir):
                    from ._cairo import *  # noqa: F401,F403

else:
    from ._cairo import *  # noqa: F401,F403


def get_include():
    """Returns a path to the directory containing the C header files"""

    def is_ok(path):
        return os.path.exists(path) and os.path.isdir(path)

    package_path = os.path.dirname(os.path.realpath(__file__))
    install_path = os.path.join(package_path, "include")

    # in case we are installed
    if is_ok(install_path):
        return install_path

    # in case we are running from source
    if is_ok(package_path):
        return package_path

    # in case we are in an .egg
    import pkg_resources

    return pkg_resources.resource_filename(__name__, "include")
