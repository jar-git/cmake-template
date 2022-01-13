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
project(header VERSION 0.0.1 LANGUAGES CXX)

# Interface library is the target definition for a header-only library.
add_library(${PROJECT_NAME} INTERFACE inc/jar/core/enum.hpp inc/jar/core/type_traits.hpp)

# Alias target for the interface library to be used outside of the project.
add_library(lib::header ALIAS ${PROJECT_NAME})

# Add the template class to target sources.
target_sources(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/core/enum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/core/type_traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/core/contract.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/system/basic_handle.hpp
)

# Include directories for the header-only library.
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:inc>
)

# Define requirement for the compiler to support strongly typed enumerations. See:
# https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html
# https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
target_compile_features(${PROJECT_NAME}
    INTERFACE
        cxx_strong_enums
)

# Add unit tests and benchmarks.
add_subdirectory(test/unit)
