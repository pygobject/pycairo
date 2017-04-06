***
FAQ
***

.. currentmodule:: cairo


Pycairo FAQ - Frequently Asked Questions
========================================

Q: Can I subclass pycairo classes?

A: Cairo, the C library, is not an object oriented library, so a Python
binding can never be a truly object oriented interface to cairo.  One way to
write the Python bindings for cairo would be as a single long list of module
functions - this would be the most accurate representation of the underlying C
library. Pycairo (and most other cairo language bindings?) instead chose to
implement the bindings using Context, Surface, Pattern, etc classes.  An
advantage is that the classes organise cairo into groups of similar functions.
A disadvantage is that creates an illusion that cairo is object oriented
library, and people are then tempted to create subclasses to override cairo
methods. When in fact there are no methods to override, just cairo functions
which can't be overridden.

The cairo documentation Appendix A "Creating a language binding for cairo"
section "Memory Management" describes why deriving from a Surface creates
problems and is best avoided.

cairo.Context can be subclassed.
All other pycairo subclasses cannot be subclassed.


Q: How do I use pycairo with numpy?

A: See test/isurface_create_for_data2.py


Q: How do I use pycairo with pygame?

A: See test/pygame-test1.py
       test/pygame-test2.py
