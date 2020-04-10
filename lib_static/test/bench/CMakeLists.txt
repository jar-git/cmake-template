# Copyright 2020 Jani Arola, All rights reserved.
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
set(BENCHMARK_NAME sys_benchmark)

# Define executable benchmark target.
add_executable(${BENCHMARK_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/bench.cpp)

# Add benchmark sources.
target_sources(${BENCHMARK_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/basic_socket_benchmark.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/stream_socket_benchmark.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/stream_socket_benchmark.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/datagram_socket_benchmark.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/net/datagram_socket_benchmark.cpp
)

# Add libraries.
target_link_libraries(${BENCHMARK_NAME}
    PRIVATE
        benchmark_framework
        lib::static
)

# Set include directories.
target_include_directories(${BENCHMARK_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Create domain socket paths.
set(SERVER_SOCKET_PATH ${CMAKE_CURRENT_BINARY_DIR}/socket)
set(DGRAM_SOCKET_CHANNEL_A ${CMAKE_CURRENT_BINARY_DIR}/dgram_channel_a)
set(DGRAM_SOCKET_CHANNEL_B ${CMAKE_CURRENT_BINARY_DIR}/dgram_channel_b)
set(NO_SERVER_SOCKET_PATH ${CMAKE_CURRENT_BINARY_DIR}/not_connected_socket)

file(TOUCH ${SERVER_SOCKET_PATH})
file(TOUCH ${NO_SERVER_SOCKET_PATH})
file(TOUCH ${DGRAM_SOCKET_CHANNEL_A})
file(TOUCH ${DGRAM_SOCKET_CHANNEL_B})

# Define pre-processor macro values for the target library. Command:
# https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
target_compile_definitions(${BENCHMARK_NAME}
    PRIVATE
        SOCKET_ADDRESS="${SERVER_SOCKET_PATH}"
        NO_ADDRESS="${NO_SERVER_SOCKET_PATH}"
        DGRAM_CHANNEL_A="${DGRAM_SOCKET_CHANNEL_A}"
        DGRAM_CHANNEL_B="${DGRAM_SOCKET_CHANNEL_B}"
)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${BENCHMARK_NAME} COMMAND ${BENCHMARK_NAME})

# Set the label for the benchmark "tests", so that they will be excluded by
# default when running the unit tests with ctest. Command:
# https://cmake.org/cmake/help/latest/command/set_tests_properties.html
set_tests_properties(${BENCHMARK_NAME} PROPERTIES LABELS "BenchmarkTest")
