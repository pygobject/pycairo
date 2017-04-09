Pycairo is a Python module providing bindings for the `cairo graphics library
<https://cairographics.org/>`__.

The Pycairo bindings are designed to match the cairo C API as closely as
possible, and to deviate only in cases which are clearly better implemented in
a more ‘Pythonic’ way.

Features of the Pycairo bindings:

* Provides an object oriented interface to cairo.
* Queries the error status of objects and translates them to exceptions.
* Provides a C API that can be used by other Python extensions.

Pycairo depends on **cairo >= 1.10.2** and works with **Python 2.7+** as well
as **Python 3.3+**. If you are looking for bindings that work with PyPy have a
look at `cairocffi <https://cairocffi.readthedocs.io>`__. Pycairo is licensed
under the **LGPLv2.1** as well as the **MPLv1.1**.

For more information visit https://pycairo.readthedocs.io
