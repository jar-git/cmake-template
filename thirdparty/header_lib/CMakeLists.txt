# Define minimum CMake version.
cmake_minimum_required(VERSION 3.5)

# Define library name, version and language.
project(header-only VERSION 0.0.1 LANGUAGES CXX)

# Library target definition as a header-only library. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-buildsystem.7.html
add_library(header-only INTERFACE)

# Namespaced target alias for library
add_library(header::only ALIAS header-only)

# Include directories for the header-only library. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-generator-expressions.7.html
target_include_directories(header-only
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
            $<INSTALL_INTERFACE:inc>)
