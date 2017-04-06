snippet_normalize (cr, width, height)

def path_ellipse (cr, x, y, width, height, angle=0):
    """
    x      - center x
    y      - center y
    width  - width of ellipse  (in x direction when angle=0)
    height - height of ellipse (in y direction when angle=0)
    angle  - angle in radians to rotate, clockwise
    """
    cr.save()
    cr.translate (x, y)
    cr.rotate (angle)
    cr.scale (width / 2.0, height / 2.0)
    cr.arc (0.0, 0.0, 1.0, 0.0, 2.0 * M_PI)
    cr.restore()

def fill_and_stroke (cr, fill_c, stroke_c, stroke_linewidth):
    assert fill_c is not None or stroke_c is not None
    cr.save()

    if fill_c is not None:
        cr.set_source_rgba(*fill_c)
        if stroke_c is not None:
            cr.fill_preserve()
        else:
            cr.fill()

    if stroke_c is not None:
        cr.set_source_rgba(*stroke_c)
        cr.set_line_width(stroke_linewidth)
        cr.stroke()

    cr.restore()

path_ellipse (cr, 0.5, 0.5, 1.0, 0.3, M_PI/4.0)
fill_and_stroke (cr, (1,0,0,1), (0,0,0,1), 0.02)
