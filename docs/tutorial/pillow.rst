Integration with Pillow
=======================

https://pillow.readthedocs.io

Creating an ImageSurface from a PIL Image:

.. code:: python

    import cairo
    import PIL.Image as Image

    def from_pil(im: Image, alpha: float=1.0, format: cairo.Format=cairo.FORMAT_ARGB32) -> cairo.ImageSurface:
        """
        :param im: Pillow Image
        :param alpha: 0..1 alpha to add to non-alpha images
        :param format: Pixel format for output surface
        """
        assert format in (
            cairo.FORMAT_RGB24,
            cairo.FORMAT_ARGB32,
        ), f"Unsupported pixel format: {format}"
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

Converting an ImageSurface to a PIL Image:

.. code:: python

    import cairo
    import PIL.Image as Image

    def to_pil(surface: cairo.ImageSurface) -> Image:
        format = surface.get_format()
        size = (surface.get_width(), surface.get_height())
        stride = surface.get_stride()

        with surface.get_data() as memory:
            if format == cairo.Format.RGB24:
                return Image.frombuffer(
                    "RGB", size, memory.tobytes(),
                    'raw', "BGRX", stride)
            elif format == cairo.Format.ARGB32:
                return Image.frombuffer(
                    "RGBA", size, memory.tobytes(),
                    'raw', "BGRa", stride)
            else:
                raise NotImplementedError(repr(format))

    # Create an image surface from a PNG file (or any other source)
    surface = cairo.ImageSurface.create_from_png("test.png")

    # Convert to a PIL Image
    im = to_pil(surface)

    # Use Pillow to store it as a JPEG
    im.save("result.jpg")
