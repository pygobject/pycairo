===============
Getting Started
===============

Installation:

* Python 2: ``pip2 install pycairo``
* Python 3: ``pip3 install pycairo``

Installing Pycairo requires cairo including its headers. Here are some
examples on how to install those for some platforms:

* Ubuntu/Debian: ``sudo apt install libcairo2-dev``
* macOS/Homebrew: ``brew install cairo``
* Arch Linux: ``sudo pacman -S cairo``
* Fedora: ``sudo dnf install cairo-devel``
* openSUSE: ``sudo zypper install cairo-devel``

To verify that the installation works run the following Python code:

.. code:: python

    import cairo
