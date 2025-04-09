import typing
import cairo
import pathlib

if typing.TYPE_CHECKING:
    cairo.PSSurface(b"", 0, 0)
    cairo.PSSurface("", 0, 0)
    cairo.PSSurface(pathlib.Path(""), 0, 0)
