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
project(benchmark_framework)

# Set benchmark version.
set(BENCHMARK_VERSION 1.5.3)
set(BENCHMARK_PREFIX ${CMAKE_BINARY_DIR}/benchmark-${BENCHMARK_VERSION})
set(LIB_BENCHMARK_LOCATION ${BENCHMARK_PREFIX}/lib/libbenchmark${CMAKE_STATIC_LIBRARY_SUFFIX})

# Configure google benchmark.
list(APPEND BENCH_CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release")
list(APPEND BENCH_CMAKE_ARGS "-DCMAKE_CONFIGURATION_TYPES=Release")
list(APPEND BENCH_CMAKE_ARGS "-DBENCHMARK_ENABLE_TESTING=OFF")
list(APPEND BENCH_CMAKE_ARGS "-DBENCHMARK_ENABLE_GTEST_TESTS=OFF")
list(APPEND BENCH_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${BENCHMARK_PREFIX}")
list(APPEND BENCH_CMAKE_ARGS "-DCMAKE_INSTALL_LIBDIR=${BENCHMARK_PREFIX}/lib")
list(APPEND BENCH_CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")

# Download and install Google benchmark, also disable all output from ExternalProject_Add.
ExternalProject_Add(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v${BENCHMARK_VERSION}
    CMAKE_ARGS "${BENCH_CMAKE_ARGS}"
    BUILD_COMMAND ""
    BUILD_BYPRODUCTS ${LIB_BENCHMARK_LOCATION}
    LOG_DOWNLOAD 1
    LOG_UPDATE 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_TEST 1
    LOG_INSTALL 1
    INSTALL_COMMAND cmake --build . --target install --config Release
)

# Define a target library for linkage.
add_library(${PROJECT_NAME} IMPORTED UNKNOWN GLOBAL)

# Add dependency to benchmark to force the download and install.
add_dependencies(${PROJECT_NAME} benchmark)

# Link all the required libraries to target.
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        Threads::Threads
        ${LIB_BENCHMARK_LOCATION}
)

# Create directory for the includes.
file(MAKE_DIRECTORY ${BENCHMARK_PREFIX}/include)

# Set the target library import location and include googletest directories. See:
# https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        IMPORTED_LOCATION ${LIB_BENCHMARK_LOCATION}
        INTERFACE_INCLUDE_DIRECTORIES ${BENCHMARK_PREFIX}/include
)
