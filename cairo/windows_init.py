import os


def _search_for_dlls_on_path() -> str:
    """Search for Cairo DLLs in the Path.

    Returns:
        str: The first directory found.

    """
    env_paths = os.environ.get("PATH", "").split(os.pathsep)
    cairo_dlls = ("cairo-2.dll", "libcairo-2.dll", "cairo.dll", "libcairo.dll")
    dirs_with_cairo_dll = (
        path_dir
        for path_dir in env_paths
        for dll in cairo_dlls
        if os.path.isfile(os.path.join(path_dir, dll))
    )
    return next(dirs_with_cairo_dll) if dirs_with_cairo_dll else ""
