# Define library name, version and language.
project(application VERSION 0.0.1 LANGUAGES CXX)

# Library target definition. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-buildsystem.7.html
add_executable(application ${CMAKE_CURRENT_SOURCE_DIR}/src/app/main.cpp)

# Namespaced target alias for executable target
add_executable(application::executable ALIAS application)

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build.
target_sources(application
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/inc/app/application.hpp
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/app/application.cpp)

# Define libraries the target links against.
target_link_libraries(application
        shared::library
        static::library
        header::only)

# Define headers for this library and add public include directories for any
# project that compiles against this target. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-generator-expressions.7.html
target_include_directories(application
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Set compiler warning flags.  See:
# https://cmake.org/cmake/help/v3.5/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_<LANG>_COMPILER_ID
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options
# https://clang.llvm.org/docs/UsersManual.html
# https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx
target_compile_options(application
        PUBLIC
            $<$<CXX_COMPILER_ID:GNU>: -Wextra -Wall -Wpedantic>
            $<$<CXX_COMPILER_ID:Clang>: -Wextra -Wall -pedantic>
            $<$<CXX_COMPILER_ID:MSVC>: /Wall /analyze>)
