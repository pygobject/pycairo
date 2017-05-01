from math import pi


def draw(cr, width, height):
    cr.scale(width, height)
    cr.set_line_width(0.04)

    cr.arc(0.5, 0.5, 0.3, 0, 2 * pi)
    cr.clip()

    cr.rectangle(0, 0, 1, 1)
    cr.fill()
    cr.set_source_rgb(0, 1, 0)
    cr.move_to(0, 0)
    cr.line_to(1, 1)
    cr.move_to(1, 0)
    cr.line_to(0, 1)
    cr.stroke()
