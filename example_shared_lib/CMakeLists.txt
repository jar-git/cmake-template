# Define variable for project name.
set(project_name shared)
# Define library name, version and language.
project(${project_name} VERSION 0.0.1 LANGUAGES CXX)

# Define default library type(shared or shared) for this sub project.
option(BUILD_SHARED_LIBS "Build library as shared by default." ON)

# Library target definition. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-buildsystem.7.html
add_library(${project_name})

# Namespaced target alias for library
add_library(${project_name}::library ALIAS ${project_name})

# Define requirement for the compiler to support strongly typed enumerations. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-compile-features.7.html
# https://cmake.org/cmake/help/v3.5/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
target_compile_features(${project_name}
        PUBLIC
            cxx_strong_enums)

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build.
target_sources(${project_name}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/inc/shared/library.hpp
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shared/library.cpp)

# Define headers for this library and add public include directories for any
# project that compiles against this target. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-generator-expressions.7.html
target_include_directories(${project_name}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
            $<INSTALL_INTERFACE:inc>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Set compiler warning flags.  See:
# https://cmake.org/cmake/help/v3.5/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_<LANG>_COMPILER_ID
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options
# https://clang.llvm.org/docs/UsersManual.html
# https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx
target_compile_options(${project_name}
        PUBLIC
            $<$<CXX_COMPILER_ID:GNU>: -Wextra -Wall -Wpedantic>
            $<$<CXX_COMPILER_ID:Clang>: -Wextra -Wall -pedantic>
            $<$<CXX_COMPILER_ID:MSVC>: /Wall /analyze>)

# Add unit tests.
add_subdirectory(test)