project(
  'pycairo', 'c',
  version: '1.17.1',
  meson_version: '>= 0.46.0',
  default_options: [
    'warning_level=1',
    'buildtype=debugoptimized',
  ],
)

cair_version_req = '>=1.13.1'

pymod = import('python')
python = pymod.find_installation(get_option('python'))

configure_file(
  input: 'setup.cfg',
  output: 'setup.cfg',
  configuration: configuration_data(),
)

main_c_args = [
  '-Wall',
  '-Warray-bounds',
  '-Wcast-align',
  '-Wconversion',
  '-Wdeclaration-after-statement',
  '-Wextra',
  '-Wformat=2',
  '-Wformat-nonliteral',
  '-Wformat-security',
  '-Wimplicit-function-declaration',
  '-Winit-self',
  '-Winline',
  '-Wmissing-format-attribute',
  '-Wmissing-noreturn',
  '-Wnested-externs',
  '-Wold-style-definition',
  '-Wpacked',
  '-Wpointer-arith',
  '-Wreturn-type',
  '-Wshadow',
  '-Wsign-compare',
  '-Wstrict-aliasing',
  '-Wundef',
  '-Wunused-but-set-variable',
]

main_c_args += [
  '-Wno-missing-field-initializers',
  '-Wno-unused-parameter',
]

main_c_args += [
  '-fno-strict-aliasing',
  '-fvisibility=hidden',
]

if not ['3.3', '3.4'].contains(python.language_version())
  main_c_args += [
    '-Wswitch-default',
  ]
endif

cc = meson.get_compiler('c')
main_c_args = cc.get_supported_arguments(main_c_args)

pycairo_version = meson.project_version()
version_arr = pycairo_version.split('.')
pycairo_version_major = version_arr[0].to_int()
pycairo_version_minor = version_arr[1].to_int()
pycairo_version_micro = version_arr[2].to_int()

pyext_c_args = [
  '-DPYCAIRO_VERSION_MAJOR=@0@'.format(pycairo_version_major),
  '-DPYCAIRO_VERSION_MINOR=@0@'.format(pycairo_version_minor),
  '-DPYCAIRO_VERSION_MICRO=@0@'.format(pycairo_version_micro),
]

pkginfo_conf = configuration_data()
pkginfo_conf.set('VERSION', pycairo_version)
configure_file(input : 'PKG-INFO.in',
  output : 'pycairo-@0@.egg-info'.format(pycairo_version),
  configuration : pkginfo_conf,
  install_dir : python.get_install_dir())

subdir('cairo')
subdir('tests')
