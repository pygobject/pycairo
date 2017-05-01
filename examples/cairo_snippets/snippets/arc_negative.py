from math import pi


def draw(cr, width, height):
    cr.scale(width, height)
    cr.set_line_width(0.04)

    xc = 0.5
    yc = 0.5
    radius = 0.4
    angle1 = 45.0 * (pi / 180.0)  # angles are specified
    angle2 = 180.0 * (pi / 180.0)  # in radians

    cr.arc_negative(xc, yc, radius, angle1, angle2)
    cr.stroke()

    # draw helping lines
    cr.set_source_rgba(1, 0.2, 0.2, 0.6)
    cr.arc(xc, yc, 0.05, 0, 2 * pi)
    cr.fill()
    cr.set_line_width(0.03)
    cr.arc(xc, yc, radius, angle1, angle1)
    cr.line_to(xc, yc)
    cr.arc(xc, yc, radius, angle2, angle2)
    cr.line_to(xc, yc)
    cr.stroke()
