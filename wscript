# -*- python -*-

import os

top = '.'
out = 'build_directory'
d = top

APPNAME='py2cairo'
VERSION='1.8.11'
cairo_version_required = '1.10.0'


def options(ctx):
  print('  %s/options()' %d)
  ctx.tool_options('gnu_dirs')
  ctx.tool_options('compiler_cc')
  ctx.tool_options('python') # options for disabling pyc or pyo compilation


def configure(ctx):
  print('  %s/configure()' %d)

  env = ctx.env
  ctx.check_tool('gnu_dirs')
  ctx.check_tool('compiler_cc')
  ctx.check_tool('python')
  ctx.check_python_version((2,6,0))
  ctx.check_python_headers()
  ctx.check_cfg(package='cairo', atleast_version=cairo_version_required,
                 args='--cflags --libs')

  # add gcc options
  if env['CC_NAME'] == 'gcc':
    env.append_unique('CCFLAGS', ['-std=c99', '-Wall'])

  version = [int(s) for s in VERSION.split('.')]
  ctx.define('VERSION', VERSION)
  ctx.define('PYCAIRO_VERSION_MAJOR', version[0])
  ctx.define('PYCAIRO_VERSION_MINOR', version[1])
  ctx.define('PYCAIRO_VERSION_MICRO', version[2])

  ctx.write_config_header('src/config.h')

  print("Configuration:")
  print("%-40s : %s" % ('PREFIX', env['PREFIX']))
  print("%-40s : %s" % ('LIBDIR', env['LIBDIR']))


def build(ctx):
  print('  %s/build()' %d)
  ctx.recurse('src')

  # generate and install the .pc file
  ctx(
    features = 'subst',
    source   = 'pycairo.pc.in',
    target   = 'pycairo.pc',
    dct      = {
      'VERSION'   : VERSION,
      'prefix'    : ctx.env['PREFIX'],
      'includedir': os.path.join(ctx.env['PREFIX'], 'include'),
      },
    install_path = os.path.join(ctx.env['LIBDIR'], 'pkgconfig'),
    )


def dist_hook():
  # remove unwanted files from the archive

  # individual files
  for f in [
    'RELEASING',
    'examples/cairo_snippets/c_to_python.py',
    'doc/html_docs_create.sh',
    'doc/html_docs_upload.sh',
    ]:
    os.remove(f)

  # rm examples/*.{pdf,png,ps,svg}
  D='examples'
  for f in os.listdir(D):
    if f.endswith(('.pdf', '.png', '.ps', '.svg')):
      os.remove(os.path.join(D, f))

  D='examples/cairo_snippets/snippets'
  for f in os.listdir(D):
    if f.endswith(('.pdf', '.png', '.ps', '.svg')):
      os.remove(os.path.join(D, f))
