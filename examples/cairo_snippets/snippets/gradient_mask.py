import cairo


def draw(cr, width, height):
    cr.scale(width, height)
    cr.set_line_width(0.04)

    pattern = cairo.LinearGradient(0, 0, 1, 1)
    pattern.add_color_stop_rgb(0, 0, 0.3, 0.8)
    pattern.add_color_stop_rgb(1, 0, 0.8, 0.3)

    mask = cairo.RadialGradient(0.5, 0.5, 0.25, 0.5, 0.5, 0.5)
    mask.add_color_stop_rgba(0, 0, 0, 0, 1)
    mask.add_color_stop_rgba(0.5, 0, 0, 0, 0)

    cr.set_source(pattern)
    cr.mask(mask)
