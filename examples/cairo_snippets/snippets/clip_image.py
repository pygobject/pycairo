from math import pi
from pathlib import Path
import cairo


def draw(cr, width, height):

    cr.scale(width, height)
    cr.arc(0.5, 0.5, 0.25, 0, 2 * pi)
    cr.clip()

    image = cairo.ImageSurface.create_from_png(
        Path(__file__).parent / "romedalen.png"
    )
    w, h = image.get_width(), image.get_height()
    cr.scale(1 / w, 1 / h)

    cr.set_source_surface(image)
    cr.paint()
