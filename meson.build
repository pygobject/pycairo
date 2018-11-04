project(
  'pycairo', 'c',
  version: '1.18.0',
  meson_version: '>= 0.47.0',
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
  copy: true,
)

cc = meson.get_compiler('c')

main_c_args = []
if cc.get_id() == 'msvc'
# Adapted from msvc_recommended_pragmas.h, since we don't depend on GLib here
# Warnings that we want to look out for
  main_c_args += [
    '-we4002', # too many actual parameters for macro
    '-we4003', # not enough actual parameters for macro
    '-w14010', # single-line comment contains line-continuation character
    '-we4013', # 'function' undefined; assuming extern returning int
    '-w14016', # no function return type; using int as default
    '-we4020', # too many actual parameters
    '-we4021', # too few actual parameters
    '-we4027', # function declared without formal parameter list
    '-we4029', # declared formal parameter list different from definition
    '-we4033', # 'function' must return a value
    '-we4035', # 'function' : no return value
    '-we4045', # array bounds overflow
    '-we4047', # different levels of indirection
    '-we4049', # terminating line number emission
    '-we4053', # An expression of type void was used as an operand
    '-we4071', # no function prototype given
    '-we4150',
    '-we4819', # The file contains a character that cannot be represented in the current code page
  ]

# Silence warnings for stuff that should not really raise concern
  main_c_args += [
    '-wd4101', # unreferenced local variable
    '-wd4244', # No possible loss of data warnings
    '-wd4305', # No truncation from int to char warnings
  ]

# Silence warnings for using non-MS CRT stuff
  main_c_args += [
    '-D_CRT_SECURE_NO_WARNINGS',
    '-D_CRT_NONSTDC_NO_WARNINGS'
  ]
else
  main_c_args += [
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
endif

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
