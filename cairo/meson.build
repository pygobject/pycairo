python_sources = [
  '__init__.py',
  '__init__.pyi',
  'py.typed',
]

sources = [
  'bufferproxy.c',
  'cairomodule.c',
  'context.c',
  'device.c',
  'enums.c',
  'error.c',
  'font.c',
  'glyph.c',
  'matrix.c',
  'misc.c',
  'path.c',
  'pattern.c',
  'rectangle.c',
  'region.c',
  'surface.c',
  'textcluster.c',
  'textextents.c',
]

foreach python_file : python_sources
  configure_file(
    input: python_file,
    output: python_file,
    copy: true,
  )
endforeach

cairo_dep = dependency('cairo', version: cair_version_req, required: cc.get_id() != 'msvc')

if cc.get_id() == 'msvc' and not cairo_dep.found()
  if cc.has_header('cairo.h')
    cairo_dep = cc.find_library('cairo')
  endif
endif

python.install_sources(python_sources,
  pure : false,
  subdir : 'cairo'
)

header_file = configure_file(
  input: 'pycairo.h',
  output: 'py3cairo.h',
  copy: true,
)

install_headers(
  [header_file],
  install_dir: join_paths(python.get_install_dir(), 'cairo', 'include'),
)
install_headers([header_file], subdir: 'pycairo')

pyext = python.extension_module('_cairo', sources,
  dependencies : [cairo_dep],
  install: true,
  subdir : 'cairo',
  c_args: pyext_c_args + main_c_args,
)

pkg = import('pkgconfig')

pkg.generate(
  name: 'py3cairo',
  description: 'Python 3 bindings for cairo',
  subdirs: 'pycairo',
  requires: ['cairo'],
)

pycairo_dep = declare_dependency(
  version: pycairo_version,
  include_directories: [include_directories('.')],
)
