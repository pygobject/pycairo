#!/usr/bin/env python

import subprocess
import sys
import os
from distutils.core import Extension, setup, Command
from distutils.command.build_ext import build_ext as du_build_ext
from distutils.command.install import install as du_install
from distutils.command.install_data import install_data as du_install_data
from distutils.command.build import build as du_build


PYCAIRO_VERSION = '1.13.1'
CAIRO_VERSION_REQUIRED = '1.12.0'
XPYB_VERSION_REQUIRED = '1.3'


def pkg_config_version_check(pkg, version):
    command = [
        "pkg-config",
        "--print-errors",
        "--exists",
        '%s >= %s' % (pkg, version),
    ]
    try:
        subprocess.check_output(command)
    except subprocess.CalledProcessError as e:
        raise SystemExit(e)


def pkg_config_parse(opt, pkg):
    command = ["pkg-config", opt, pkg]
    try:
        ret = subprocess.check_output(command)
    except subprocess.CalledProcessError as e:
        raise SystemExit(e)

    output = ret.decode()
    opt = opt[-2:]
    return [x.lstrip(opt) for x in output.split()]


def write_config_file(path, version):
    v = version.split('.')

    with open(path, 'w') as fo:
        fo.write("""\
/* Configuration header created by setup.py - do not edit */
#ifndef _CONFIG_H
#define _CONFIG_H 1

#define PYCAIRO_VERSION_MAJOR %s
#define PYCAIRO_VERSION_MINOR %s
#define PYCAIRO_VERSION_MICRO %s
#define VERSION "%s"

#endif /* _CONFIG_H */
""" % (v[0], v[1], v[2], version)
        )


class test_cmd(Command):
    description = "run tests"
    user_options = [
        ("enable-xpyb", None, "Build with xpyb support (default=disabled)"),
    ]

    def initialize_options(self):
        self.enable_xpyb = None

    def finalize_options(self):
        pass

    def run(self):
        import pytest

        # ensure the C extension is build inplace
        cmd = self.reinitialize_command("build_ext")
        if self.enable_xpyb is not None:
            cmd.enable_xpyb = self.enable_xpyb
        cmd.inplace = True
        cmd.ensure_finalized()
        cmd.run()

        status = pytest.main(["tests"])
        if status != 0:
            raise SystemExit(status)


class install_pkgconfig(Command):
    description = "install .pc file"
    user_options = []

    def initialize_options(self):
        self.install_data = None
        self.install_base = None
        self.outfiles = []

    def finalize_options(self):
        self.set_undefined_options(
            'install',
            ('install_data', 'install_data'),
            ('install_base', 'install_base'),
        )

    def get_outputs(self):
        return self.outfiles

    def get_inputs(self):
        return []

    def run(self):
        libdir = os.path.join(self.install_data, "lib")
        install_dir = os.path.join(libdir, "pkgconfig")
        self.mkpath(install_dir)

        if sys.version_info[0] == 3:
            target = os.path.join(install_dir, "py3cairo.pc")
        else:
            target = os.path.join(install_dir, "pycairo.pc")
        with open(target, "wb") as h:
            h.write((u"""\
prefix=%(prefix)s

Name: Pycairo
Description: Python %(py_version)d bindings for cairo
Version: %(version)s
Requires: cairo
Cflags: -I${prefix}/include/pycairo
Libs:
""" % {
                "prefix": self.install_base, "version": PYCAIRO_VERSION,
                "py_version": sys.version_info[0]}).encode("utf-8"))

        self.outfiles.append(target)


class install(du_install):

    sub_commands = du_install.sub_commands + [
        ("install_pkgconfig", lambda self: True),
    ]


class build_ext(du_build_ext):

    user_options = du_build_ext.user_options + [
        ("enable-xpyb", None, "Build with xpyb support (default=disabled)"),
    ]

    def initialize_options(self):
        du_build_ext.initialize_options(self)
        self.enable_xpyb = None

    def finalize_options(self):
        du_build_ext.finalize_options(self)

        self.set_undefined_options(
            'build',
            ('enable_xpyb', 'enable_xpyb'),
        )

    def run(self):
        pkg_config_version_check('cairo', CAIRO_VERSION_REQUIRED)

        if self.enable_xpyb:
            if sys.version_info[0] != 2:
                raise SystemExit("xpyb only supported with Python 2")
            pkg_config_version_check("xpyb", XPYB_VERSION_REQUIRED)
            ext = self.extensions[0]

            ext.define_macros += [("HAVE_XPYB", None)]
            ext.include_dirs += pkg_config_parse('--cflags-only-I', 'xpyb')
            ext.library_dirs += pkg_config_parse('--libs-only-L', 'xpyb')
            ext.libraries += pkg_config_parse('--libs-only-l', 'xpyb')

        script_dir = os.path.dirname(os.path.realpath(__file__))
        target = os.path.join(script_dir, "cairo", "config.h")
        write_config_file(target, PYCAIRO_VERSION)

        du_build_ext.run(self)


class build(du_build):

    user_options = du_build.user_options + [
        ("enable-xpyb", None, "Build with xpyb support (default=disabled)"),
    ]

    def initialize_options(self):
        du_build.initialize_options(self)
        self.enable_xpyb = False

    def finalize_options(self):
        du_build.finalize_options(self)
        self.enable_xpyb = bool(self.enable_xpyb)


class install_data(du_install_data):

    def copy_file(self, src, dst, *args, **kwargs):
        # XXX: rename target on the fly. ugly, but works
        if os.path.basename(src) == "pycairo.h" and sys.version_info[0] == 3:
            dst = os.path.join(dst, "py3cairo.h")
        return du_install_data.copy_file(self, src, dst, *args, **kwargs)


def main():

    extra_compile_args = []
    if sys.version_info[0] == 2:
        # Some python setups don't pass -fno-strict-aliasing, while MACROS like
        # Py_RETURN_TRUE require it.
        extra_compile_args.append("-fno-strict-aliasing")

    cairo_ext = Extension(
        name='cairo._cairo',
        sources=[
            'cairo/error.c',
            'cairo/cairomodule.c',
            'cairo/context.c',
            'cairo/font.c',
            'cairo/matrix.c',
            'cairo/path.c',
            'cairo/pattern.c',
            'cairo/region.c',
            'cairo/surface.c',
            'cairo/enums.c',
        ],
        include_dirs=pkg_config_parse('--cflags-only-I', 'cairo'),
        library_dirs=pkg_config_parse('--libs-only-L', 'cairo'),
        libraries=pkg_config_parse('--libs-only-l', 'cairo'),
        extra_compile_args=extra_compile_args,
    )

    setup(
        name="pycairo",
        version=PYCAIRO_VERSION,
        description="python interface for cairo",
        ext_modules=[cairo_ext],
        packages=["cairo"],
        data_files=[
            ('include/pycairo', ['cairo/pycairo.h']),
        ],
        cmdclass={
            "build": build,
            "build_ext": build_ext,
            "install": install,
            "install_pkgconfig": install_pkgconfig,
            "install_data": install_data,
            "test": test_cmd,
        },
    )


if __name__ == "__main__":
    main()
