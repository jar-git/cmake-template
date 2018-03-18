# Helper module for defining external projects. Module help:
# https://cmake.org/cmake/help/v3.5/module/ExternalProject.html
include(ExternalProject)

add_subdirectory(unit-test)
add_subdirectory(benchmark)
