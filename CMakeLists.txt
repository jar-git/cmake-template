# Copyright 2017 Jani Arola, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Define minimum CMake version. Language definition:
# https://cmake.org/cmake/help/latest/manual/cmake-language.7.html
cmake_minimum_required(VERSION 3.10)

# Define project name, version and languages. Commands:
# https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html
project(template VERSION 0.0.1 LANGUAGES CXX)

# Helper module for selecting option based on multiple values. Module help:
# https://cmake.org/cmake/help/latest/module/CMakeDependentOption.html
include(CMakeDependentOption)

# Set language standard and export compile commands. Variables:
# https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing for the sub projects.
enable_testing()

# Print Installing, but not the Up-to-date messages when using install command.
set(CMAKE_INSTALL_MESSAGE LAZY)

# Enable linker flags -r -u to create warnings for unused dependencies at link time.
# Warning: Unused direct dependencies: '/usr/lib/libm.so.6' (required by std)
set(CMAKE_LINK_WHAT_YOU_USE ON)

# Prefer pthreads and find the thread module. Module help:
# https://cmake.org/cmake/help/latest/module/FindThreads.html
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Append custom directory to CMake module search path. Pre-defined modules:
# https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config/cmake/)

# Require custom modules from the previously set directory. See find_package:
# https://cmake.org/cmake/help/latest/command/find_package.html
find_package(CCache QUIET)
find_package(ClangTidy QUIET)
find_package(Gold QUIET)
find_package(Doc QUIET)
find_package(ASAN QUIET)
find_package(LSAN QUIET)
find_package(TSAN QUIET)
find_package(MSAN QUIET)
find_package(UBSAN QUIET)
find_package(GCOV QUIET)
find_package(CppCheck QUIET)
find_package(CppClean QUIET)
find_package(IWYU QUIET)

# Include modules for static analysers and sanitizers.
include(StaticAnalysis)
include(Sanitizers)

# Include Command module for common command overrides. Overrides could be used when some build steps like static
# analysis should be disabled for unit tests or 3rd party library facades (wrappers).
include(Command)

# Add sub project directories to the main template project.
add_subdirectory(dependencies)
add_subdirectory(lib_header)
add_subdirectory(lib_static)
add_subdirectory(lib_shared)
add_subdirectory(exe_service)
