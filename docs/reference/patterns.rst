.. _patterns:

********
Patterns
********

.. currentmodule:: cairo


Patterns are the paint with which cairo draws. The primary use of patterns is
as the source for all cairo drawing operations, although they can also be used
as masks, that is, as the brush too.

A cairo *Pattern* is created by using one of the *PatternType* constructors
listed below, or implicitly through *Context.set_source_<type>()* methods.


class Pattern()
===============

.. autoclass:: Pattern
    :members:
    :undoc-members:


class SolidPattern(:class:`Pattern`)
====================================

.. autoclass:: SolidPattern
    :members:
    :undoc-members:

    .. automethod:: __init__


class SurfacePattern(:class:`Pattern`)
======================================

.. autoclass:: SurfacePattern
    :members:
    :undoc-members:

    .. automethod:: __init__


class Gradient(:class:`Pattern`)
================================

.. autoclass:: Gradient
    :members:
    :undoc-members:


class LinearGradient(:class:`Gradient`)
=======================================

.. autoclass:: LinearGradient
    :members:
    :undoc-members:

    .. automethod:: __init__


class RadialGradient(:class:`Gradient`)
=======================================

.. autoclass:: RadialGradient
    :members:
    :undoc-members:

    .. automethod:: __init__


class MeshPattern(:class:`Pattern`)
===================================

.. autoclass:: MeshPattern
    :members:
    :undoc-members:

    .. automethod:: __init__


class RasterSourcePattern(:class:`Pattern`)
===========================================

.. autoclass:: RasterSourcePattern
    :members:
    :undoc-members:

    .. automethod:: __init__
