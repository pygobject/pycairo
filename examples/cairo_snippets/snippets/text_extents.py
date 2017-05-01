from math import pi
import cairo


def draw(cr, width, height):
    cr.scale(width, height)
    cr.set_line_width(0.04)

    utf8 = "cairo"

    cr.select_font_face("Sans",
                        cairo.FONT_SLANT_NORMAL,
                        cairo.FONT_WEIGHT_NORMAL)

    cr.set_font_size(0.4)
    x_bearing, y_bearing, width, height, x_advance, y_advance = \
        cr.text_extents(utf8)

    x = 0.1
    y = 0.6

    cr.move_to(x, y)
    cr.show_text(utf8)

    # draw helping lines
    cr.set_source_rgba(1, 0.2, 0.2, 0.6)
    cr.arc(x, y, 0.05, 0, 2 * pi)
    cr.fill()
    cr.move_to(x, y)
    cr.rel_line_to(0, -height)
    cr.rel_line_to(width, 0)
    cr.rel_line_to(x_bearing, -y_bearing)
    cr.stroke()
