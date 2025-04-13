#!/usr/bin/env python
"""demonstrate pycairo and pygame"""


import math
import sys

import cairo
import pygame


def draw(surface):
    # Draw three 50% transparent circles: red, green, and blue on a white background
    ctx = cairo.Context(surface)

    ctx.set_source_rgb(1, 1, 1)
    ctx.paint()

    width, height = 500, 500
    center_x, center_y = width / 2, height / 2
    radius = 120
    offset = 80

    ctx.set_source_rgba(1, 0, 0, 0.5)
    ctx.arc(center_x - offset, center_y - offset, radius, 0, 2.0 * math.pi)
    ctx.fill()

    ctx.set_source_rgba(0, 1, 0, 0.5)
    ctx.arc(center_x + offset, center_y - offset, radius, 0, 2.0 * math.pi)
    ctx.fill()

    ctx.set_source_rgba(0, 0, 1, 0.5)
    ctx.arc(center_x, center_y + offset, radius, 0, 2.0 * math.pi)
    ctx.fill()


def input(events):
    for event in events:
        if event.type == pygame.QUIT:
            sys.exit(0)
        else:
            print(event)


def main():
    width, height = 512, 512
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)

    pygame.init()
    pygame.display.set_mode((width, height))
    screen = pygame.display.get_surface()

    draw(surface)

    # Create PyGame surface from Cairo Surface
    buf = surface.get_data()
    image = pygame.image.frombuffer(buf, (width, height), "BGRA")
    # Tranfer to Screen
    screen.blit(image, (0, 0))
    pygame.display.flip()

    while True:
        input(pygame.event.get())


if __name__ == "__main__":
    main()
