.. title:: Tutorial

====================================
An Introduction to Cairo with Python
====================================

Cairo is a library for drawing vector graphics. Vector graphics are
interesting because they don't lose clarity when resized or transformed.

Pycairo is a set of bindings for cairo. It provides the cairo module which can
be used to call cairo commands from Python.

.. toctree::
    :titlesonly:

    integration
    examples


Understanding How to use Cairo
------------------------------

The best way to understand how to use cairo is to imagine that you are an
artist using a paintbrush to draw out a shape on canvas.

To begin, you can choose a few characteristics of your brush. You can choose
the thickness of your brush and the colour you want to paint with. You can
also choose the shape of your brush tip - You can choose either a circle or a
square.

Once you have chosen your brush, you are ready to start painting. You have to
be quite precise when describing what you want to appear.

Firstly, decide where you want to place your brush on the canvas. You do this
by supplying an x & y coordinate. Next you define how you want your brush
stroke to look - an arc, a straight line etc. Finally you define the point
where you want your stoke to end, again by supplying an x & y coordinate.
Triangles and squares are very easy to do!

More complex graphics are generated using variations of the above theme with a
few additions such as Fills (colouring in), transformations (zooming in,
moving) etc. Using the Python interface to cairo

Nearly all the work revolves around using the :class:`cairo.Context` (or
``cairo_t`` in the cairo C API). This is the object that you send your drawing
commands to. There are a few options available to initialize this object in
different ways.

Initializing the cairo.Context Object
-------------------------------------

* One Very Important thing to realize is there is a difference between the
  coordinates you are describing your graphics on and the coordinates you will
  be displaying your graphic on.

  (Ex - When giving a presentation you draw on your transparent acetate before
  hand, and then display it on your overhead projector - cairo calls the
  transparent acetate the user space coordinates and the projected image the
  device space coordinates)

  On initializing the cairo context object, we tell it how to transform our
  description to how it should be displayed. To do this we supply a
  transformation matrix. Modifying the transformation matrix can lead to some
  very interesting results.

* One of cairo's most powerful features is that it can output graphics in many
  different formats (it can use multiple back ends). For printing, we can have
  cairo translate our graphics into Postscript to be sent off to the printer.
  For on screen display, we can have cairo translate our graphics into
  something glitz can understand for hardware accelerated rendering! It has
  many more important and useful target back ends. On initializing the
  :class:`cairo.Context`, we set its target back end, supplying a few details
  (such as colour depth and size), as in the example below.

Example
-------

.. code:: python

    #!/usr/bin/env python

    import math
    import cairo

    WIDTH, HEIGHT = 256, 256

    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
    ctx = cairo.Context(surface)

    ctx.scale(WIDTH, HEIGHT)  # Normalizing the canvas

    pat = cairo.LinearGradient(0.0, 0.0, 0.0, 1.0)
    pat.add_color_stop_rgba(1, 0.7, 0, 0, 0.5)  # First stop, 50% opacity
    pat.add_color_stop_rgba(0, 0.9, 0.7, 0.2, 1)  # Last stop, 100% opacity

    ctx.rectangle(0, 0, 1, 1)  # Rectangle(x0, y0, x1, y1)
    ctx.set_source(pat)
    ctx.fill()

    ctx.translate(0.1, 0.1)  # Changing the current transformation matrix

    ctx.move_to(0, 0)
    # Arc(cx, cy, radius, start_angle, stop_angle)
    ctx.arc(0.2, 0.1, 0.1, -math.pi / 2, 0)
    ctx.line_to(0.5, 0.1)  # Line to (x,y)
    # Curve(x1, y1, x2, y2, x3, y3)
    ctx.curve_to(0.5, 0.2, 0.5, 0.4, 0.2, 0.8)
    ctx.close_path()

    ctx.set_source_rgb(0.3, 0.2, 0.5)  # Solid color
    ctx.set_line_width(0.02)
    ctx.stroke()

    surface.write_to_png("example.png")  # Output to PNG
