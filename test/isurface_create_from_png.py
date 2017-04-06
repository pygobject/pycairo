#!/usr/bin/env python
# test cairo.ImageSurface.create_from_png()

import cairo

surface = cairo.ImageSurface.create_from_png("/tmp/warpedtext.png")

# write to filename
surface.write_to_png("/tmp/t1.png")

# write to file object
f2=file("/tmp/t2.png", "w")
surface.write_to_png(f2)
f2.close()

# write to object that has a "write" method
import StringIO
buffer = StringIO.StringIO()
surface.write_to_png(buffer)
png_string = buffer.getvalue()
buffer.close()
f3=file("/tmp/t3.png", "w")
f3.write(png_string)
f3.close()

# write to object that has a "write" method
import cStringIO
buffer = cStringIO.StringIO()
surface.write_to_png(buffer)
png_string = buffer.getvalue()
buffer.close()
f4=file("/tmp/t4.png", "w")
f4.write(png_string)
f4.close()

# error test - to check the error message, should raise TypeError
#surface.write_to_png(101)
