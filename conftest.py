# Copyright (c) 2024 CNES.
#
# This software is distributed by the CNES under a proprietary license.
# It is not public and cannot be redistributed or used without permission.
import os
import pathlib
import sys
import sysconfig

import pytest

# Check Python requirement
MAJOR = sys.version_info[0]
MINOR = sys.version_info[1]

# Working directory
WORKING_DIRECTORY = pathlib.Path(__file__).parent.absolute()


def build_dirname(extname=None):
    """Returns the name of the build directory."""
    extname = "" if extname is None else os.sep.join(extname.split(".")[:-1])
    path = pathlib.Path(
        WORKING_DIRECTORY,
        "build",
        f"lib.{sysconfig.get_platform()}-{MAJOR}.{MINOR}",
        extname,
    )
    if path.exists():
        return path
    return pathlib.Path(
        WORKING_DIRECTORY,
        "build",
        f"lib.{sysconfig.get_platform()}-{sys.implementation.cache_tag}",
        extname,
    )


def push_front_syspath():
    """Add the build directory to the front of sys.path."""
    if WORKING_DIRECTORY.joinpath("setup.py").exists():
        # We are in the root directory of the development tree
        sys.path.insert(0, str(build_dirname().resolve()))


def pytest_addoption(parser):
    """Add command line options to pytest."""
    parser.addoption(
        "--sad",
        action="store",
        help="Root directory of the model data to use.",
        default=os.environ.get("PERTH_SAD"),
    )


@pytest.fixture
def sad(request):
    """Return the path to the SAD."""
    value = os.path.expanduser(request.config.getoption("--sad"))
    if value is None:
        raise ValueError("The PERTH_SAD environment variable is not set.")
    os.environ["PERTH_SAD"] = value
    return value


push_front_syspath()
