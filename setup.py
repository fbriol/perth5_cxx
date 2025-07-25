#!/usr/bin/env python3
"""Setup script for the PERTH library."""

import os
import pathlib
import platform
import re
import sys
import sysconfig
import warnings

import setuptools
import setuptools.command.build_ext

# Working directory
WORKING_DIRECTORY = pathlib.Path(__file__).parent.absolute()

# OSX deployment target
OSX_DEPLOYMENT_TARGET = "10.14"

# Python version file
PY_VERSION = WORKING_DIRECTORY / "src" / "perth" / "version.py"

# C++ version file
CXX_VERSION = WORKING_DIRECTORY / "include" / "perth" / "version.hpp"

# Git empty tree SHA - represents the state before any commits exist
# This is the SHA-1 hash of an empty git tree object, used as a baseline
# when no previous tags exist for computing file differences
GIT_EMPTY_TREE_SHA = "4b825dc642cb6eb9a060e54bf8d69288fbee4904"


def compare_setuptools_version(required: tuple[int, ...]) -> bool:
    """Compare the version of setuptools with the required version."""
    current = tuple(map(int, setuptools.__version__.split(".")[:2]))
    return current >= required


def distutils_dirname(prefix=None, extname=None) -> pathlib.Path:
    """Returns the name of the build directory."""
    prefix = f"{prefix or 'lib'}.{sysconfig.get_platform()}"
    extname = "" if extname is None else os.sep.join(extname.split(".")[:-1])
    if compare_setuptools_version((62, 1)):
        return pathlib.Path(
            WORKING_DIRECTORY,
            "build",
            f"{prefix}-{sys.implementation.cache_tag}",
            extname,
        )
    return pathlib.Path(
        WORKING_DIRECTORY,
        "build",
        f"{prefix}-{sys.version_info[0]}.{sys.version_info[1]}",
        extname,
    )


def fetch_package_version() -> str:
    if not (WORKING_DIRECTORY / ".git").exists():
        with PY_VERSION.open() as stream:
            for line in stream:
                if line.startswith("__version__"):
                    return line.split("=")[1].strip()[1:-1]

    # Make sure that the working directory is the root of the project,
    # otherwise setuptools_scm will not be able to find the version number.
    os.chdir(WORKING_DIRECTORY)
    import setuptools_scm

    try:
        return setuptools_scm.get_version()
    except:  # noqa: E722
        warnings.warn(
            "Unable to find the version number with setuptools_scm.",
            RuntimeWarning,
            stacklevel=2,
        )
        return "0.0.0"


def update_version_library(release: str) -> None:
    """Update the version of the library."""
    if not (WORKING_DIRECTORY / ".git").exists():
        return

    pattern = re.compile(r"(?:(\d+)\.)?(?:(\d+)\.)?(\*|\d+)(\.dev\d+)?")
    match = pattern.match(release)
    if match is None:
        raise RuntimeError(f"Invalid version number: {release}")
    major, minor, patch, dev = match.groups()
    if minor is None:
        minor = patch
        patch = "0"
    if dev is None:
        dev = ""

    if CXX_VERSION.exists():
        with CXX_VERSION.open() as stream:
            existing = "".join(stream.read())
    else:
        existing = ""

    new = f"""/// @file perth/version.hpp
/// @brief Version of the library
#pragma once
/// Major version of the library
#define PERTH_VERSION_MAJOR {major}
/// Minor version of the library
#define PERTH_VERSION_MINOR {minor}
/// Patch version of the library
#define PERTH_VERSION_PATCH {patch}{dev}
"""
    if existing != "".join(new):
        with CXX_VERSION.open("w") as stream:
            stream.write(new)

    if PY_VERSION.exists():
        with PY_VERSION.open() as stream:
            existing = "".join(stream.read())
    else:
        existing = ""

    new = f'''
# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""
Get software version information
================================
"""
__version__ = "{release}"
'''
    if existing != "".join(new):
        with PY_VERSION.open("w") as stream:
            stream.write(new)


# pylint: disable=too-few-public-methods
class CMakeExtension(setuptools.Extension):
    """Python extension to build."""

    def __init__(self, name) -> None:
        super().__init__(name, sources=[])

    # pylint: enable=too-few-public-methods


# pylint: disable=too-many-instance-attributes
class BuildExt(setuptools.command.build_ext.build_ext):
    """Build everything needed to install."""

    user_options = setuptools.command.build_ext.build_ext.user_options
    user_options += [
        (
            "cmake-args=",
            None,
            "Additional CMake arguments, separated by ;",
        ),
        (
            "cxx-compiler=",
            None,
            "Specify the C++ compiler to use",
        ),
        (
            "generator=",
            None,
            "Specify the CMake generator to use",
        ),
        (
            "mkl-root=",
            None,
            "Specify the MKL installation directory",
        ),
        (
            "mkl=",
            None,
            "Use MKL as the BLAS library",
        ),
        (
            "reconfigure",
            None,
            "Force CMake to reconfigure the project",
        ),
    ]

    boolean_options = setuptools.command.build_ext.build_ext.boolean_options
    boolean_options += [
        "mkl",
    ]

    def initialize_options(self) -> None:
        """Set default values for all the options that this command
        supports."""
        super().initialize_options()
        # pylint: disable=attribute-defined-outside-init
        # The following attributes are defined outside the __init__ method to
        # follow the setuptools API.
        self.cmake_args = None
        self.cxx_compiler = None
        self.generator = None
        self.mkl = None
        self.mkl_root = None
        self.reconfigure = None
        # pylint: enable=attribute-defined-outside-init

    def run(self) -> None:
        """Carry out the action."""
        self.build_cmake(self.extensions)
        super().run()

    def set_mklroot(self) -> None:
        """Set the MKLROOT environment variable.

        This method sets the MKLROOT environment variable based on the
        following conditions:
        1. If the user has specified the `mkl_root` option, the MKLROOT
           environment variable is set to the value of this option.
        2. If the `mkl_root` option is not specified and the MKLROOT
           environment variable is not already set, the method checks for the
           presence of the `mkl.h` header file in the Python installation
           directory. If found, the MKLROOT environment variable is set to the
           Python installation directory.
        """
        if self.mkl_root is not None:
            os.environ["MKLROOT"] = self.mkl_root
        elif "MKLROOT" not in os.environ:
            mkl_header = pathlib.Path(sys.prefix, "include", "mkl.h")
            if not mkl_header.exists():
                mkl_header = pathlib.Path(
                    sys.prefix,
                    "Library",
                    "include",
                    "mkl.h",
                )

            if not mkl_header.exists():
                raise RuntimeError(
                    "MKLROOT environment variable is not set and MKL header "
                    "file is not found in the Python installation directory"
                )
            os.environ["MKLROOT"] = sys.prefix

    @staticmethod
    def conda_prefix() -> str | None:
        """Returns the conda prefix."""
        if "CONDA_PREFIX" in os.environ:
            return os.environ["CONDA_PREFIX"]
        return None

    def set_cmake_user_options(self) -> list[str]:
        """Set the CMake user options."""
        result: list[str] = []

        if self.cxx_compiler is not None:
            result.append("-DCMAKE_CXX_COMPILER=" + self.cxx_compiler)

        if self.mkl:
            self.set_mklroot()

        if self.cmake_args is not None:
            result += self.cmake_args.split(";")

        return result

    def build_cmake(self, ext: list[CMakeExtension]) -> None:
        """Execute cmake to build the Python extension."""
        # These dirs will be created in build_py, so if you don't have
        # any python sources to bundle, the dirs will be missing
        build_temp = pathlib.Path(WORKING_DIRECTORY, self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdirs = {
            str(
                pathlib.Path(
                    self.get_ext_fullpath(
                        item.name,
                    )
                ).parent.resolve()
            )
            for item in ext
        }
        if len(extdirs) != 1:
            raise RuntimeError(
                "All extensions should write to the same directory",
            )
        extdir = extdirs.pop()

        cfg = "Debug" if self.debug else "Release"

        cmake_args: list[str] = [
            "-DCMAKE_BUILD_TYPE=" + cfg,
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + extdir,
            "-DPython_EXECUTABLE=" + sys.executable,
            *self.set_cmake_user_options(),
        ]
        build_args = ["--config", cfg]

        is_windows = platform.system() == "Windows"

        if self.generator is not None:
            cmake_args.append("-G" + self.generator)
        elif is_windows:
            cmake_args.append("-G" + "Visual Studio 17 2022")

        if not is_windows:
            build_args += ["--", f"-j{os.cpu_count()}"]
            if platform.system() == "Darwin":
                cmake_args += [
                    f"-DCMAKE_OSX_DEPLOYMENT_TARGET={OSX_DEPLOYMENT_TARGET}",
                ]
        else:
            cmake_args += [
                "-DCMAKE_GENERATOR_PLATFORM=x64",
                f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
            ]
            build_args += ["--", "/m"]

        os.chdir(str(build_temp))

        # Configure CMake if needed or requested
        configure = (
            (self.reconfigure is not None)
            if pathlib.Path(
                build_temp,
                "CMakeFiles",
                "TargetDirectories.txt",
            ).exists()
            else True
        )

        if configure:
            self.spawn(["cmake", str(WORKING_DIRECTORY), *cmake_args])
        if not self.dry_run:
            target = [item.name.split(".")[-1] for item in ext]
            cmake_cmd = ["cmake", "--build", ".", "--target", *target]
            self.spawn(cmake_cmd + build_args)
        os.chdir(str(WORKING_DIRECTORY))

    # pylint: enable=too-many-instance-attributes


def typehints() -> list[tuple[str, list[str]]]:
    """Get the list of type information files."""
    pyi = []
    for root, _, files in os.walk(WORKING_DIRECTORY):
        pyi += [
            str(pathlib.Path(root, item).relative_to(WORKING_DIRECTORY))
            for item in files
            if item.endswith(".pyi")
        ]
    return [
        (
            str(
                pathlib.Path(
                    "perth",
                )
            ),
            pyi,
        )
    ]


def main() -> None:
    """Execute the setup."""
    release = fetch_package_version()
    update_version_library(release)
    setuptools.setup(
        cmdclass={
            "build_ext": BuildExt,
        },
        package_dir={"perth": "src/perth"},
        packages=setuptools.find_namespace_packages(
            where="src",
            include=["perth", "perth.*"],
        ),
        package_data={
            "perth": ["py.typed", "*.pyi"],
        },
        # data_files=typehints(),
        version=release,
        ext_modules=[
            CMakeExtension(name="perth._core"),
        ],
    )


if __name__ == "__main__":
    if platform.system() == "Darwin":
        os.environ["MACOSX_DEPLOYMENT_TARGET"] = OSX_DEPLOYMENT_TARGET
    main()
