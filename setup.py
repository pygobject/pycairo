#!/usr/bin/env python

import distutils.core      as dic
import distutils.dir_util  as dut
import distutils.file_util as fut
import io
import subprocess
import sys

pycairo_version        = '1.8.9'
cairo_version_required = '1.8.8'
pkgconfig_file = 'pycairo.pc'
config_file    = 'src/config.h'


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


def createPcFile(PcFile):
  print 'creating %s' % PcFile
  with open(PcFile, 'w') as fo:
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

def createConfigFile(ConfigFile):
  print 'creating %s' % ConfigFile
  v = pycairo_version.split('.')

  with open(ConfigFile, 'w') as fo:
    fo.write ("""\
// Configuration header created by setup.py - do not edit
#ifndef _CONFIG_H
#define _CONFIG_H 1

#define PYCAIRO_VERSION_MAJOR %s
#define PYCAIRO_VERSION_MICRO %s
#define PYCAIRO_VERSION_MINOR %s
#define VERSION "%s"

#endif // _CONFIG_H
""" % (v[0], v[1], v[2], pycairo_version)
            )


if sys.version_info < (2,6):
  raise SystemExit('Error: Python >= 2.6 is required')

pkg_config_version_check ('cairo', cairo_version_required)
if sys.platform == 'win32':
  runtime_library_dirs = []
else:
  runtime_library_dirs = pkg_config_parse('--libs-only-L', 'cairo')

createPcFile(pkgconfig_file)
createConfigFile(config_file)


cairo = dic.Extension(
  name = 'cairo._cairo',
  sources = ['src/cairomodule.c',
             'src/context.c',
             'src/font.c',
             'src/matrix.c',
             'src/path.c',
             'src/pattern.c',
             'src/surface.c',
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
  ext_modules = [cairo],
  data_files=[('include/pycairo',['src/pycairo.h']),
              ('lib/pkgconfig',[pkgconfig_file])],
  )
