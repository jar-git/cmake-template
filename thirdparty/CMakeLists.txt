# Define minimum CMake version.
cmake_minimum_required(VERSION 3.5)
# Define project name.
project(thirdparty)

# Add locally kept 3rd party library.
add_subdirectory(header_lib)

# Include ExternalProject CMake module. See:
# https://cmake.org/cmake/help/v3.5/module/ExternalProject.html
include(ExternalProject)

# Set googletest version.
set(GTEST_VERSION 1.8.0)

# Check if GTest exists in the system.
find_package(GTest QUIET)

if(NOT GTEST_FOUND)

    # If GTest is not found, download and install GTest.
    ExternalProject_Add(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-${GTEST_VERSION}
            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest-${GTEST_VERSION}
            INSTALL_DIR "${CMAKE_BINARY_DIR}"
            CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}"
    )

    # Define a target library for linkage.
    add_library(testframe IMPORTED STATIC GLOBAL)
    # Add dependency to googletest to force the download and install.
    add_dependencies(testframe googletest)
    # Link all the required libraries to target.
    target_link_libraries(testframe
            INTERFACE
                Threads::Threads
                ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib/libgtest.a
                ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib/libgtest_main.a)

    # Set the target library import location and include googletest directories. See:
    # https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html
    set_target_properties(testframe PROPERTIES
            IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib/libgtest_main.a
            IMPORTED_LINK_INTERFACE_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
            INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/include)

else(NOT GTEST_FOUND)

    # Most likely not working.

    # Define a target library for linkage.
    add_library(testframe IMPORTED STATIC GLOBAL)
    # Link all the required libraries to target.
    target_link_libraries(testframe
            INTERFACE
                Threads::Threads
                GTest::GTest
                GTest::Main)

    # Set the target library import location and include googletest directories. See:
    # https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html
    set_target_properties(testframe PROPERTIES
            IMPORTED_LOCATION ${GTEST_LIBRARIES}
            IMPORTED_LINK_INTERFACE_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
            INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS})

endif(NOT GTEST_FOUND)

# Set benchmark version.
set(BENCHMARK_VERSION 1.2.0)

# Download and install Google benchmark.
ExternalProject_Add(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v${BENCHMARK_VERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/benchmark-${BENCHMARK_VERSION}
        INSTALL_DIR "${CMAKE_BINARY_DIR}"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
)
