cmake_minimum_required(VERSION 3.14)
project(pybg LANGUAGES CXX)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# Enable testing
include(CTest)
enable_testing()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Eigen3
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Find Python
find_package(
  Python
  COMPONENTS Interpreter Development.Module
  REQUIRED)

# Find GTest
find_package(GTest)

# Add nanobind
add_subdirectory(external/nanobind)

# Include directories
include_directories(${EIGEN3_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the main library
file(GLOB_RECURSE SOURCES "src/library/*.cpp")
add_library(perth STATIC ${SOURCES})

# If the test option is enabled, add the test subdirectory to the build.
if(GTest_FOUND)
  add_subdirectory(src/tests)
endif()

# Create the Python module
file(GLOB_RECURSE PYBIND_SOURCES "src/pybind/*.cpp")
nanobind_add_module(_core ${PYBIND_SOURCES})
target_link_libraries(_core PUBLIC perth)
