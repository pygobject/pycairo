.. _surfaces:

*********************
Surfaces (incomplete)
*********************

cairo.Surface is the abstract type representing all different drawing targets
that cairo can render to. The actual drawings are performed using a
:class:`cairo.Context`.

A cairo.Surface is created by using backend-specific constructors
of the form cairo.<XXX>Surface().

class Surface()
===============

.. class:: Surface()

*Surface* is the abstract base class from which all the other surface classes
derive. It cannot be instantiated directly.

   .. method:: copy_page()

      Emits the current page for backends that support multiple pages, but
      doesn't clear it, so that the contents of the current page will be
      retained for the next page.  Use :meth:`.show_page` if you want to get an
      empty page after the emission.

      :meth:`Context.copy_page` is a convenience function for this.

      .. versionadded:: 1.6

   .. method:: create_similar(content, width, height)

      :param content: the content for the new surface
      :param width: width of the new surface, (in device-space units)
      :type width: int
      :param height: height of the new surface (in device-space units)
      :type width: int

      :returns: a newly allocated *Surface*.

      Create a *Surface* that is as compatible as possible with the existing
      surface. For example the new surface will have the same fallback
      resolution and :class:`FontOptions`. Generally, the new surface will
      also use the same backend, unless that is not possible for some
      reason.

      See the documentation for :ref:`content <mattributes_content>`
      for full details of the content options.

      Initially the surface contents are all 0 (transparent if contents have
      transparency, black otherwise.)

   .. method:: finish()

      This method finishes the *Surface* and drops all references to external
      resources. For example, for the Xlib backend it means that cairo will no
      longer access the drawable, which can be freed.  After calling finish()
      the only valid operations on a *Surface* are flushing and finishing it.
      Further drawing to the surface will not affect the surface but will
      instead trigger a :exc:`cairo.Error` exception.

   .. method:: flush()

      Do any pending drawing for the *Surface* and also restore any temporary
      modification's cairo has made to the *Surface's* state. This method
      must be called before switching from drawing on the *Surface* with cairo
      to drawing on it directly with native APIs. If the *Surface* doesn't
      support direct access, then this function does nothing.

   .. method:: get_content()

      :returns: The content type of *Surface*.

      This function returns the content type of *Surface* which indicates
      whether the surface contains color and/or alpha information. See
      :ref:`content <mattributes_content>`.

      .. versionadded:: 1.2

   .. method:: get_device_offset()

      :returns: (x_offset, y_offset) a tuple of float

        * x_offset: the offset in the X direction, in device units
        * y_offset: the offset in the Y direction, in device units

      This method returns the previous device offset set by :meth:`.set_device_offset`.

      .. versionadded:: 1.2

   .. method:: get_fallback_resolution()

      :returns: (x_pixels_per_inch, y_pixels_per_inch) a tuple of float

        * x_pixels_per_inch: horizontal pixels per inch
        * y_pixels_per_inch: vertical pixels per inch

      This method returns the previous fallback resolution set by
      :meth:`.set_fallback_resolution`, or default fallback resolution if
      never set.

      .. versionadded:: 1.8

   .. method:: get_font_options()

      :returns: a :class:`FontOptions`

      Retrieves the default font rendering options for the *Surface*. This
      allows display surfaces to report the correct subpixel order for
      rendering on them, print surfaces to disable hinting of metrics and so
      forth. The result can then be used with :class:`ScaledFont`.

   .. method:: mark_dirty()

      Tells cairo that drawing has been done to *Surface* using means other
      than cairo, and that cairo should reread any cached areas. Note that you
      must call :meth:`.flush` before doing such drawing.

   .. method:: mark_dirty_rectangle(x, y, width, height)

      :param x: X coordinate of dirty rectangle
      :type x: int
      :param y: Y coordinate of dirty rectangle
      :type y: int
      :param width: width of dirty rectangle
      :type width: int
      :param height: height of dirty rectangle
      :type height: int

      Like :meth:`.mark_dirty`, but drawing has been done only to the
      specified rectangle, so that cairo can retain cached contents for other
      parts of the surface.

      Any cached clip set on the *Surface* will be reset by this function, to
      make sure that future cairo calls have the clip set that they expect.

   .. method:: set_device_offset(x_offset, y_offset)

      :param x_offset: the offset in the X direction, in device units
      :type x_offset: float
      :param y_offset: the offset in the Y direction, in device units
      :type y_offset: float

      Sets an offset that is added to the device coordinates determined by the
      CTM when drawing to *Surface*. One use case for this function is when we
      want to create a *Surface* that redirects drawing for a portion of an
      onscreen surface to an offscreen surface in a way that is completely
      invisible to the user of the cairo API. Setting a transformation via
      :meth:`Context.translate` isn't sufficient to do this, since functions
      like :meth:`Context.device_to_user` will expose the hidden offset.

      Note that the offset affects drawing to the surface as well as using the
      surface in a source pattern.

   .. method:: set_fallback_resolution(x_pixels_per_inch, y_pixels_per_inch)

      :param x_pixels_per_inch: horizontal setting for pixels per inch
      :type x_pixels_per_inch: float
      :param y_pixels_per_inch: vertical setting for pixels per inch
      :type y_pixels_per_inch: float

      Set the horizontal and vertical resolution for image fallbacks.

      When certain operations aren't supported natively by a backend, cairo
      will fallback by rendering operations to an image and then overlaying
      that image onto the output. For backends that are natively
      vector-oriented, this function can be used to set the resolution used
      for these image fallbacks, (larger values will result in more detailed
      images, but also larger file sizes).

      Some examples of natively vector-oriented backends are the ps, pdf, and
      svg backends.

      For backends that are natively raster-oriented, image fallbacks are
      still possible, but they are always performed at the native device
      resolution. So this function has no effect on those backends.

      Note: The fallback resolution only takes effect at the time of
      completing a page (with :meth:`Context.show_page` or
      :meth:`Context.copy_page`) so there is currently no way to have more
      than one fallback resolution in effect on a single page.

      The default fallback resoultion is 300 pixels per inch in both
      dimensions.

      .. versionadded:: 1.2

   .. method:: show_page()

      Emits and clears the current page for backends that support multiple
      pages. Use :meth:`.copy_page` if you don't want to clear the page.

      There is a convenience function for this that takes a
      :meth:`Context.show_page`.

      .. versionadded:: 1.6

   .. method:: write_to_png(fobj)

      :param fobj: the file to write to
      :type fobj: str, file or file-like object
      :raises: *MemoryError* if memory could not be allocated for the operation

               *IOError* if an I/O error occurs while attempting to write the file

      Writes the contents of *Surface* to *fobj* as a PNG image.


class ImageSurface(:class:`Surface`)
====================================

A *cairo.ImageSurface* provides the ability to render to memory buffers either
allocated by cairo or by the calling code. The supported image formats are
those defined in :ref:`FORMAT attributes <mattributes_format>`.

.. class:: ImageSurface(format, width, height)

   :param format: format of pixels in the surface to create
   :param width: width of the surface, in pixels
   :param height: height of the surface, in pixels
   :returns: a new *ImageSurface*
   :raises: *MemoryError* in case of no memory

   Creates an *ImageSurface* of the specified format and dimensions. Initially
   the surface contents are all 0. (Specifically, within each pixel, each
   color or alpha channel belonging to format will be 0. The contents of bits
   within a pixel, but not belonging to the given format are undefined).

   .. method:: create_for_data(data, format, width, height, stride)

      :param data: a writable Python buffer object
      :param format: the format of pixels in the buffer
      :param width: the width of the image to be stored in the buffer
      :param height: the height of the image to be stored in the buffer
      :param stride: the number of bytes between the start of rows in the buffer as allocated. This value should always be computed by :meth:`.format_stride_for_width` before allocating the data buffer.
      :returns: a new *ImageSurface*
      :raises: *MemoryError* in case of no memory.

               :class:`cairo.Error` in case of invalid *stride* value.

      Creates an *ImageSurface* for the provided pixel data. The initial
      contents of buffer will be used as the initial image contents; you must
      explicitly clear the buffer, using, for example, cairo_rectangle() and
      cairo_fill() if you want it cleared.

      Note that the *stride* may be larger than width*bytes_per_pixel to
      provide proper alignment for each pixel and row. This alignment is
      required to allow high-performance rendering within cairo. The correct
      way to obtain a legal stride value is to call
      :meth:`.format_stride_for_width` with the desired format and maximum
      image width value, and use the resulting stride value to allocate the
      data and to create the *ImageSurface*. See
      :meth:`.format_stride_for_width` for example code.

   .. method:: create_from_png(fobj)

      :param fobj: a filename, file, or file-like object of the PNG to load.

      Creates a new *ImageSurface* and initializes the contents to the given
      PNG file.

   .. method:: format_stride_for_width(format, width)

      :param format: a cairo :ref:`format <mattributes_format>` value
      :param width: the desired width of an *ImageSurface* to be created.
      :returns: the appropriate stride to use given the desired format and width, or -1 if either the format is invalid or the width too large.

      This method provides a stride value that will respect all alignment
      requirements of the accelerated image-rendering code within
      cairo. Typical usage will be of the form::

        stride = cairo.ImageSurface.stride_for_width (format, width)
        surface = cairo.ImageSurface.create_for_data (data, format,
      				  width, height, stride)

      .. versionadded:: 1.6

   .. method:: get_data()

      :returns: a Python buffer object for the data of the *ImageSurface*, for direct inspection or modification.

      .. versionadded:: 1.2

   .. method:: get_format()

   .. method:: get_height()

   .. method:: get_stride()

   .. method:: get_width()


class PDFSurface(:class:`Surface`)
==================================

.. class:: PDFSurface

.. comment
 C:  surface = cairo_pdf_surface_create (filename, width_in_points,
 				        height_in_points);
 Py: surface = cairo.PDFSurface (f, width_in_points, height_in_points)
     where 'f' is a filename, a file object, or a file-like object


   .. method:: set_size()


class PSSurface(:class:`Surface`)
=================================

.. class:: PSSurface

.. comment
 C:  surface = cairo_ps_surface_create (filename, width_in_points,
 			               height_in_points);
 Py: surface = cairo.PSSurface  (f, width_in_points, height_in_points)
     where 'f' is a filename, a file object, or a file-like object

   .. method:: dsc_begin_page_setup()

   .. method:: dsc_begin_setup()

   .. method:: dsc_comment()

   .. method:: get_eps()

   .. method:: ps_level_to_string()

   .. method:: restrict_to_level()

   .. method:: set_eps()

   .. method:: set_size()


class SVGSurface(:class:`Surface`)
==================================

.. class:: SVGSurface

.. comment
 Py: surface = cairo.SVGSurface (f, width_in_points, height_in_points)
     where 'f' is a filename, a file object, or a file-like object

 C:  surface = cairo_svg_surface_create (filename, width_in_points,
 				        height_in_points);
..


class Win32Surface(:class:`Surface`)
====================================

.. class:: Win32Surface



class XlibSurface(:class:`Surface`)
===================================

.. class:: XlibSurface

   *XlibSurface* cannot be instantiated directly. But an XlibSurface instance
   can be returned from a function call when using pygtk.

   .. method:: get_depth()

   .. method:: get_height()

   .. method:: get_width()
