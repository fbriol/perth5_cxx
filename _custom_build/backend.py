# Copyright (c) 2024 CNES.
#
# This software is distributed by the CNES under a proprietary license.
# It is not public and cannot be redistributed or used without permission.
"""Custom build backend."""

import argparse
import sys

import setuptools.build_meta


def usage(args: dict[str, str | list[str] | None]) -> argparse.Namespace:
    """Parse the command line arguments.

    Args:
        args: Dictionary of arguments to parse.
    Returns:
        Parsed arguments.
    """
    parser = argparse.ArgumentParser("Custom build backend")
    parser.add_argument(
        "--cmake-args",
        help="Additional CMake arguments, separated by ;",
    )
    parser.add_argument(
        "--cxx-compiler",
        help="Specify the C++ compiler to use",
    )
    parser.add_argument(
        "--generator",
        help="Specify the CMake generator to use",
    )
    parser.add_argument(
        "--mkl-root",
        help="Specify the MKL installation directory",
    )
    parser.add_argument(
        "--mkl",
        help="Use MKL as the BLAS library",
    )
    return parser.parse_args(args=[f"--{k}={v}" for k, v in args.items()])


def decode_bool(value: str | None) -> bool:
    """Decode a boolean value.

    Args:
        value: The value to decode.
    Returns:
        bool: The decoded boolean value.
    """
    result = False
    if value is not None:
        result = value.lower() in {"1", "true", "yes"}
    return result


class _CustomBuildMetaBackend(setuptools.build_meta._BuildMetaBackend):
    """Custom build backend.

    This class is used to pass the option from pip to the setup.py script.

    Reference: https://setuptools.pypa.io/en/latest/build_meta.html
    """

    def run_setup(self, setup_script="setup.py") -> None:
        """Run the setup script.

        Args:
            setup_script: The path to the setup script.
        """
        args = usage(self.config_settings or {})  # type: ignore[arg-type]
        setuptools_args = []
        if args.cmake_args:
            setuptools_args.append(f"--cmake-args={args.cmake_args}")
        if args.cxx_compiler:
            setuptools_args.append(f"--cxx-compiler={args.cxx_compiler}")
        if args.generator:
            setuptools_args.append(f"--generator={args.generator}")
        if args.mkl_root:
            setuptools_args.append(f"--mkl-root={args.mkl_root}")
        if decode_bool(args.mkl):
            setuptools_args.append("--mkl=yes")

        if setuptools_args:
            first, last = sys.argv[:1], sys.argv[1:]
            sys.argv = [*first, "build_ext", *setuptools_args, *last]
        super().run_setup(setup_script)

    def build_wheel(
        self,
        wheel_directory,
        config_settings=None,
        metadata_directory=None,
    ) -> str:
        """Build the wheel.

        Args:
            wheel_directory: The directory to store the wheel.
            config_settings: The configuration settings.
            metadata_directory: The metadata directory.
        Returns:
            str: The path to the built wheel.
        """
        self.config_settings = config_settings
        return super().build_wheel(
            wheel_directory,
            config_settings,
            metadata_directory,
        )


#: Custom build backend
build_wheel = _CustomBuildMetaBackend().build_wheel
