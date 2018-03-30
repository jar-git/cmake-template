################################################################################
# Example static library sub-project.
################################################################################

# Define library name, version and language.
project(static VERSION 0.0.1 LANGUAGES CXX)

# Library target definition (generates warning for missing source files).
add_library(${PROJECT_NAME})

# Alias target for the static library to be used outside of the project.
add_library(lib::static ALIAS ${PROJECT_NAME})

# Define pre-processor macro values for the target library. Command:
# https://cmake.org/cmake/help/v3.8/command/target_compile_definitions.html
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        HEXADECIMAL=1
        OCTAL=0)

# Add sources to the target.
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/static/static_library.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/static/static_library.cpp)

# Define headers for this library and add public include directories for any
# project that links against this target.
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:inc>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Define the compile options of the target. This will populate the target
# property COMPILE_OPTIONS, that could be used with the unit test.
target_compile_options(${PROJECT_NAME}
    PUBLIC
        $<$<CXX_COMPILER_ID:GNU>: -Wextra -Wall -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>: -Wextra -Wall -pedantic>
        $<$<CXX_COMPILER_ID:MSVC>: /Wall /analyze>)

# Add static analysis for project.
add_static_analysis(${PROJECT_NAME})

# Add unit tests.
add_subdirectory(test)
add_subdirectory(bench)
