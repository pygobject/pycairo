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
        buf = surface.get_data()
        image = pygame.image.frombuffer(buf, (width, height), "ARGB")


Pyglet & ImageSurface as Texture
--------------------------------

Creating a pyglet.Texture from an ImageSurface
    .. code:: python
    
        import ctypes
        import cairo

        from pyglet import app, clock, gl, image, window
        
        # create data shared by ImageSurface and Texture
        width, height = 400, 400

        surface_data = (ctypes.c_ubyte * (width * height * 4))()
        surface = cairo.ImageSurface.create_for_data (surface_data, cairo.FORMAT_ARGB32,
        width, height, width * 4); 
        texture = image.Texture.create_for_size(gl.GL_TEXTURE_2D, width, height, gl.GL_RGBA)


Draw pyglet.Texture bound to ImageSurface
    .. code:: python

        window = window.Window(width=width, height=height)

        @window.event
        def on_draw():
            window.clear()

            # Draw texture backed by ImageSurface
            gl.glEnable(gl.GL_TEXTURE_2D)

            gl.glBindTexture(gl.GL_TEXTURE_2D, texture.id)
            gl.glTexImage2D(gl.GL_TEXTURE_2D, 0, gl.GL_RGBA, width, height, 0, gl.GL_BGRA,
            gl.GL_UNSIGNED_BYTE,
            surface_data)

            gl.glBegin(gl.GL_QUADS)
            gl.glTexCoord2f(0.0, 1.0)
            gl.glVertex2i(0, 0)
            gl.glTexCoord2f(1.0, 1.0)
            gl.glVertex2i(width, 0)
            gl.glTexCoord2f(1.0, 0.0)
            gl.glVertex2i(width, height)
            gl.glTexCoord2f(0.0, 0.0)
            gl.glVertex2i(0, height)
            gl.glEnd()

        # call clock.schedule_update here to update the ImageSurface every frame
        app.run()


Pillow (PIL) & Cairo
--------------------

Creating an ImageSurface from a PIL Image:
    .. code:: python
        
        import PIL.Image as Image

        def from_pil(im, alpha=1.0, format=cairo.FORMAT_ARGB32):
            """
            :param im: Pillow Image
            :param alpha: 0..1 alpha to add to non-alpha images
            :param format: Pixel format for output surface
            """
            assert format in (cairo.FORMAT_RGB24, cairo.FORMAT_ARGB32), "Unsupported pixel format: %s" % format
            if 'A' not in im.getbands():
                im.putalpha(int(alpha * 256.))
            arr = bytearray(im.tobytes('raw', 'BGRa'))
            surface = cairo.ImageSurface.create_for_data(arr, format, im.width, im.height)
            return surface


        filename = 'test.jpeg'

        # Open image to an ARGB32 ImageSurface
        im = Image.open(filename)
        surface1 = from_pil(im)

        # Open image to an RGB24 ImageSurface
        im = Image.open(filename)
        surface2 = from_pil(im, format=cairo.FORMAT_RGB24)

        # Open image to an ARGB32 ImageSurface, 50% opacity
        im = Image.open(filename)
        surface3 = from_pil(im, alpha=0.5, format=cairo.FORMAT_ARGB32)


Freetype-py & Cairo
-------------------

See https://github.com/rougier/freetype-py/tree/master/examples for examples. Most of the ``*-cairo.py`` examples illustrate conversion from FreeType bitmaps to Cairo surfaces; the two examples, ``glyph-vector-cairo.py`` and ``glyph-vector-2-cairo.py``, illustrate conversion from FreeType glyph contours to Cairo paths.
