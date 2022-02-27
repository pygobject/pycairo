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

.. autoclass:: Surface()
    :members:
    :undoc-members:

    .. automethod:: __init__


class ImageSurface(:class:`Surface`)
====================================

.. autoclass:: ImageSurface
    :members:
    :undoc-members:

    .. automethod:: __init__


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

.. autoclass:: RecordingSurface
    :members:
    :undoc-members:

    .. automethod:: __init__


class SVGSurface(:class:`Surface`)
==================================

The *SVGSurface* is used to render cairo graphics to SVG files and is a
multi-page vector surface backend

.. autoclass:: SVGSurface
    :members:
    :undoc-members:

    .. automethod:: __init__


class Win32Surface(:class:`Surface`)
====================================

The Microsoft Windows surface is used to render cairo graphics to Microsoft
Windows windows, bitmaps, and printing device contexts.

.. autoclass:: Win32Surface
    :members:
    :undoc-members:

    .. automethod:: __init__


class Win32PrintingSurface(:class:`Surface`)
============================================

.. autoclass:: Win32PrintingSurface
    :members:
    :undoc-members:

    .. automethod:: __init__


class XCBSurface(:class:`Surface`)
==================================

.. autoclass:: XCBSurface
    :members:
    :undoc-members:

    .. automethod:: __init__


class XlibSurface(:class:`Surface`)
===================================

.. autoclass:: XlibSurface
    :members:
    :undoc-members:


class ScriptSurface(:class:`Surface`)
=====================================

.. autoclass:: ScriptSurface
    :members:
    :undoc-members:

    .. automethod:: __init__


class TeeSurface(:class:`Surface`)
=====================================

.. autoclass:: TeeSurface
    :members:
    :undoc-members:

    .. automethod:: __init__
