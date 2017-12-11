==========================
Frequently Asked Questions
==========================

The paths in the installed pkg-config file are wrong. Why?
    Multiple reasons:

    * This can happen if you install from a Python wheel. Since you
      can't pass a prefix to setup.py bdist_wheel you don't have control over
      the paths in the .pc file.

    * When installing with pip: pip will sometimes create a wheel and cache
      it for future installations. In case the next installation is to a
      different prefix the .pc file contains the paths of the first
      installation.

    If you can think of any way to improve this, please tell us :)
