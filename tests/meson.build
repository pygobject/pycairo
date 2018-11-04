test_sources = [
  'hypothesis_fspaths.py',
  '__init__.py',
  'test_api.py',
  'test_cmod.py',
  'test_context.py',
  'test_device.py',
  'test_enums.py',
  'test_error.py',
  'test_font.py',
  'test_glyph.py',
  'test_hypothesis.py',
  'test_matrix.py',
  'test_path.py',
  'test_pattern.py',
  'test_rectangle.py',
  'test_region.py',
  'test_surface_numpy.py',
  'test_surface.py',
  'test_surface_pygame.py',
  'test_textcluster.py',
  'test_textextents.py',
  'test_typing.py',
]

mod_sources = [
  join_paths('cmodule', 'cmodule.c'),
  join_paths('cmodule', 'cmodulelib.c'),
]

python.extension_module('cmod', mod_sources,
  dependencies : [python_dep, cairo_dep],
  install: false,
  include_directories: include_directories('../cairo'),
  c_args: pyext_c_args + main_c_args + ['-DPY_SSIZE_T_CLEAN'],
)

foreach python_file : test_sources
  configure_file(
    input: python_file,
    output: python_file,
    copy: true,
  )
endforeach

test(
  'tests', python,
  workdir: meson.build_root(),
  args: ['-m', 'pytest'],
)

test(
  'quality', python,
  workdir: meson.build_root(),
  args: ['-m', 'flake8'],
)
