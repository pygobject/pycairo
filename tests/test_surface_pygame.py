import cairo
import pytest

# 2.1.3+ required for "BGRA" format
pygame = pytest.importorskip("pygame", minversion="2.1.3")


def test_image_surface_to_pygame_image() -> None:
    width, height = 100, 100
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
    buf = surface.get_data()
    image = pygame.image.frombuffer(buf, (width, height), "BGRA")
    assert image
