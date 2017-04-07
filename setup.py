#!/usr/bin/env python

import subprocess
import sys
import os
from distutils.core import Extension, setup, Command
from distutils.command.build_ext import build_ext as du_build_ext
from distutils.command.install import install as du_install
from distutils.command.install_data import install_data as du_install_data


PYCAIRO_VERSION = '1.10.1'
CAIRO_VERSION_REQUIRED = '1.10.2'


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
// Configuration header created by setup.py - do not edit
#ifndef _CONFIG_H
#define _CONFIG_H 1

#define PYCAIRO_VERSION_MAJOR %s
#define PYCAIRO_VERSION_MINOR %s
#define PYCAIRO_VERSION_MICRO %s
#define VERSION "%s"

#endif // _CONFIG_H
""" % (v[0], v[1], v[2], version)
        )


class test_cmd(Command):
    description = "run tests"
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        import pytest

        # ensure the C extension is build inplace
        cmd = self.reinitialize_command("build_ext")
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
        self.prefix = None
        self.outfiles = []

    def finalize_options(self):
        self.set_undefined_options(
            'install',
            ('install_data', 'install_data'),
            ('prefix', 'prefix'),
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
Cflags: -I%(prefix)s/include/pycairo
Libs:
""" % {
                "prefix": self.prefix, "version": PYCAIRO_VERSION,
                "py_version": sys.version_info[0]}).encode("utf-8"))

        self.outfiles.append(target)


class install(du_install):

    sub_commands = du_install.sub_commands + [
        ("install_pkgconfig", lambda self: True),
    ]


class build_ext(du_build_ext):

    def run(self):
        pkg_config_version_check('cairo', CAIRO_VERSION_REQUIRED)

        script_dir = os.path.dirname(os.path.realpath(__file__))
        target = os.path.join(script_dir, "cairo", "config.h")
        write_config_file(target, PYCAIRO_VERSION)

        du_build_ext.run(self)


class install_data(du_install_data):

    def copy_file(self, src, dst, *args, **kwargs):
        # XXX: rename target on the fly. ugly, but works
        if os.path.basename(src) == "pycairo.h" and sys.version_info[0] == 3:
            dst = os.path.join(dst, "py3cairo.h")
        return du_install_data.copy_file(self, src, dst, *args, **kwargs)


def main():

    if sys.platform == 'win32':
        runtime_library_dirs = []
    else:
        runtime_library_dirs = pkg_config_parse('--libs-only-L', 'cairo')

    cairo_ext = Extension(
        name='cairo._cairo',
        sources=[
            'cairo/cairomodule.c',
            'cairo/context.c',
            'cairo/font.c',
            'cairo/matrix.c',
            'cairo/path.c',
            'cairo/pattern.c',
            'cairo/surface.c',
        ],
        include_dirs=pkg_config_parse('--cflags-only-I', 'cairo'),
        library_dirs=pkg_config_parse('--libs-only-L', 'cairo'),
        libraries=pkg_config_parse('--libs-only-l', 'cairo'),
        runtime_library_dirs=runtime_library_dirs,
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
            "build_ext": build_ext,
            "install": install,
            "install_pkgconfig": install_pkgconfig,
            "install_data": install_data,
            "test": test_cmd,
        },
    )


if __name__ == "__main__":
    main()
