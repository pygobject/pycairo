.. image:: https://cdn.rawgit.com/pygobject/pycairo/master/docs/images/pycairo.svg
   :align: center
   :width: 370px

|

Pycairo is a Python module providing bindings for the `cairo graphics library
<https://cairographics.org/>`__. It depends on **cairo >= 1.13.1** and works
with **Python 3.5+**. Pycairo, including this documentation, is licensed under
the `LGPL-2.1-only OR MPL-1.1 <https://spdx.org/ids-how>`__.

The Pycairo bindings are designed to match the cairo C API as closely as
possible, and to deviate only in cases which are clearly better implemented in
a more ‘Pythonic’ way.

.. code:: shell

    pip install pycairo

Installing Pycairo requires cairo including its headers. For more info see
"`Getting Started
<https://pycairo.readthedocs.io/en/latest/getting_started.html>`__".

----

.. image:: https://cdn.rawgit.com/pygobject/pycairo/master/docs/images/example.svg
   :align: right
   :width: 200px

.. code:: python

    import cairo

    with cairo.SVGSurface("example.svg", 200, 200) as surface:
        context = cairo.Context(surface)
        x, y, x1, y1 = 0.1, 0.5, 0.4, 0.9
        x2, y2, x3, y3 = 0.6, 0.1, 0.9, 0.5
        context.scale(200, 200)
        context.set_line_width(0.04)
        context.move_to(x, y)
        context.curve_to(x1, y1, x2, y2, x3, y3)
        context.stroke()
        context.set_source_rgba(1, 0.2, 0.2, 0.6)
        context.set_line_width(0.02)
        context.move_to(x, y)
        context.line_to(x1, y1)
        context.move_to(x2, y2)
        context.line_to(x3, y3)
        context.stroke()

----

Features of the Pycairo bindings:

* Provides an object oriented interface to cairo.
* Queries the error status of objects and translates them to exceptions.
* Provides a C API that can be used by other Python extensions.

If Pycairo is not what you need, have a look at `cairocffi
<https://cairocffi.readthedocs.io>`__, which is an API compatible package
using `cffi <https://cffi.readthedocs.io/>`__ or `Qahirah
<https://github.com/ldo/qahirah>`__, which is using `ctypes
<https://docs.python.org/3/library/ctypes.html>`__ and provides a more
"pythonic" API with less focus on matching the cairo C API.

For more information visit https://pycairo.readthedocs.io

.. image:: https://travis-ci.org/pygobject/pycairo.svg?branch=master
    :target: https://travis-ci.org/pygobject/pycairo

.. image:: https://dev.azure.com/pygobject/pycairo/_apis/build/status/pygobject.pycairo
  :target: https://dev.azure.com/pygobject/pycairo/_build/latest?definitionId=1

.. image:: https://codecov.io/gh/pygobject/pycairo/branch/master/graph/badge.svg
  :target: https://codecov.io/gh/pygobject/pycairo
