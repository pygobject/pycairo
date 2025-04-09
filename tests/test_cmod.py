import cairo
import pytest

try:
    from . import cmod
except ImportError:
    pytest.skip("cmod not built", allow_module_level=True)


def test_foo() -> None:
    surface = cmod.create_image_surface()
    assert isinstance(surface, cairo.ImageSurface)
