============
Installation
============

Installing Pycairo requires pkg-config and cairo including its headers. Here
are some information of how to:

Ubuntu/Debian
************

.. code-block:: sh

        sudo apt install libcairo2-dev pkg-config python3-dev``


macOS/Homebrew
**************

.. code-block:: sh

        brew install cairo pkg-config


Arch Linux
**********

.. code-block:: sh

        sudo pacman -S cairo pkgconf
        
Fedora
******

.. code-block:: sh

        sudo dnf install cairo-devel pkg-config python3-devel


openSUSE
********

.. code-block:: sh

        sudo zypper install cairo-devel pkg-config python3-devel

Windows
*******

Wheels are available for Windows. So simply installing with pip will get you a
working version of pycairo. But if you need to use it outside of python then 
you should use no binary version of pycairo.


.. code-block:: python

        pip install --no-binary :all: pycairo

=================
Verifying Install
=================

To verify that the installation works run the following Python code:

.. code-block:: python

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
