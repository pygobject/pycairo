# snippet list generation
import os

# list of snippet files
snip_list = [x[:-3] for x in os.listdir (os.path.dirname (__file__)) if not x.startswith('_') and x.endswith('.py')]
snip_list.sort()

# function used by some or all snippets
def snippet_normalize (ctx, width, height):
    ctx.scale (width, height)
    ctx.set_line_width (0.04)

def snippet_set_bg_svg (ctx, filename):
    import cairo.svg

    svg_cr = cairo.svg.Context()
    svg_cr.parse (filename)
    w, h = svg_cr.get_size()
    ctx.save()
    ctx.scale (1.0/w, 1.0/h)
    svg_cr.render (ctx)
    ctx.restore()

    
