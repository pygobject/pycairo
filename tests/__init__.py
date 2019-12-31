# -*- coding: utf-8 -*-

import os


def set_dll_search_path():
    # Python 3.8 no longer searches for DLLs in PATH, so we have to add
    # everything in PATH manually. Note that unlike PATH add_dll_directory
    # has no defined order, so if there are two cairo DLLs in PATH we
    # might get a random one.
    if os.name != "nt" or not hasattr(os, "add_dll_directory"):
        return
    for p in os.environ.get("PATH", "").split(os.pathsep):
        try:
            os.add_dll_directory(p)
        except OSError:
            pass


set_dll_search_path()
