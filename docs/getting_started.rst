===============
Getting Started
===============

Installation:

* ``pip3 install pycairo``

Installing Pycairo requires pkg-config and cairo including its headers. Here
are some examples on how to install those for some platforms:

* Ubuntu/Debian: ``sudo apt install libcairo2-dev pkg-config python3-dev``
* macOS/Homebrew: ``brew install cairo pkg-config``
* Arch Linux: ``sudo pacman -S cairo pkgconf``
* Fedora: ``sudo dnf install cairo-devel pkg-config python3-devel``
* openSUSE: ``sudo zypper install cairo-devel pkg-config python3-devel``

To verify that the installation works run the following Python code:

.. code:: python

    import cairo

FAQ
---

**My cairo import is failing after installing it with pip**

pip caches Python wheels from earlier installs by default and if the cairo
version the wheel was built against ist different from the one you run against
then this might lead to errors. You can force pip to re-build and re-install
pycairo in this case:

::

    pip3 install --force-reinstall --no-cache-dir pycairo
