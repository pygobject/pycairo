# -*- python -*-

import os

APPNAME='pycairo'
VERSION='1.8.9'
cairo_version_required = '1.8.8'
srcdir = '.'
blddir = '../%s-build' % APPNAME

d = srcdir


def set_options(opt):
  print('  %s/set_options' %d)
  opt.tool_options('compiler_cc')
  opt.tool_options('python') # options for disabling pyc or pyo compilation


def init():
  print('  %s/init' %d)


def configure(conf):
  print('  %s/configure' %d)

  env = conf.env
  conf.check_tool('misc')
  conf.check_tool('compiler_cc')
  conf.check_tool('python')
  conf.check_python_version((2,6,0))
  conf.check_python_headers()
  conf.check_cfg(package='cairo', atleast_version=cairo_version_required,
                 args='--cflags --libs')

  # add gcc options
  if env['CC_NAME'] == 'gcc':
    for opt in ('-std=c99', '-Wall'):
      if opt not in env['CCFLAGS']:
        env.append_value('CCFLAGS', opt)

  version = [int(s) for s in VERSION.split('.')]
  conf.define('VERSION', VERSION)
  conf.define('PYCAIRO_VERSION_MAJOR', version[0])
  conf.define('PYCAIRO_VERSION_MINOR', version[1])
  conf.define('PYCAIRO_VERSION_MICRO', version[2])

  conf.write_config_header('src/config.h')


def build(bld):
  print('  %s/build' %d)
  bld.add_subdirs('src')

  # generate and install the .pc file
  obj = bld.new_task_gen('subst')
  obj.source = 'pycairo.pc.in'
  obj.target = 'pycairo.pc'
  obj.dict = {
    'VERSION'   : VERSION,
    'prefix'    : bld.env['PREFIX'],
    'includedir': os.path.join(bld.env['PREFIX'], 'include'),
    }
  obj.install_path = os.path.join(bld.env['PREFIX'], 'lib', 'pkgconfig')


def dist():  # create archives of project
  print('  %s/dist' %d)


def shutdown():
  print('  %s/shutdown' %d)
