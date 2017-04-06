#!/usr/bin/env python

from distutils.core import setup, Extension

def pkg_config_parse(pkg, opt):
    import os
    conf = os.popen('pkg-config %s %s' % (opt,pkg)).read()
    opt = opt[-2:]
    return [x.lstrip(opt) for x in conf.split()]

def get_include(pkg):
    return pkg_config_parse(pkg,'--cflags-only-I')

def get_lib_dirs(pkg):
    return pkg_config_parse(pkg,'--libs-only-L')

def get_libs(pkg):
    return pkg_config_parse(pkg,'--libs-only-l')

#~ print get_include('cairo')
#~ print get_lib_dirs('cairo')
#~ print get_libs('cairo')

_cairo = Extension(name = 'cairo._cairo',
            sources= ['cairo/cairomodule.c',
            'cairo/pycairo-context.c', 'cairo/pycairo-font.c',
            'cairo/pycairo-matrix.c', 'cairo/pycairo-path.c',
            'cairo/pycairo-pattern.c', 'cairo/pycairo-surface.c',
            ],
            include_dirs = get_include('cairo'),
            library_dirs = get_lib_dirs('cairo'),
            libraries = get_libs('cairo'),
        )

setup( name = "pycairo",
    version = "1.1.6",
    description = "python interface for cairo",

    packages = ['cairo'],
    ext_modules = [_cairo],
    )

