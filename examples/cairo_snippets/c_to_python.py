#!/usr/bin/env python
"""
translate C <snippet>.cairo to Python <snippet>.py

;       -> ''
cairo_  -> cr.
'(cr, ' -> (     but not snippet_normalize (cr, width, height)
(cr)    -> ()
/*      -> #/*
CAIRO_  -> cairo.
"""

import sys


if len(sys.argv) != 2 or not sys.argv[1].endswith('.cairo'):
    raise SystemExit('usage: c_to_python.py <file>.cairo')

filename_in  = sys.argv[1]
filename_out = filename_in.replace('.cairo', '.py')

file_in  = file(filename_in)
file_out = file(filename_out, 'w')    

for line in file_in:
    line = line.replace(';', '')    # should be ';' and whitespace to EOL only -> \n
    if not line.startswith('snippet_'):
        line = line.replace('cairo_', 'cr.')
        line = line.replace('(cr, ',  '(')
        line = line.replace('(cr)',   '()')
        line = line.replace('/*',     '#/*')
        line = line.replace('    ',   '')
        line = line.replace('CAIRO_', 'cairo.')
    
    file_out.write (line)
