#!/usr/bin/env python

import io
import subprocess
import sys
import os
import errno

if os.environ.get("PYCAIRO_SETUPTOOLS"):
    # for testing
    import setuptools
    setuptools

from distutils.core import Extension, setup, Command, Distribution
from distutils.ccompiler import new_compiler
from distutils import log
from distutils import sysconfig


PYCAIRO_VERSION = '1.16.2'
CAIRO_VERSION_REQUIRED = '1.13.1'
XPYB_VERSION_REQUIRED = '1.3'


def get_command_class(name):
    # in case pip loads with setuptools this returns the extended commands
    return Distribution({}).get_command_class(name)


def _check_output(command):
    try:
        return subprocess.check_output(command)
    except OSError as e:
        if e.errno == errno.ENOENT:
            raise SystemExit(
                "%r not found.\nCommand %r" % (command[0], command))
        raise SystemExit(e)
    except subprocess.CalledProcessError as e:
        raise SystemExit(e)


def pkg_config_version_check(pkg, version):
    command = [
        "pkg-config",
        "--print-errors",
        "--exists",
        '%s >= %s' % (pkg, version),
    ]

    _check_output(command)


def pkg_config_parse(opt, pkg):
    command = ["pkg-config", opt, pkg]
    ret = _check_output(command)
    output = ret.decode()
    opt = opt[-2:]
    return [x.lstrip(opt) for x in output.split()]


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
        self.install_base = None
        self.install_data = None
        self.compiler_type = None
        self.outfiles = []

    def finalize_options(self):
        self.set_undefined_options(
            'install_lib',
            ('install_base', 'install_base'),
            ('install_data', 'install_data'),
        )

        self.set_undefined_options(
            'build_ext',
            ('compiler_type', 'compiler_type'),
        )

    def get_outputs(self):
        return self.outfiles

    def get_inputs(self):
        return []

    def run(self):
        # https://github.com/pygobject/pycairo/issues/83
        # The pkg-config file contains absolute paths depending on the
        # prefix. pip uses wheels as cache and when installing with --user
        # and then to a virtualenv, the wheel gets reused containing the
        # wrong paths. So in case bdist_wheel is used, just skip this command.
        cmd = self.distribution.get_command_obj("bdist_wheel", create=False)
        if cmd is not None:
            log.info(
                "Skipping install_pkgconfig, not supported with bdist_wheel")
            return

        # same for bdist_egg
        cmd = self.distribution.get_command_obj("bdist_egg", create=False)
        if cmd is not None:
            log.info(
                "Skipping install_pkgconfig, not supported with bdist_egg")
            return

        if self.compiler_type == "msvc":
            log.info(
                "Skipping install_pkgconfig, not supported with MSVC")
            return

        python_lib = sysconfig.get_python_lib(True, True, self.install_data)
        pkgconfig_dir = os.path.join(os.path.dirname(python_lib), 'pkgconfig')
        self.mkpath(pkgconfig_dir)

        pcname = "py3cairo.pc" if sys.version_info[0] == 3 else "pycairo.pc"
        target = os.path.join(pkgconfig_dir, pcname)

        log.info("Writing %s" % target)
        log.info("pkg-config prefix: %s" % self.install_base)
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
                "prefix": self.install_base,
                "version": PYCAIRO_VERSION,
                "py_version": sys.version_info[0]}).encode("utf-8"))

        self.outfiles.append(target)


class install_pycairo_header(Command):
    description = "install pycairo header"
    user_options = []

    def initialize_options(self):
        self.install_data = None
        self.install_lib = None
        self.force = None
        self.outfiles = []

    def finalize_options(self):
        self.set_undefined_options(
            'install_lib',
            ('install_data', 'install_data'),
            ('install_lib', 'install_lib'),
        )

        self.set_undefined_options(
            'install',
            ('force', 'force'),
        )

    def get_outputs(self):
        return self.outfiles

    def get_inputs(self):
        return [os.path.join('cairo', 'pycairo.h')]

    def run(self):
        # https://github.com/pygobject/pycairo/issues/92
        # https://github.com/pygobject/pycairo/issues/98
        hname = "py3cairo.h" if sys.version_info[0] == 3 else "pycairo.h"
        source = self.get_inputs()[0]

        # for things using get_include()
        lib_hdir = os.path.join(self.install_lib, "cairo", "include")
        self.mkpath(lib_hdir)
        lib_header_path = os.path.join(lib_hdir, hname)
        (out, _) = self.copy_file(source, lib_header_path)
        self.outfiles.append(out)

        cmd = self.distribution.get_command_obj("bdist_wheel", create=False)
        if cmd is not None:
            return
        cmd = self.distribution.get_command_obj("bdist_egg", create=False)
        if cmd is not None:
            return

        # for things using pkg-config
        data_hdir = os.path.join(self.install_data, "include", "pycairo")
        self.mkpath(data_hdir)
        header_path = os.path.join(data_hdir, hname)
        (out, _) = self.copy_file(source, header_path)
        self.outfiles.append(out)


du_install_lib = get_command_class("install_lib")


class install_lib(du_install_lib):

    def initialize_options(self):
        self.install_base = None
        self.install_lib = None
        self.install_data = None
        du_install_lib.initialize_options(self)

    def finalize_options(self):
        du_install_lib.finalize_options(self)
        self.set_undefined_options(
            'install',
            ('install_base', 'install_base'),
            ('install_lib', 'install_lib'),
            ('install_data', 'install_data'),
        )

    def run(self):
        du_install_lib.run(self)
        # bdist_egg doesn't run install, so run our commands here instead
        self.run_command("install_pkgconfig")
        self.run_command("install_pycairo_header")


du_build_ext = get_command_class("build_ext")


class build_ext(du_build_ext):

    user_options = du_build_ext.user_options + [
        ("enable-xpyb", None, "Build with xpyb support (default=disabled)"),
    ]

    def initialize_options(self):
        du_build_ext.initialize_options(self)
        self.enable_xpyb = None
        self.compiler_type = None

    def finalize_options(self):
        du_build_ext.finalize_options(self)

        self.set_undefined_options(
            'build',
            ('enable_xpyb', 'enable_xpyb'),
        )

        self.compiler_type = new_compiler(compiler=self.compiler).compiler_type

    def run(self):
        ext = self.extensions[0]

        # If we are using MSVC, don't use pkg-config,
        # just assume that INCLUDE and LIB contain
        # the paths to the Cairo headers and libraries,
        # respectively.
        if self.compiler_type == "msvc":
            ext.libraries += ['cairo']
        else:
            pkg_config_version_check('cairo', CAIRO_VERSION_REQUIRED)
            ext.include_dirs += pkg_config_parse('--cflags-only-I', 'cairo')
            ext.library_dirs += pkg_config_parse('--libs-only-L', 'cairo')
            ext.libraries += pkg_config_parse('--libs-only-l', 'cairo')
            if sys.version_info[0] == 2:
                # Some python setups don't pass -fno-strict-aliasing,
                # while MACROS like Py_RETURN_TRUE require it.
                ext.extra_compile_args += ["-fno-strict-aliasing"]

            if os.environ.get("PYCAIRO_WARN"):
                ext.extra_compile_args += [
                    "-Wall",
                    "-Wundef",
                    "-Wextra",
                    "-Wno-missing-field-initializers",
                    "-Wno-unused-parameter",
                    "-Wnested-externs",
                    "-Wpointer-arith",
                    "-Wno-missing-field-initializers",
                    "-Wdeclaration-after-statement",
                    "-Wformat=2",
                    "-Wold-style-definition",
                    "-Wcast-align",
                    "-Wformat-nonliteral",
                    "-Wformat-security",
                    "-Wsign-compare",
                    "-Wstrict-aliasing",
                    "-Wshadow",
                    "-Winline",
                    "-Wpacked",
                    "-Wmissing-format-attribute",
                    "-Wmissing-noreturn",
                    "-Winit-self",
                    "-Wunused-but-set-variable",
                    "-Warray-bounds",
                    "-Wimplicit-function-declaration",
                    "-Wreturn-type",
                    "-Wconversion",
                    "-Wno-unknown-warning-option",
                ]

                if sys.version_info[:2] not in [(3, 3), (3, 4)]:
                    ext.extra_compile_args += [
                        "-Wswitch-default",
                    ]

        if self.enable_xpyb:
            if sys.version_info[0] != 2:
                raise SystemExit("xpyb only supported with Python 2")
            pkg_config_version_check("xpyb", XPYB_VERSION_REQUIRED)

            ext.define_macros += [("HAVE_XPYB", None)]
            ext.include_dirs += pkg_config_parse('--cflags-only-I', 'xpyb')
            ext.library_dirs += pkg_config_parse('--libs-only-L', 'xpyb')
            ext.libraries += pkg_config_parse('--libs-only-l', 'xpyb')

        du_build_ext.run(self)


du_build = get_command_class("build")


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


def main():

    cairo_ext = Extension(
        name='cairo._cairo',
        sources=[
            'cairo/device.c',
            'cairo/bufferproxy.c',
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
            'cairo/misc.c',
            'cairo/glyph.c',
            'cairo/rectangle.c',
            'cairo/textcluster.c',
            'cairo/textextents.c',
        ],
        depends=[
            'cairo/compat.h',
            'cairo/private.h',
            'cairo/pycairo.h',
        ],
        define_macros=[
            ("PYCAIRO_VERSION_MAJOR", PYCAIRO_VERSION.split('.')[0]),
            ("PYCAIRO_VERSION_MINOR", PYCAIRO_VERSION.split('.')[1]),
            ("PYCAIRO_VERSION_MICRO", PYCAIRO_VERSION.split('.')[2]),
        ],
    )

    with io.open('README.rst', encoding="utf-8") as h:
        long_description = h.read()

    cmdclass = {
        "build": build,
        "build_ext": build_ext,
        "install_lib": install_lib,
        "install_pkgconfig": install_pkgconfig,
        "install_pycairo_header": install_pycairo_header,
        "test": test_cmd,
    }

    setup(
        name="pycairo",
        version=PYCAIRO_VERSION,
        url="https://pycairo.readthedocs.io",
        description="Python interface for cairo",
        long_description=long_description,
        maintainer="Christoph Reiter",
        maintainer_email="reiter.christoph@gmail.com",
        ext_modules=[cairo_ext],
        packages=["cairo"],
        classifiers=[
            'Operating System :: OS Independent',
            'Programming Language :: Python :: 2',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: 3.3',
            'Programming Language :: Python :: 3.4',
            'Programming Language :: Python :: 3.5',
            'Programming Language :: Python :: 3.6',
            'Programming Language :: Python :: Implementation :: CPython',
            ('License :: OSI Approved :: '
             'GNU Lesser General Public License v2 (LGPLv2)'),
            'License :: OSI Approved :: Mozilla Public License 1.1 (MPL 1.1)',
        ],
        cmdclass=cmdclass,
    )


if __name__ == "__main__":
    main()
