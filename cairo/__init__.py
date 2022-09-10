from pathlib import Path

import os
import sys

if sys.platform == "win32" and sys.version_info >= (3, 8):
    env_path = os.environ.get("PATH", "").split(os.pathsep)
    first_gtk_dll_path = next(
            filter(
                lambda path: path is not None
                and Path.is_file(Path(path) / "cairo.dll"),
                env_path,
            ),
            None,
    )
    if first_gtk_dll_path:
        with os.add_dll_directory(first_gtk_dll_path):
            from ._cairo import *  # noqa: F401,F403

else:
    from ._cairo import *  # noqa: F401,F403


def get_include():
    """Returns a path to the directory containing the C header files"""

    import os

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
