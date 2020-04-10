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
project(application VERSION 0.0.1 LANGUAGES CXX)

# Executable target definition. CMake-build-system documentation:
# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Alias target for the executable to be used outside of the project. Command:
# https://cmake.org/cmake/help/latest/command/add_executable.html
add_executable(app::exe ALIAS ${PROJECT_NAME})

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build). Defining the target sources with public
# attribute allows to populate the target property INTERFACE_SOURCES, and
# this allows the test project to use the target property directly. Command:
# https://cmake.org/cmake/help/latest/command/target_sources.html
target_sources(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/app/application.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/application.cpp)

# Define libraries the target links against. Target property LINK_LIBRARIES
# will be populated with a list of the linked libraries. Command:
# https://cmake.org/cmake/help/latest/command/target_link_libraries.html
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        lib::shared
        lib::static
#        lib::header
        Threads::Threads)

# Define the include directories for header files for the target. Command:
# https://cmake.org/cmake/help/latest/command/target_include_directories.html
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Define the compile options of the target. This will populate the target
# property COMPILE_OPTIONS, that could be used with the unit test. Command:
# https://cmake.org/cmake/help/latest/command/target_compile_options.html
# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
target_compile_options(${PROJECT_NAME}
        PUBLIC
            $<$<CXX_COMPILER_ID:GNU>: -Wextra -Wall -Wpedantic>
            $<$<CXX_COMPILER_ID:Clang>: -Wextra -Wall -pedantic>
            $<$<CXX_COMPILER_ID:MSVC>: /Wall /analyze>)

#target_link_options(
#    PUBLIC
#)

#target_link_directories(
#    PUBLIC
#)

# Compiler options for the compilers defined above:
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options
# https://clang.llvm.org/docs/UsersManual.html
# https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx

# Add static analysis for project.
add_static_analysis(${PROJECT_NAME})

# Set parent project name, test projects will use it to add include directories,
# sources and link libraries with generator expressions.
set(PARENT_PROJECT ${PROJECT_NAME})

# Create sanitizer environment script for the application.
add_sanitizer_env(${PROJECT_NAME} ON)

# Add unit tests.
add_subdirectory(test)
add_subdirectory(bench)
