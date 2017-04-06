#!/usr/bin/env python

import cairo
import math

def warpPath(ctx, function):
    first = True

    for type, points in ctx.copy_path():
        if type == cairo.PATH_MOVE_TO:
            if first:
                ctx.new_path()
                first = False
            x, y = function(*points)
            ctx.move_to(x, y)

        elif type == cairo.PATH_LINE_TO:
            x, y = function(*points)
            ctx.line_to(x, y)

        elif type == cairo.PATH_CURVE_TO:
            x1, y1, x2, y2, x3, y3 = points
            x1, y1 = function(x1, y1)
            x2, y2 = function(x2, y2)
            x3, y3 = function(x3, y3)
            ctx.curve_to(x1, y1, x2, y2, x3, y3)

        elif type == cairo.PATH_CLOSE_PATH:
            ctx.close_path()

def spiral(x, y):
    theta0 = -math.pi * 3 / 4
    theta = x / Width * math.pi * 2 + theta0
    radius = y + 200 - x/7
    xnew = radius*math.cos(theta)
    ynew = radius*math.sin(-theta)
    return xnew + Width/2, ynew + Height/2

def curl(x, y):
    xn = x - Textwidth/2
    #yn = y - Textheight/2
    xnew = xn
    ynew = y + xn ** 3 / ((Textwidth/2)**3) * 70
    return xnew + Width/2, ynew + Height*2/5


Width, Height = 512, 512
surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, Width, Height)
ctx = cairo.Context(surface)
solidpattern = ctx.get_source()

# background
pat = cairo.LinearGradient (0.0, 0.0, 0, Height)
pat.add_color_stop_rgba (1, 0, 0, 0, 1)
pat.add_color_stop_rgba (0, 1, 1, 1, 1)

ctx.rectangle (0,0,Width,Height)
ctx.set_source (pat)
ctx.fill ()

# foreground
ctx.set_source (solidpattern)
ctx.set_source_rgb (1,1,1)

ctx.select_font_face("Sans")
ctx.set_font_size(80)

# spiral text
ctx.new_path()
ctx.move_to(0, 0)
ctx.text_path("pycairo - " + "spam " * 5)
warpPath(ctx, spiral)
ctx.fill()

# curly text
ctx.new_path()
ctx.move_to(0, 0)
ctx.set_source_rgb(0.3, 0.3, 0.3)
text = "I am curly :)"
ctx.text_path(text)
Textwidth, Textheight = ctx.text_extents(text)[2:4]
warpPath(ctx, curl)
ctx.fill()

surface.write_to_png("warpedtext.png")
