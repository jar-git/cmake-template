################################################################################
# Example cmake-template project.
################################################################################

# Define minimum CMake version. Language definition:
# https://cmake.org/cmake/help/v3.8/manual/cmake-language.7.html
cmake_minimum_required(VERSION 3.8)
# Define project name, version and languages. Commands:
# https://cmake.org/cmake/help/v3.8/manual/cmake-commands.7.html
project(template VERSION 0.0.1 LANGUAGES CXX)

# Helper module for selecting option based on multiple values. Module help:
# https://cmake.org/cmake/help/v3.8/module/CMakeDependentOption.html
include(CMakeDependentOption)

# Set language, compiler and linker variables. Variables:
# https://cmake.org/cmake/help/v3.8/manual/cmake-variables.7.html
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS_DEBUG ...)
#set(CMAKE_LINKER_FLAGS_DEBUG ...)

# Enable testing for the sub projects.
enable_testing()

# Print Installing, but not the Up-to-date messages when using install command.
set(CMAKE_INSTALL_MESSAGE LAZY)

# Enable linker flags -r -u to create warnings for unused dependencies at link time.
# Warning: Unused direct dependencies: '/lib/x86_64-linux-gnu/libm.so.6'
# set(CMAKE_LINK_WHAT_YOU_USE ON)

# Prefer pthreads and find the thread module. Module help:
# https://cmake.org/cmake/help/v3.8/module/FindThreads.html
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Append custom directory to CMake module search path. Pre-defined modules:
# https://cmake.org/cmake/help/v3.8/manual/cmake-modules.7.html
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/)

# Require custom modules from the previously set directory. See find_package:
# https://cmake.org/cmake/help/v3.8/command/find_package.html
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
find_package(CppLint QUIET)
find_package(CppClean QUIET)
find_package(IWYU QUIET)

# Include Command module for common command overrides. Overrides could be used
# when some build steps like static analysis should be disabled for unit tests
# or thirdparty library facades (wrappers).
# include(Command)

# Add sub project directories to the main template project. Exclude tools from
# ALL target, because it contains scripts for debugging, running and building,
# which are not needed when using this template project from an IDE.
add_subdirectory(tools)
add_subdirectory(thirdparty)
add_subdirectory(example_header_lib)
add_subdirectory(example_static_lib)
add_subdirectory(example_shared_lib)
add_subdirectory(example_app)

