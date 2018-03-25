project(benchmark_framework)

# Set benchmark version.
set(BENCHMARK_VERSION 1.2.0)

# Disable benchmark testing.
set(BENCHMARK_ENABLE_TESTING OFF)

# TODO: Fix ExternalProject_Add command, won't work with Ninja generator.

# Download and install Google benchmark, also disable all output ExternalProject_Add
# creates as it is quite verbose.
ExternalProject_Add(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v${BENCHMARK_VERSION}
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}
    INSTALL_DIR "${CMAKE_BINARY_DIR}"
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}"
    LOG_DOWNLOAD 1
    LOG_UPDATE 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_TEST 1
    LOG_INSTALL 1
)

# Define a target library for linkage.
add_library(${PROJECT_NAME} IMPORTED STATIC GLOBAL)

# Add dependency to benchmark to force the download and install.
add_dependencies(${PROJECT_NAME} benchmark)

# Link all the required libraries to target.
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        Threads::Threads
        ${CMAKE_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}/lib/libbenchmark.a)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}/include)

# Set the target library import location and include googletest directories. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html
set_target_properties(${PROJECT_NAME} PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}/lib/libbenchmark.a
    IMPORTED_LINK_INTERFACE_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}/include)
