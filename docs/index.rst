.. raw:: html

    <div class=section>

.. image:: images/pycairo.svg
   :align: center
   :width: 370px

|
|

.. toctree::
   :maxdepth: 1
   :titlesonly:
   :hidden:

   changelog
   Tutorial <tutorial>
   reference/index
   pycairo_c_api
   resources
   faq

.. currentmodule:: cairo
.. title:: Overview

.. include:: ../README.rst
    :start-after: |

----

Tarballs:
    https://github.com/pygobject/pycairo/releases
Git repo:
    https://github.com/pygobject/pycairo
Bug tracker:
    https://github.com/pygobject/pycairo/issues
Mailing list:
    https://lists.cairographics.org/cgi-bin/mailman/listinfo/cairo

See the ":ref:`reference_index`" for further details.

To use the pycairo library::

    import cairo

To build/install the library::

    python2/3 setup.py build
    python2/3 setup.py install

To run the tests::

    python2/3 setup.py test

The Python 2 version supports `xpyb
<https://xcb.freedesktop.org/XcbPythonBinding/>`__ integration which is
disabled by default. To enable, build as follows::

    python2 setup.py build --enable-xpyb
    # and for running tests:
    python2 setup.py test --enable-xpyb

For examples of pycairo code see the 'examples' directory that comes with the
pycairo distribution.

For author information see the git history as well as the now deleted
"ChangeLog" file in the git history.
