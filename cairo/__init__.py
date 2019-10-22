# support overrides in different directories than our module
# so we can use _cairo from an external build dir
try:
    from ._cairo import *  # noqa: F401,F403
except ImportError:
    # If import fails, try an extended path search,
    # to work with uninstalled out-of-tree build setups
    from pkgutil import extend_path
    __path__ = extend_path(__path__, __name__)
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
