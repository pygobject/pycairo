.. _paths:

*****
Paths
*****

.. currentmodule:: cairo

class Path()
============

.. class:: Path()

   *Path* cannot be instantiated directly, it is created by calling
   :meth:`Context.copy_path` and :meth:`Context.copy_path_flat`.

   str(path) lists the path elements.

   See :class:`path attributes <cairo.PathDataType>`

   Path is an iterator.

   See examples/warpedtext.py for example usage.
