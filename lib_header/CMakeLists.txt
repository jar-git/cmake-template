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
project(util VERSION 0.0.1 LANGUAGES CXX)

# Interface library is the target definition for a header-only library.
add_library(${PROJECT_NAME} INTERFACE)

# Alias target for the interface library to be used outside of the project.
add_library(lib::util ALIAS ${PROJECT_NAME})

# Add the template class to target sources.
target_sources(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/util/contract.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/shutdown_mode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/socket_family.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/socket_type.hpp
)

# Include directories for the header-only library.
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:inc>
)

# Add unit tests and benchmarks.
add_subdirectory(test/unit)
