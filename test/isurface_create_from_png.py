#!/usr/bin/env python
'''test cairo.ImageSurface.create_from_png() and
        cairo.Surface.write_to_png()
'''

import os
import tempfile

import cairo

if not (cairo.HAS_IMAGE_SURFACE and cairo.HAS_PNG_FUNCTIONS):
  raise SystemExit ('cairo was not compiled with ImageSurface and PNG support')

inFileName = os.path.join(os.path.dirname(__file__), '..', 'examples',
                          'cairo_snippets', 'data', 'romedalen.png')
surface = cairo.ImageSurface.create_from_png(inFileName)

# write to filename
_, outFileName = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
surface.write_to_png(outFileName)
print "see %s output file" % outFileName

# write to file object
h, outFileName = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
os.close(h)
f=file(outFileName, "wb")
surface.write_to_png(f)
f.close()
print "see %s output file" % outFileName

# write to object that has a "write" method
import StringIO
_, outFileName = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
buf = StringIO.StringIO()
surface.write_to_png(buf)
png_string = buf.getvalue()
buf.close()
f=file(outFileName, "wb")
f.write(png_string)
f.close()
print "see %s output file" % outFileName

# write to object that has a "write" method
_, outFileName = tempfile.mkstemp(prefix='pycairo_', suffix='.png')
import cStringIO
buf = cStringIO.StringIO()
surface.write_to_png(buf)
png_string = buf.getvalue()
buf.close()
f=file(outFileName, "wb")
f.write(png_string)
f.close()
print "see %s output file" % outFileName

# error test - to check the error message, should raise TypeError
#surface.write_to_png(101)
