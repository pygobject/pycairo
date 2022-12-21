import os


if os.name == "nt" and hasattr(os, "add_dll_directory"):
    # In Windows, Python 3.8+ no longer searches the Path for DLLs for consistency and security

    # Try to search for DLLs in the PY_DLL_DIR if the environmental variable exists
    dll_env = os.environ.get("PY_DLL_DIR", "")
    if os.path.isdir(dll_env):
        with os.add_dll_directory(dll_env):
            from ._cairo import *  # noqa: F401,F403
    else:
        try:
            from ._cairo import *  # noqa: F401,F403
        except ImportError:
            # ImportError: DLL load failed while importing _cairo
            # Try to search for Cairo DLLs in the Path
            env_paths = os.environ.get("PATH", "").split(os.pathsep)
            cairo_dlls = ("cairo-2.dll", "libcairo-2.dll", "cairo.dll", "libcairo.dll")
            dirs_with_cairo_dll = (
                path_dir
                for path_dir in env_paths
                for dll in cairo_dlls
                if os.path.isfile(os.path.join(path_dir, dll))
            )
            if dirs_with_cairo_dll:
                first_dir = next(dirs_with_cairo_dll)
                with os.add_dll_directory(first_dir):
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
