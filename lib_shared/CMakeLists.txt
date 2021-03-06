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

# Define library name, version and language.
project(shared VERSION 0.0.1 LANGUAGES CXX)

# Library target definition (generates warning for missing source files).
add_library(${PROJECT_NAME} SHARED)

# Alias target for the shared library to be used outside of the project.
add_library(lib::shared ALIAS ${PROJECT_NAME})

# Define requirement for the compiler to support strongly typed enumerations. See:
# https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html
# https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_strong_enums)

# Add shared library sources to the target.
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/shared/shared_library.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shared/shared_library.cpp)

# Define include directories for this library and add public include directories
# for any project that links against this target.
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
