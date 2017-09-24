# Define minimum CMake version.
cmake_minimum_required(VERSION 3.5)

# Add locally kept 3rd party library.
add_subdirectory(header_lib)

# Include ExternalProject CMake module. See:
# https://cmake.org/cmake/help/v3.5/module/ExternalProject.html
include(ExternalProject)

# Set googletest version.
set(GTEST_VERSION 1.8.0)

# Download and install GoogleTest.
ExternalProject_Add(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-${GTEST_VERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest-${GTEST_VERSION}
        # Disable all other commands.
        INSTALL_COMMAND ""
)

# Get em directories!?
#ExternalProject_Get_Property(gtest source_dir binary_dir)


# Set benchmark version.
set(BENCHMARK_VERSION 1.2.0)

# Download and install Google benchmark.
ExternalProject_Add(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v${BENCHMARK_VERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}
        # Disable all other commands.
        INSTALL_COMMAND ""
)
