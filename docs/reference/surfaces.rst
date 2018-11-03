.. _surfaces:

********
Surfaces
********

.. currentmodule:: cairo

.. comment block
   example reST:
   (add back '..' where required at column 0)
 . class:: module.C[(signature)]
   .. classmethod:: name(signature)
   .. staticmethod:: name(signature)
   .. method:: method(signature)

      :param p1: xxx
      :type p1: int
      :param p2: xxx
      :type p2: str
      :returns: xxx
      :rtype: list of strings
      :raises: xxx

      .. versionadded:: 1.6
   links:
     :data:`cairo.ANTIALIAS_SUBPIXEL`
     :class:`Context`
     :exc:`cairo.Error`
     :meth:`.copy_page`
     :meth:`Context.copy_page`

     ``ctx.rel_move_to(dx, dy)``   # code snippet


cairo.Surface is the abstract type representing all different drawing targets
that cairo can render to. The actual drawings are performed using a
:class:`Context`.

A cairo.Surface is created by using backend-specific constructors
of the form cairo.<XXX>Surface().

class Surface()
===============

.. class:: Surface()

   *Surface* is the abstract base class from which all the other surface
   classes derive. It cannot be instantiated directly.

   .. note::

      .. versionadded:: 1.17.0

         :class:`cairo.Surface` can be used as a context manager:

      .. code:: python

         # surface.finish() will be called on __exit__
         with cairo.SVGSurface("example.svg", 200, 200) as surface:
             pass

         # surface.unmap_image(image_surface) will be called on __exit__
         with surface.map_to_image(None) as image_surface:
             pass

   .. method:: copy_page()

      Emits the current page for backends that support multiple pages, but
      doesn't clear it, so that the contents of the current page will be
      retained for the next page.  Use :meth:`.show_page` if you want to get an
      empty page after the emission.

      :meth:`Context.copy_page` is a convenience function for this.

      .. versionadded:: 1.6

   .. method:: create_similar(content, width, height)

      :param cairo.Content content: the content for the new
        surface
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

      :returns: The content type of *Surface*,
        which indicates whether the *Surface* contains color and/or alpha
        information.
      :rtype: cairo.Content

      .. versionadded:: 1.2

   .. method:: get_device_offset()

      :returns: (x_offset, y_offset) a tuple of float

        * x_offset: the offset in the X direction, in device units
        * y_offset: the offset in the Y direction, in device units

      This method returns the previous device offset set by
      :meth:`.set_device_offset`.

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

   .. method:: supports_mime_type(mime_type)

      :param str mime_type: the mime type (:ref:`constants_MIME_TYPE`)
      :returns: :obj:`True` if surface supports mime_type, :obj:`False`
        otherwise
      :rtype: bool

      Return whether surface supports ``mime_type``.

      .. versionadded:: 1.12.0

   .. method:: set_mime_data(mime_type, data)

      :param mime_type: the MIME type of the image data
        (:ref:`constants_MIME_TYPE`)
      :type mime_type: str
      :param data: the image data to attach to the surface
      :type data: bytes

      Attach an image in the format ``mime_type`` to *Surface*.
      To remove the data from a surface,
      call this function with same mime type and :obj:`None` for data.

      The attached image (or filename) data can later be used
      by backends which support it
      (currently: PDF, PS, SVG and Win32 Printing surfaces)
      to emit this data instead of making a snapshot of the surface.
      This approach tends to be faster and requires less memory and disk space.

      The recognized MIME types are listed under :ref:`constants_MIME_TYPE`.

      See corresponding backend surface docs for details
      about which MIME types it can handle.
      Caution: the associated MIME data will be discarded
      if you draw on the surface afterwards.
      Use this function with care.

      .. versionadded:: 1.12.0

   .. method:: get_mime_data(mime_type)

      :param mime_type: the MIME type of the image data
        (:ref:`constants_MIME_TYPE`)
      :type mime_type: str
      :returns: :class:`bytes` or :obj:`None`

      Return mime data previously attached to surface
      with :meth:`set_mime_data` using the specified mime type.
      If no data has been attached with the given mime type,
      :obj:`None` is returned.

      .. versionadded:: 1.12.0

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
      :type fobj: filename (:obj:`pathlike`), file or file-like object
      :raises: :exc:`MemoryError` if memory could not be allocated for the operation

               :exc:`IOError` if an I/O error occurs while attempting to write
               the file

      Writes the contents of *Surface* to *fobj* as a PNG image.

   .. method:: create_for_rectangle(x, y, width, height)

      :param float x: the x-origin of the sub-surface from the top-left of the
         target surface (in device-space units)
      :param float y: the y-origin of the sub-surface from the top-left of the
         target surface (in device-space units)
      :param float width: width of the sub-surface (in device-space units)
      :param float height: height of the sub-surface (in device-space units)
      :returns: a new surface
      :rtype: cairo.Surface

      Create a new surface that is a rectangle within the target surface. All
      operations drawn to this surface are then clipped and translated onto
      the target surface. Nothing drawn via this sub-surface outside of its
      bounds is drawn onto the target surface, making this a useful method for
      passing constrained child surfaces to library routines that draw
      directly onto the parent surface, i.e. with no further backend
      allocations, double buffering or copies.

      .. note::

         The semantics of subsurfaces have not been finalized yet unless the
         rectangle is in full device units, is contained within the extents of
         the target surface, and the target or subsurface's device transforms
         are not changed.

      .. versionadded:: 1.12.0

   .. method:: create_similar_image(format, width, height)

      :param cairo.Format format: the format for the new surface
      :param int width: width of the new surface, (in device-space units)
      :param int height: height of the new surface, (in device-space units)
      :returns: a new image surface
      :rtype: cairo.ImageSurface

      Create a new image surface that is as compatible as possible for
      uploading to and the use in conjunction with an existing surface.
      However, this surface can still be used like any normal image surface.

      Initially the surface contents are all 0 (transparent if contents have
      transparency, black otherwise.)

      .. versionadded:: 1.12.0

   .. method:: has_show_text_glyphs()

      :returns: :obj:`True` if surface supports
         :meth:`Context.show_text_glyphs`, :obj:`False` otherwise
      :rtype: bool

      Returns whether the surface supports sophisticated
      :meth:`Context.show_text_glyphs` operations. That is, whether it
      actually uses the provided text and cluster data to a
      :meth:`Context.show_text_glyphs` call.

      Note: Even if this function returns :obj:`False`, a
      :meth:`Context.show_text_glyphs` operation targeted at surface will
      still succeed. It just will act like a :meth:`Context.show_glyphs`
      operation. Users can use this function to avoid computing UTF-8 text and
      cluster mapping if the target surface does not use it.

      .. versionadded:: 1.12.0

   .. method:: set_device_scale(x_scale, y_scale)

      :param float x_scale: a scale factor in the X direction
      :param float y_scale: a scale factor in the Y direction

      Sets a scale that is multiplied to the device coordinates determined by
      the CTM when drawing to surface . One common use for this is to render
      to very high resolution display devices at a scale factor, so that code
      that assumes 1 pixel will be a certain size will still work. Setting a
      transformation via :meth:`Context.translate` isn't sufficient to do
      this, since functions like :meth:`Context.device_to_user` will expose
      the hidden scale.

      .. versionadded:: 1.14.0

   .. method:: get_device_scale()

      :returns: (x_scale,y_scale) a 2-tuple of float

      This function returns the previous device offset set by
      :meth:`Surface.set_device_scale`.

      .. versionadded:: 1.14.0

   .. method:: get_device()

      :returns: the device or :obj:`None` if the surface does not have an
         associated device
      :rtype: Device

      This function returns the device for a surface.

      .. versionadded:: 1.14.0

   .. method:: map_to_image(extents)

      :param RectangleInt extents: limit the extraction to an rectangular
         region or :obj:`None` for the whole surface

      :returns: newly allocated image surface
      :rtype: ImageSurface
      :raises Error:

      Returns an image surface that is the most efficient mechanism for
      modifying the backing store of the target surface.

      Note, the use of the original surface as a target or source whilst it is
      mapped is undefined. The result of mapping the surface multiple times is
      undefined. Calling :meth:`Surface.finish` on the resulting image surface
      results in undefined behavior. Changing the device transform of the
      image surface or of surface before the image surface is unmapped results
      in undefined behavior.

      The caller must use :meth:`Surface.unmap_image` to destroy this image
      surface.

      .. versionadded:: 1.15.0

   .. method:: unmap_image(image)

      :param ImageSurface image: the currently mapped image

      Unmaps the image surface as returned from :meth:`Surface.map_to_image`.

      The content of the image will be uploaded to the target surface.
      Afterwards, the image is destroyed.

      Using an image surface which wasn't returned by
      :meth:`Surface.map_to_image` results in undefined behavior.

      .. versionadded:: 1.15.0


class ImageSurface(:class:`Surface`)
====================================

A *cairo.ImageSurface* provides the ability to render to memory buffers either
allocated by cairo or by the calling code. The supported image formats are
those defined in :class:`cairo.Format`.

.. class:: ImageSurface(format, width, height)

   :param cairo.Format format: format of pixels in the surface to create
   :param width: width of the surface, in pixels
   :param height: height of the surface, in pixels
   :returns: a new *ImageSurface*
   :raises: :exc:`MemoryError` in case of no memory

   Creates an *ImageSurface* of the specified format and dimensions. Initially
   the surface contents are all 0. (Specifically, within each pixel, each
   color or alpha channel belonging to format will be 0. The contents of bits
   within a pixel, but not belonging to the given format are undefined).

   .. classmethod:: create_for_data(data, format, width, height[, stride])

      :param data: a writable Python buffer/memoryview object
      :param cairo.Format format: the format of pixels in the
        buffer
      :param width: the width of the image to be stored in the buffer
      :param height: the height of the image to be stored in the buffer
      :param stride: the number of bytes between the start of rows in the
        buffer as allocated. If not given the value from
        :meth:`cairo.Format.stride_for_width` is used.
      :returns: a new *ImageSurface*
      :raises: :exc:`MemoryError` in case of no memory.

               :exc:`cairo.Error` in case of invalid *stride* value.

      Creates an *ImageSurface* for the provided pixel data. The initial
      contents of buffer will be used as the initial image contents; you must
      explicitly clear the buffer, using, for example, cairo_rectangle() and
      cairo_fill() if you want it cleared.

      Note that the *stride* may be larger than width*bytes_per_pixel to
      provide proper alignment for each pixel and row. This alignment is
      required to allow high-performance rendering within cairo. The correct
      way to obtain a legal stride value is to call
      :meth:`cairo.Format.stride_for_width` with the desired format and
      maximum image width value, and use the resulting stride value to
      allocate the data and to create the :class:`ImageSurface`. See
      :meth:`cairo.Format.stride_for_width` for example code.

   .. classmethod:: create_from_png(fobj)

      :param fobj:
        a :obj:`pathlike`, file, or file-like object of the PNG to load.
      :returns: a new *ImageSurface* initialized the contents to the given
        PNG file.

   .. staticmethod:: format_stride_for_width(format, width)

      See :meth:`cairo.Format.stride_for_width`.

      .. versionadded:: 1.6

   .. method:: get_data()

      :returns: a Python buffer object for the data of the *ImageSurface*, for
        direct inspection or modification. On Python 3 a memoryview object is
        returned.

      .. versionadded:: 1.2

   .. method:: get_format()

      :returns: the format of the *ImageSurface*.
      :rtype: cairo.Format

      .. versionadded:: 1.2

   .. method:: get_height()

      :returns: the height of the *ImageSurface* in pixels.

   .. method:: get_stride()

      :returns: the stride of the *ImageSurface* in bytes. The stride is the distance in bytes from the beginning of one row of the image data to the beginning of the next row.

   .. method:: get_width()

      :returns: the width of the *ImageSurface* in pixels.


class PDFSurface(:class:`Surface`)
==================================

The PDFSurface is used to render cairo graphics to Adobe PDF files and is a
multi-page vector surface backend.

.. class:: PDFSurface(fobj, width_in_points, height_in_points)

   :param fobj: a filename or writable file object. None may be used to specify no output. This will generate a *PDFSurface* that may be queried and used as a source, without generating a temporary file.
   :type fobj: None, :obj:`pathlike`, file or file-like object
   :param width_in_points: width of the surface, in points
     (1 point == 1/72.0 inch)
   :type  width_in_points: float
   :param height_in_points: height of the surface, in points
     (1 point == 1/72.0 inch)
   :type  height_in_points: float
   :returns: a new *PDFSurface* of the specified size in points to be written
     to *fobj*.
   :raises: :exc:`MemoryError` in case of no memory

   .. versionadded:: 1.2

   .. method:: set_size()

      :param width_in_points: new surface width, in points
        (1 point == 1/72.0 inch)
      :type  width_in_points: float
      :param height_in_points: new surface height, in points
        (1 point == 1/72.0 inch)
      :type  height_in_points: float

      Changes the size of a *PDFSurface* for the current (and subsequent) pages.

      This function should only be called before any drawing operations have
      been performed on the current page. The simplest way to do this is to
      call this function immediately after creating the surface or immediately
      after completing a page with either :meth:`Context.show_page` or
      :meth:`Context.copy_page`.

      .. versionadded:: 1.2

   .. method:: restrict_to_version(version)

      :param version: PDF version
      :type cairo.PDFVersion version: PDF version

      Restricts the generated PDF file to version . See :meth:`get_versions`
      for a list of available version values that can be used here.

      This function should only be called before any drawing operations have
      been performed on the given surface. The simplest way to do this is to
      call this function immediately after creating the surface.

      .. versionadded:: 1.12.0

   .. staticmethod:: get_versions()

      :returns: supported version list
      :rtype: list

      Retrieve the list of supported versions. See
      :meth:`restrict_to_version`.

      .. versionadded:: 1.12.0

   .. staticmethod:: version_to_string(version)

      :param version: PDF version
      :type cairo.PDFVersion version: PDF version
      :returns: the string associated to the given version
      :rtype: str
      :raises ValueError: if version isn't valid

      Get the string representation of the given version id. See
      :meth:`get_versions` for a way to get the list of valid version ids.

      .. versionadded:: 1.12.0

   .. method:: add_outline(parent_id, utf8, link_attribs, flags)

      :param int parent_id:
         the id of the parent item or :data:`PDF_OUTLINE_ROOT` if this is a top
         level item.
      :param text utf8:
         the name of the outline
      :param text link_attribs:
         the link attributes specifying where this outline links to
      :param PDFOutlineFlags flags:
         outline item flags
      :returns: the id for the added item.
      :rtype: int

      .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

   .. method:: set_metadata(metadata, utf8)

      :param PDFMetadata metadata: The metadata item to set.
      :param text utf8: metadata value

      Set document metadata. The :attr:`PDFMetadata.CREATE_DATE` and
      :attr:`PDFMetadata.MOD_DATE` values must be in ISO-8601 format:
      YYYY-MM-DDThh:mm:ss. An optional timezone of the form "[+/-]hh:mm" or
      "Z" for UTC time can be appended. All other metadata values can be any
      UTF-8 string.

      .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

   .. method:: set_page_label(utf8)

      :param text utf8: metadata value

      Set page label for the current page.

      .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

   .. method:: set_thumbnail_size(width, height)

      :param int width: Thumbnail width.
      :param int height: Thumbnail height

      Set the thumbnail image size for the current and all subsequent pages.
      Setting a width or height of 0 disables thumbnails for the current and
      subsequent pages.

      .. versionadded:: 1.18.0 Only available with cairo 1.15.10+


class PSSurface(:class:`Surface`)
=================================

The *PSSurface* is used to render cairo graphics to Adobe PostScript files and
is a multi-page vector surface backend.

.. class:: PSSurface(fobj, width_in_points, height_in_points)

   :param fobj: a filename or writable file object. None may be used to specify no output. This will generate a *PSSurface* that may be queried and used as a source, without generating a temporary file.
   :type fobj: None, :obj:`pathlike`, file or file-like object
   :param width_in_points: width of the surface, in points
     (1 point == 1/72.0 inch)
   :type  width_in_points: float
   :param height_in_points: height of the surface, in points
     (1 point == 1/72.0 inch)
   :type  height_in_points: float
   :returns: a new *PDFSurface* of the specified size in points to be written
     to *fobj*.
   :raises: :exc:`MemoryError` in case of no memory

   Note that the size of individual pages of the PostScript output can
   vary. See :meth:`.set_size`.

   .. method:: dsc_begin_page_setup()

      This method indicates that subsequent calls to
      :meth:`.dsc_comment` should direct comments to the PageSetup
      section of the PostScript output.

      This method call is only needed for the first page of a surface. It
      should be called after any call to :meth:`.dsc_begin_setup` and
      before any drawing is performed to the surface.

      See :meth:`.dsc_comment` for more details.

      .. versionadded:: 1.2

   .. method:: dsc_begin_setup()

      This function indicates that subsequent calls to :meth:`.dsc_comment`
      should direct comments to the Setup section of the PostScript output.

      This function should be called at most once per surface, and must be
      called before any call to :meth:`.dsc_begin_page_setup` and before any
      drawing is performed to the surface.

      See :meth:`.dsc_comment` for more details.

      .. versionadded:: 1.2

   .. method:: dsc_comment(comment)

      :param comment: a comment string to be emitted into the PostScript output
      :type comment: str

      Emit a comment into the PostScript output for the given surface.

      The comment is expected to conform to the PostScript Language
      Document Structuring Conventions (DSC). Please see that manual for
      details on the available comments and their meanings. In
      particular, the %%IncludeFeature comment allows a
      device-independent means of controlling printer device features. So
      the PostScript Printer Description Files Specification will also be
      a useful reference.

      The comment string must begin with a percent character (%) and the
      total length of the string (including any initial percent
      characters) must not exceed 255 characters. Violating either of
      these conditions will place *PSSurface* into an error state. But
      beyond these two conditions, this function will not enforce
      conformance of the comment with any particular specification.

      The comment string should not have a trailing newline.

      The DSC specifies different sections in which particular comments
      can appear. This function provides for comments to be emitted
      within three sections: the header, the Setup section, and the
      PageSetup section.  Comments appearing in the first two sections
      apply to the entire document while comments in the BeginPageSetup
      section apply only to a single page.

      For comments to appear in the header section, this function should
      be called after the surface is created, but before a call to
      :meth:`.dsc_begin_setup`.

      For comments to appear in the Setup section, this function should be
      called after a call to :meth:`.dsc_begin_setup` but before a call to
      :meth:`.dsc_begin_page_setup`.

      For comments to appear in the PageSetup section, this function should be
      called after a call to :meth:`.dsc_begin_page_setup`.

      Note that it is only necessary to call :meth:`.dsc_begin_page_setup` for
      the first page of any surface. After a call to :meth:`Context.show_page`
      or :meth:`Context.copy_page` comments are unambiguously directed to the
      PageSetup section of the current page. But it doesn't hurt to call this
      function at the beginning of every page as that consistency may make the
      calling code simpler.

      As a final note, cairo automatically generates several comments on
      its own. As such, applications must not manually generate any of
      the following comments:

      Header section: %!PS-Adobe-3.0, %Creator, %CreationDate, %Pages,
      %BoundingBox, %DocumentData, %LanguageLevel, %EndComments.

      Setup section: %BeginSetup, %EndSetup

      PageSetup section: %BeginPageSetup, %PageBoundingBox,
      %EndPageSetup.

      Other sections: %BeginProlog, %EndProlog, %Page, %Trailer, %EOF

      Here is an example sequence showing how this function might be used::

        surface = PSSurface (filename, width, height)
        ...
        surface.dsc_comment (surface, "%%Title: My excellent document")
        surface.dsc_comment (surface, "%%Copyright: Copyright (C) 2006 Cairo Lover")
        ...
        surface.dsc_begin_setup (surface)
        surface.dsc_comment (surface, "%%IncludeFeature: *MediaColor White")
        ...
        surface.dsc_begin_page_setup (surface)
        surface.dsc_comment (surface, "%%IncludeFeature: *PageSize A3")
        surface.dsc_comment (surface, "%%IncludeFeature: *InputSlot LargeCapacity")
        surface.dsc_comment (surface, "%%IncludeFeature: *MediaType Glossy")
        surface.dsc_comment (surface, "%%IncludeFeature: *MediaColor Blue")
        ... draw to first page here ..
        ctx.show_page (cr)
        ...
        surface.dsc_comment (surface, "%%IncludeFeature:  PageSize A5");
        ...

      .. versionadded:: 1.2

   .. method:: get_eps()

      :returns: True iff the *PSSurface* will output Encapsulated PostScript.

      .. versionadded:: 1.6

   .. staticmethod:: level_to_string(level)

      :param cairo.PSLevel level: a PS level
      :returns: the string associated to given level.
      :rtype: str

      Get the string representation of the given *level*.  See
      :meth:`get_levels` for a way to get the list of valid level
      ids.

      .. note:: Prior to 1.12 this was available under
         :meth:`ps_level_to_string`

      .. versionadded:: 1.12.0

   .. data:: ps_level_to_string

      Alias for :meth:`level_to_string`

      .. versionadded:: 1.6

   .. method:: restrict_to_level(level)

      :param cairo.PSLevel level: a PS level

      Restricts the generated PostSript file to *level*. See
      :meth:`get_levels` for a list of available level values that
      can be used here.

      This function should only be called before any drawing operations have
      been performed on the given surface. The simplest way to do this is to
      call this function immediately after creating the surface.

      .. versionadded:: 1.6

   .. method:: set_eps(eps)

      :param eps: True to output EPS format PostScript
      :type eps: bool

      If *eps* is True, the PostScript surface will output Encapsulated
      PostScript.

      This function should only be called before any drawing operations have
      been performed on the current page. The simplest way to do this is to
      call this function immediately after creating the surface. An
      Encapsulated PostScript file should never contain more than one page.

      .. versionadded:: 1.6

   .. method:: set_size(width_in_points, height_in_points)

      :param width_in_points: new surface width, in points (1 point == 1/72.0 inch)
      :param height_in_points: new surface height, in points (1 point == 1/72.0 inch)
      :type width_in_points: float
      :type height_in_points: float

      Changes the size of a PostScript surface for the current (and
      subsequent) pages.

      This function should only be called before any drawing operations
      have been performed on the current page. The simplest way to do
      this is to call this function immediately after creating the
      surface or immediately after completing a page with either
      :meth:`Context.show_page` or :meth:`Context.copy_page`.

      .. versionadded:: 1.2

   .. staticmethod:: get_levels()

      :returns: supported level list
      :rtype: list

      Retrieve the list of supported levels. See
      :meth:`restrict_to_level`.

      .. versionadded:: 1.12.0


class RecordingSurface(:class:`Surface`)
========================================

A *RecordingSurface* is a surface that records all drawing operations at the
highest level of the surface backend interface, (that is, the level of paint,
mask, stroke, fill, and show_text_glyphs). The recording surface can then be
"replayed" against any target surface by using it as a source surface.

If you want to replay a surface so that the results in target will be
identical to the results that would have been obtained if the original
operations applied to the recording surface had instead been applied to the
target surface, you can use code like this::

  cr = cairo.Context(target)
  cr.set_source_surface(recording_surface, 0.0, 0.0)
  cr.paint()

A *RecordingSurface* is logically unbounded, i.e. it has no implicit
constraint on the size of the drawing surface. However, in practice this is
rarely useful as you wish to replay against a particular target surface with
known bounds. For this case, it is more efficient to specify the target
extents to the recording surface upon creation.

The recording phase of the recording surface is careful to snapshot all
necessary objects (paths, patterns, etc.), in order to achieve accurate
replay.

.. class:: RecordingSurface(content, rectangle)

   :param cairo.Content content: the content for the new  surface
   :param cairo.Rectangle rectangle: or None to record unbounded operations.
   :returns: a new *RecordingSurface*

   Creates a *RecordingSurface* which can be used to record all drawing
   operations at the highest level (that is, the level of paint, mask, stroke,
   fill and show_text_glyphs). The *RecordingSurface* can then be "replayed"
   against any target surface by using it as a source to drawing operations.

   The recording phase of the *RecordingSurface* is careful to snapshot all
   necessary objects (paths, patterns, etc.), in order to achieve accurate
   replay.

   .. versionadded:: 1.11.0

   .. method:: ink_extents()

      ::rtype: (x0,y0,width,height) a 4-tuple of float

	* x0: the x-coordinate of the top-left of the ink bounding box
	* y0: the y-coordinate of the top-left of the ink bounding box
	* width: the width of the ink bounding box
	* height: the height of the ink bounding box

      Measures the extents of the operations stored within the
      *RecordingSurface*. This is useful to compute the required size of an
      *ImageSurface* (or equivalent) into which to replay the full sequence of
      drawing operations.

      .. versionadded:: 1.11.0


   .. method:: get_extents()

      :returns: a rectangle or :obj:`None` if the surface is unbounded.
      :rtype: Rectangle

      Get the extents of the recording-surface.

      .. versionadded:: 1.12.0


class SVGSurface(:class:`Surface`)
==================================

The *SVGSurface* is used to render cairo graphics to SVG files and is a
multi-page vector surface backend

.. class:: SVGSurface(fobj, width_in_points, height_in_points)

   :param fobj: a filename or writable file object. None may be used to specify no output. This will generate a *SVGSurface* that may be queried and used as a source, without generating a temporary file.
   :type fobj: None, :obj:`pathlike`, file or file-like object
   :param width_in_points: width of the surface, in points (1 point == 1/72.0 inch)
   :type  width_in_points: float
   :param height_in_points: height of the surface, in points (1 point == 1/72.0 inch)
   :type  height_in_points: float
   :returns: a new *SVGSurface* of the specified size in points to be written to *fobj*.
   :raises: :exc:`MemoryError` in case of no memory

   .. method:: restrict_to_version(version)

      :param version: SVG version
      :type cairo.SVGVersion version: SVG version

      Restricts the generated SVG file to version . See :meth:`get_versions`
      for a list of available version values that can be used here.

      This function should only be called before any drawing operations have
      been performed on the given surface. The simplest way to do this is to
      call this function immediately after creating the surface.

      .. versionadded:: 1.12.0

   .. staticmethod:: get_versions()

      :returns: supported version list
      :rtype: list

      Retrieve the list of supported versions. See
      :meth:`restrict_to_version`.

      .. versionadded:: 1.12.0

   .. staticmethod:: version_to_string(version)

      :param version: SVG version
      :type cairo.SVGVersion version: SVG version
      :returns: the string associated to the given version
      :rtype: str
      :raises ValueError: if version isn't valid

      Get the string representation of the given version id. See
      :meth:`get_versions` for a way to get the list of valid version ids.

      .. versionadded:: 1.12.0

   .. method:: get_document_unit()

      :returns: the SVG unit of the SVG surface.
      :rtype: SVGUnit

      Get the unit of the SVG surface.

      .. versionadded:: 1.18.0 Only available with cairo 1.15.10+

   .. method:: set_document_unit(unit)

      :param SVGUnit unit: SVG unit

      Use the specified unit for the width and height of the generated SVG
      file. See :class:`SVGUnit` for a list of available unit values that can
      be used here.

      This function can be called at any time before generating the SVG file.

      However to minimize the risk of ambiguities it's recommended to call it
      before any drawing operations have been performed on the given surface,
      to make it clearer what the unit used in the drawing operations is.

      The simplest way to do this is to call this function immediately after
      creating the SVG surface.

      Note if this function is never called, the default unit for SVG
      documents generated by cairo will be "pt". This is for historical
      reasons.

      .. versionadded:: 1.18.0 Only available with cairo 1.15.10+


class Win32Surface(:class:`Surface`)
====================================

The Microsoft Windows surface is used to render cairo graphics to Microsoft
Windows windows, bitmaps, and printing device contexts.

.. class:: Win32Surface(hdc)

   :param hdc: the DC to create a surface for
   :type hdc: int
   :returns: the newly created surface

   Creates a cairo surface that targets the given DC. The DC will be queried
   for its initial clip extents, and this will be used as the size of the
   cairo surface. The resulting surface will always be of format
   cairo.FORMAT_RGB24, see :class:`cairo.Format`.



class Win32PrintingSurface(:class:`Surface`)
============================================

The Win32PrintingSurface is a multi-page vector surface type.

.. class:: Win32PrintingSurface(hdc)

   :param hdc: the DC to create a surface for
   :type hdc: int
   :returns: the newly created surface

   Creates a cairo surface that targets the given DC. The DC will be queried
   for its initial clip extents, and this will be used as the size of the
   cairo surface. The DC should be a printing DC; antialiasing will be
   ignored, and GDI will be used as much as possible to draw to the surface.

   The returned surface will be wrapped using the paginated surface to provide
   correct complex rendering behaviour; :meth:`cairo.Surface.show_page` and
   associated methods must be used for correct output.


class XCBSurface(:class:`Surface`)
==================================

The XCB surface is used to render cairo graphics to X Window System windows
and pixmaps using the XCB library.

Note that the XCB surface automatically takes advantage of the X render
extension if it is available.

.. class:: XCBSurface

   :param connection: an XCB connection
   :param drawable: a X drawable
   :param visualtype: a X visualtype
   :param width: The surface width
   :param height: The surface height

   Creates a cairo surface that targets the given drawable (pixmap or window).

   .. note:: This methods works using xpyb.

   .. method:: set_size(width, height)

   :param width: The width of the surface
   :param height: The height of the surface

   Informs cairo of the new size of the X Drawable underlying the surface. For a surface created
   for a Window (rather than a Pixmap), this function must be called each time the size of the
   window changes. (For a subwindow, you are normally resizing the window yourself, but for a
   toplevel window, it is necessary to listen for ConfigureNotify events.)

   A Pixmap can never change size, so it is never necessary to call this function on a surface
   created for a Pixmap.


class XlibSurface(:class:`Surface`)
===================================

The XLib surface is used to render cairo graphics to X Window System windows
and pixmaps using the XLib library.

Note that the XLib surface automatically takes advantage of X render extension
if it is available.

.. class:: XlibSurface

   .. note:: *XlibSurface* cannot be instantiated directly because Python
      interaction with Xlib would require open source Python bindings to Xlib
      which provided a C API.
      However, an *XlibSurface* instance can be returned from a function call
      when using pygtk http://www.pygtk.org/.

   .. method:: get_depth()

      :returns: the number of bits used to represent each pixel value.

      .. versionadded:: 1.2

   .. method:: get_height()

      :returns: the height of the X Drawable underlying the surface in pixels.

      .. versionadded:: 1.2

   .. method:: get_width()

      :returns: the width of the X Drawable underlying the surface in pixels.

      .. versionadded:: 1.2


class ScriptSurface(:class:`Surface`)
=====================================

The script surface provides the ability to render to a native script that
matches the cairo drawing model. The scripts can be replayed using tools under
the util/cairo-script directory, or with cairo-perf-trace.


.. class:: ScriptSurface(script, content, width, height)

    :param cairo.ScriptDevice script: the script (output device)
    :param cairo.Content content: the content of the surface
    :param float width: width in pixels
    :param float height: height in pixels
    :rtype: cairo.ScriptSurface
    :raises cairo.Error:

    Create a new surface that will emit its rendering through ``script``.

    .. versionadded:: 1.14

    .. classmethod:: create_for_target(script, target)

        :param cairo.ScriptDevice script: the script (output device)
        :param cairo.Surface target: a target surface to wrap
        :rtype: cairo.ScriptSurface
        :raises cairo.Error:

        Create a proxy surface that will render to ``target`` and record the
        operations to ``device``.

        .. versionadded:: 1.14


class TeeSurface(:class:`Surface`)
=====================================

This surface supports redirecting all its input to multiple surfaces.

.. class:: TeeSurface(master)

    :param cairo.Surface master:
    :rtype: cairo.TeeSurface
    :raises cairo.Error:

    .. versionadded:: 1.14

    .. method:: add(target)

        :param cairo.Surface target:
        :raises cairo.Error:

        Add the surface

        .. versionadded:: 1.14

    .. method:: remove(target)

        :param cairo.Surface target:
        :raises cairo.Error:

        Remove the surface

        .. versionadded:: 1.14

    .. method:: index(index)

        :param int index:
        :rtype: cairo.Surface
        :raises cairo.Error:

        Returns the surface at index ``index``. The master surface is
        at index 0.

        .. versionadded:: 1.14
