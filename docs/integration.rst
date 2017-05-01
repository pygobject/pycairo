================================
Integration with other Libraries
================================

NumPy & ImageSurface
--------------------

Creating an ImageSurface from a NumPy array:
    .. code:: python

        import numpy
        import cairo

        width, height = 255, 255
        data = numpy.ndarray(shape=(height, width), dtype=numpy.uint32)
        surface = cairo.ImageSurface.create_for_data(
            data, cairo.FORMAT_ARGB32, width, height)

Creating a NumPy array from an ImageSurface:
    .. code:: python

        import numpy
        import cairo

        width, height = 255, 255
        surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
        buf = surface.get_data()
        data = numpy.ndarray(shape=(width, height),
                             dtype=numpy.uint32,
                             buffer=buf)


Pygame & ImageSurface
---------------------

Creating a pygame.image from an ImageSurface:
    .. code:: python

        import pygame
        import cairo

        width, height = 255, 255
        surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
        image = pygame.image.frombuffer(buf, (width, height), "ARGB")
