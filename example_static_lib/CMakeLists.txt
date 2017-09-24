# Define library name, version and language.
project(static VERSION 0.0.1 LANGUAGES CXX)

# Library target definition. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-buildsystem.7.html
add_library(static)

# Namespaced target alias for library
add_library(static::library ALIAS static)

# Define requirement for the compiler to support constexpr feature. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-compile-features.7.html
# https://cmake.org/cmake/help/v3.5/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
target_compile_features(static
        PUBLIC
            cxx_constexpr)

# Define pre-processor macro values for the target library.
target_compile_definitions(static
        PRIVATE
            HEXADECIMAL=1
            OCTAL=0)

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build.
target_sources(static
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/inc/static/library.hpp
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/static/library.cpp)

# Define headers for this library and add public include directories for any
# project that compiles against this target.
# https://cmake.org/cmake/help/v3.5/manual/cmake-generator-expressions.7.html
target_include_directories(static
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
target_compile_options(static
        PUBLIC
            $<$<CXX_COMPILER_ID:GNU>: -Wextra -Wall -Wpedantic>
            $<$<CXX_COMPILER_ID:Clang>: -Wextra -Wall -pedantic>
            $<$<CXX_COMPILER_ID:MSVC>: /Wall /analyze>)