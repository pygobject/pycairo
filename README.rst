.. image:: https://cdn.rawgit.com/pygobject/pycairo/master/docs/images/pycairo.svg
   :align: center
   :width: 370px

|

Pycairo is a Python module providing bindings for the `cairo graphics library
<https://cairographics.org/>`__.

The Pycairo bindings are designed to match the cairo C API as closely as
possible, and to deviate only in cases which are clearly better implemented in
a more ‘Pythonic’ way.

Features of the Pycairo bindings:

* Provides an object oriented interface to cairo.
* Queries the error status of objects and translates them to exceptions.
* Provides a C API that can be used by other Python extensions.

Pycairo depends on **cairo >= 1.12** and works with **Python 2.7+** as well as
**Python 3.3+**. Pycairo, including this documentation, is licensed under the
**LGPLv2.1** as well as the **MPLv1.1**.

If Pycairo is not what you need, have a look at `cairocffi
<https://cairocffi.readthedocs.io>`__, which is an API compatible package
using `cffi <https://cffi.readthedocs.io/>`__ or `Qahirah
<https://github.com/ldo/qahirah>`__, which is using `ctypes
<https://docs.python.org/3/library/ctypes.html>`__ and provides a more
"pythonic" API with less focus on matching the cairo C API.

For more information visit https://pycairo.readthedocs.io

.. image:: https://travis-ci.org/pygobject/pycairo.svg?branch=master
    :target: https://travis-ci.org/pygobject/pycairo

.. image:: https://ci.appveyor.com/api/projects/status/9hurdbb19lg2i9xm/branch/master?svg=true
    :target: https://ci.appveyor.com/project/lazka/pycairo/branch/master
