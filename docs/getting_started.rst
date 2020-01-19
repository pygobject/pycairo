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
