# CMake module for defining 3rd party dependencies as external projects. Module:
# https://cmake.org/cmake/help/v3.5/module/ExternalProject.html
include(ExternalProject)

add_subdirectory(unit-test)
add_subdirectory(benchmark)
