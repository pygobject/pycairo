Integration with other Libraries
================================

Pyglet & ImageSurface as Texture
--------------------------------

Creating a pyglet.Texture from an ImageSurface:
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


Freetype-py & Cairo
-------------------

See https://github.com/rougier/freetype-py/tree/master/examples for examples. Most of the ``*-cairo.py`` examples illustrate conversion from FreeType bitmaps to Cairo surfaces; the two examples, ``glyph-vector-cairo.py`` and ``glyph-vector-2-cairo.py``, illustrate conversion from FreeType glyph contours to Cairo paths.


ModernGL & ImageSurface as Texture
----------------------------------

Creating a moderngl.Texture from an ImageSurface:
    .. code:: python

        import moderngl
        import cairo
        ctx = moderngl.create_context(standalone=True)
        width, height = 400, 400
        surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
        texture = ctx.texture((width, height), 4, data=surface.get_data())

An example can also be found in the ModernGL project:
https://github.com/moderngl/moderngl/blob/master/examples/integration_pycairo.py
