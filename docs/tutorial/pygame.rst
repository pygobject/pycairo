Integration with Pygame
=======================

https://www.pygame.org

Pygame & ImageSurface
---------------------

Creating a pygame.image from an ImageSurface:

.. code:: python

    import pygame
    import cairo

    width, height = 255, 255
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
    buf = surface.get_data()
    image = pygame.image.frombuffer(buf, (width, height), "ARGB")
