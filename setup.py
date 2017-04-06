#!/usr/bin/env python

import distutils.core      as dic
import distutils.dir_util  as dut
import distutils.file_util as fut
import subprocess
import sys

pycairo_version        = '1.8.2'
cairo_version_required = '1.8.2'

def call(command):
    pipe = subprocess.Popen(command, shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    pipe.wait()
    return pipe

def pkg_config_version_check(pkg, version):
    pipe = call('pkg-config --print-errors --exists "%s >= %s"' %
                (pkg, version))
    if pipe.returncode == 0:
        print '%s >= %s detected' % (pkg, version)
    else:
        print pipe.stderr.read()
        raise SystemExit('Error: %s >= %s not found' % (pkg, version))

def pkg_config_parse(opt, pkg):
    pipe = call("pkg-config %s %s" % (opt, pkg))
    output = pipe.stdout.read()
    opt = opt[-2:]
    return [x.lstrip(opt) for x in output.split()]


if sys.version_info < (2,5):
    raise SystemExit('Error: Python >= 2.5 is required')

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
