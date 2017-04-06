#!/usr/bin/env python

import cairo

surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 100, 100)
ctx = cairo.Context(surface)

print "default clip"
print ctx.copy_clip_rectangles(), "\n"

print "custom path" # 2 rectangles
ctx.move_to(0, 0)
ctx.line_to(50,0)
ctx.line_to(50,30)
ctx.line_to(60,30)
ctx.line_to(60,60)
ctx.line_to(30,60)
ctx.line_to(0, 60)
ctx.close_path()
ctx.clip()
print ctx.copy_clip_rectangles(), "\n"


