#!/usr/bin/env python

import cairo
import math

class PathWarp(object):
	def __init__(self, function, ctx):
		self.function = function
		self.ctx = ctx

	def lineto(self, x, y):
		x, y = self.function(x, y)
		self.ctx.line_to(x, y)

	def moveto(self, x, y):
		if self.first:
			self.ctx.new_path()
			self.first = False
		x, y = self.function(x, y)
		self.ctx.move_to(x, y)

	def curveto(self, x1, y1, x2, y2, x3, y3):
		x1, y1 = self.function(x1, y1)
		x2, y2 = self.function(x2, y2)
		x3, y3 = self.function(x3, y3)
		self.ctx.curve_to(x1, y1, x2, y2, x3, y3)

	def closepath(self):
		self.ctx.close_path()

	def warpPath(self):
		self.first = True
		self.ctx.current_path(self.moveto, self.lineto, self.curveto, self.closepath)


WIDTH, HEIGHT = 512, 512
file = open("warpedtext.png", "wb")

ctx = cairo.Context()
ctx.set_target_png(file, cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
ctx.identity_matrix()

solidpattern = ctx.pattern

# background
pat = cairo.Pattern(x0=0.0, y0=0.0, x1=0, y1=HEIGHT)
pat.add_color_stop (1, 0, 0, 0, 1)
pat.add_color_stop (0, 1, 1, 1, 1)

ctx.rectangle (0,0,WIDTH,HEIGHT)
ctx.set_pattern (pat)
ctx.fill ()

# foreground
ctx.set_pattern (solidpattern)
ctx.set_rgb_color(1,1,1)

# spiral text
ctx.scale_font(80)
ctx.move_to(0, 0)
ctx.text_path("pycairo - " + "spam " * 5)
def spiral(x, y):
	theta0 = -math.pi * 3 / 4
	theta = x / WIDTH * math.pi * 2 + theta0
	radius = y + 200 - x/7
	xnew = radius*math.cos(theta)
	ynew = radius*math.sin(-theta)
	return xnew + WIDTH/2, ynew + HEIGHT/2
warp = PathWarp(spiral, ctx)
warp.warpPath()
ctx.fill()
ctx.new_path()

# curly text
ctx.move_to(0, 0)
ctx.set_rgb_color(0.3, 0.3, 0.3)
text = "I am curly :)"
ctx.text_path(text)
textwidth, textheight = ctx.text_extents(text)[2:4]
def curl(x, y):
	xn = x - textwidth/2
	yn = y - textheight/2
	xnew = xn
	ynew = y + xn ** 3 / ((textwidth/2)**3) * 70
	return xnew + WIDTH/2, ynew + HEIGHT*2/5
warp = PathWarp(curl, ctx)
warp.warpPath()
ctx.fill()


ctx.show_page()

