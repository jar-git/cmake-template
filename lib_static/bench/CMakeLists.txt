################################################################################
# Benchmarks for static library.
################################################################################
project(static_benchmark)

# Define target for the benchmark.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/bench.cpp)

# Add benchmark sources.
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/static/bench_static.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/static/bench_static.cpp)

# Link the benchmark target against benchmark library and the parent project.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        benchmark_framework
        lib::static)

# Set include directories.
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

# Set the label for the benchmark "tests", so that they will be excluded by
# default when running the unit tests with ctest. Command:
# https://cmake.org/cmake/help/latest/command/set_tests_properties.html
set_tests_properties(${PROJECT_NAME} PROPERTIES LABELS "BenchmarkTest")

# Add static analysis for project.
add_static_analysis(${PROJECT_NAME})
