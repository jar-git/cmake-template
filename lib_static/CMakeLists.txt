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
project(static VERSION 0.0.1 LANGUAGES CXX)

# Define library as static. The explicit static definition is not required, if left out the default seems to be static
# library. If implicit static is used, then it can be controlled via the BUILD_SHARED_LIBS variable.
add_library(${PROJECT_NAME} STATIC)

# Alias target for the interface library to be used outside of the project.
add_library(lib::static ALIAS ${PROJECT_NAME})

# Add the template class to target sources.
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/fs/file_descriptor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/datagram_socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/basic_stream_socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/stream_socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/stream_server_socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/domain_address.cpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/fs/file_descriptor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/socket_address.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/domain_address.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/socket.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/datagram_socket.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/basic_stream_socket.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/stream_socket.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/net/stream_server_socket.hpp
)

# Include directories for the header-only library.
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:inc>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add dependency to lib_header (aka lib::util).
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        lib::util
)

# Set all compiler warnings on.
target_compile_options(${PROJECT_NAME}
    PUBLIC
        $<$<CXX_COMPILER_ID:GNU>: -Wextra -Wall -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>: -Wextra -Wall -pedantic>
        $<$<CXX_COMPILER_ID:MSVC>: /Wall /analyze>
)

# Turn on static analysis
add_static_analysis(${PROJECT_NAME})

# Add unit tests and benchmarks.
add_subdirectory(test/bench)
add_subdirectory(test/unit)
