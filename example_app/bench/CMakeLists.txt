################################################################################
# Benchmarks for example application.
################################################################################
set(test_name app_benchmark)

# Define target for the benchmark.
add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/src/bench.cpp)

# Add benchmark sources and parent project sources to the target.
target_sources(${test_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/application.cpp
        $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_SOURCES>)

# Link the benchmark target against benchmark library and the dependencies
# of the parent project.
target_link_libraries(${test_name}
    PRIVATE
        benchmark_framework
        $<TARGET_PROPERTY:${PROJECT_NAME},LINK_LIBRARIES>)

# Use the parent projects include directories in the unit test.
target_include_directories(${test_name}
    PRIVATE
        $<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${test_name} COMMAND ${test_name})

# Set the label for the benchmark "tests", so that they will be excluded by
# default when running the unit tests with ctest. Command:
# https://cmake.org/cmake/help/v3.6/command/set_tests_properties.html
set_tests_properties(${test_name} PROPERTIES LABELS "BenchmarkTest")
