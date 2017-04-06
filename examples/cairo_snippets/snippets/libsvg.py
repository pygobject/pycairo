import cairo.svg

snippet_normalize (cr, width, height)

svg_cr = cairo.svg.Context()

svg_cr.parse ("data/home.svg")
w, h = svg_cr.get_size()
cr.scale (1.0/w, 1.0/h)
svg_cr.render (cr)

