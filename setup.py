#!/usr/bin/env python

import distutils.core      as dic
import distutils.dir_util  as dut
import distutils.file_util as fut
import os
import sys

pycairo_version        = '1.4.0'
cairo_version_required = '1.4.0'

# Notes:
# on Fedora Core 5 module is compiled with 'gcc -g' - why -g?
# later: replace os.popen() with subprocess module, new in Python 2.4

def pkg_config (opt, pkg):
    fo = os.popen ('pkg-config %s %s' % (opt, pkg))
    return fo.read(), fo.close()

def pkg_config_version_check (pkg, version):
    output, status = pkg_config ('--atleast-version=%s' % version, pkg)
    if status is None:
        print '%s version >= %s detected' % (pkg, version)
    else:
        raise SystemExit ('Error: %s version >= %s not found' % (pkg, version))

def pkg_config_parse (opt, pkg):
    output, status = pkg_config (opt, pkg)
    opt = opt[-2:]
    return [x.lstrip(opt) for x in output.split()]


pkg_config_version_check ('cairo', cairo_version_required)
if sys.platform == 'win32':
    runtime_library_dirs = []
else:
    runtime_library_dirs = pkg_config_parse('--libs-only-L', 'cairo')

pkgconfig_file='pycairo.pc'
print 'creating %s' % pkgconfig_file
fo = open (pkgconfig_file, 'w')
fo.write ("""\
prefix=%s

Name: Pycairo
Description: Python bindings for cairo
Version: %s
Requires: cairo
Cflags: -I${prefix}/include/pycairo
Libs:
""" % (sys.prefix, pycairo_version)
          )
fo.close()

cairo = dic.Extension(
    name = 'cairo._cairo',
     sources = ['cairo/cairomodule.c',
               'cairo/pycairo-context.c',
               'cairo/pycairo-font.c',
               'cairo/pycairo-matrix.c',
               'cairo/pycairo-path.c',
               'cairo/pycairo-pattern.c',
               'cairo/pycairo-surface.c',
               ],
    include_dirs = pkg_config_parse('--cflags-only-I', 'cairo'),
    library_dirs = pkg_config_parse('--libs-only-L', 'cairo'),
    libraries    = pkg_config_parse('--libs-only-l', 'cairo'),
    runtime_library_dirs = runtime_library_dirs,
    )

dic.setup(
    name = "pycairo",
    version = pycairo_version,
    description = "python interface for cairo",
    packages = ['cairo'],
    ext_modules = [cairo],
    data_files=[('include/pycairo',['cairo/pycairo.h']),
                ('lib/pkgconfig',[pkgconfig_file])],
    )
