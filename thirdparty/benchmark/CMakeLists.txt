# project(benchmark_framework)

# Set benchmark version.
set(BENCHMARK_VERSION 1.2.0)

# TODO: Fix ExternalProject_Add command, won't work with Ninja generator.

# Download and install Google benchmark, also disable all output ExternalProject_Add
# creates as it is quite verbose.
ExternalProject_Add(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v${BENCHMARK_VERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}
        INSTALL_DIR "${CMAKE_BINARY_DIR}"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
        LOG_DOWNLOAD 1
        LOG_UPDATE 1
        LOG_CONFIGURE 1
        LOG_BUILD 1
        LOG_TEST 1
        LOG_INSTALL 1
)
