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

   getting_started
   changelog
   Tutorial <tutorial>
   reference/index
   c_api/index
   resources
   faq

.. currentmodule:: cairo
.. title:: Overview

.. include:: ../README.rst
    :start-after: |

----

PyPI:
    https://pypi.org/project/pycairo
Tarballs:
    https://github.com/pygobject/pycairo/releases
Git repo:
    https://github.com/pygobject/pycairo
Bug tracker:
    https://github.com/pygobject/pycairo/issues
Mailing list:
    https://lists.cairographics.org/cgi-bin/mailman/listinfo/cairo

See the ":ref:`reference_index`" for further details.

For examples of pycairo code see the 'examples' directory that comes with the
pycairo distribution.

----

Alternatives:

* `cairocffi <https://cairocffi.readthedocs.io>`__ provides a large subset of
  the pycairo API but instead of being implemented in C it uses `cffi
  <https://cffi.readthedocs.io/>`__ to talk to cairo. In case you'd prefer not
  to use a C extension then give this a try. Or if you use PyPy and want to
  benefit from the lower overhead of cffi compared to C extensions.
* `Qahirah <https://github.com/ldo/qahirah>`__ provides a more "pythonic" API
  with less focus on matching the cairo C API. It also isn't a C extension and
  uses the Python builtin `ctypes
  <https://docs.python.org/3/library/ctypes.html>`__ module to talk to cairo.
