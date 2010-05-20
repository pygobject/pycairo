#!/usr/bin/env python
"""demonstrate pycairo and pygame
method1: use an intermediate Python array object
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
data = array.array('c', chr(0) * Width * Height * 4)
stride = Width * 4
surface = cairo.ImageSurface.create_for_data(data, cairo.FORMAT_ARGB32,Width, Height, stride)

pygame.init()
window = pygame.display.set_mode( (Width,Height) )
screen = pygame.display.get_surface()

draw(surface)

#Create PyGame surface from Cairo Surface
image = pygame.image.frombuffer(data.tostring(),(Width,Height),"ARGB",)
#Tranfer to Screen
screen.blit(image, (0,0))
pygame.display.flip()

while True:
  input(pygame.event.get())
