#!/usr/bin/env python
"""demonstrate pycairo and pygame
method1: use pycairo and pygame directly
"""

import array
import math
import sys

import cairo
import pygame

def draw(surface):
  x,y, radius = (250,250, 200)
  ctx = cairo.Context(surface)
  ctx.set_line_width(15)
  ctx.arc(x, y, radius, 0, 2.0 * math.pi)
  ctx.set_source_rgb(0.8, 0.8, 0.8)
  ctx.fill_preserve()
  ctx.set_source_rgb(1, 1, 1)
  ctx.stroke()

def input(events):
 for event in events:
  if event.type == pygame.QUIT:
      sys.exit(0)
  else:
      print event


Width, Height = 512, 512
surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, Width, Height)

pygame.init()
window = pygame.display.set_mode( (Width,Height) )
screen = pygame.display.get_surface()

draw(surface)

#Create PyGame surface from Cairo Surface
buf = surface.get_data()
image = pygame.image.frombuffer(buf,(Width,Height),"ARGB",)
#Tranfer to Screen
screen.blit(image, (0,0))
pygame.display.flip()

while True:
  input(pygame.event.get())


"""
with pycairo 1.4.12 and pygame 1.7.1 you get the error message:

Traceback (most recent call last):
  File "./pygame-test1.py", line 42, in <module>
    image = pygame.image.frombuffer(buf,(Width,Height),"ARGB",)
TypeError: char buffer type not available

This is because with
    buf = surface.get_data()
pycairo provides a binary image buffer,
whereas with
    image = pygame.image.frombuffer(buf,(Width,Height),"ARGB",)
pygame is expecting a text-based character buffer!
"""
