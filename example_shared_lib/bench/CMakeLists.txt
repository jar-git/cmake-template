################################################################################
# Benchmarks for shared library.
################################################################################
set(test_name shared_benchmark)

# Define target for the benchmark.
add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/src/bench.cpp)

# Add benchmark sources.
target_sources(${test_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shared/library.cpp)

# Link the benchmark target against benchmark library and the parent project.
target_link_libraries(${test_name}
    PRIVATE
        benchmark_framework
        lib::shared)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${test_name} COMMAND ${test_name})

# Set the label for the benchmark "tests", so that they will be excluded by
# default when running the unit tests with ctest. Command:
# https://cmake.org/cmake/help/v3.8/command/set_tests_properties.html
set_tests_properties(${test_name} PROPERTIES LABELS "BenchmarkTest")
