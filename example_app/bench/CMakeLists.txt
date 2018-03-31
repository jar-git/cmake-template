################################################################################
# Benchmarks for example application.
################################################################################
project(app_benchmark)

# Define target for the benchmark.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/bench.cpp)

# Add benchmark sources and parent project sources to the target.
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/app/bench_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/bench_app.cpp
        $<TARGET_PROPERTY:${PARENT_PROJECT},INTERFACE_SOURCES>)

# Link the benchmark target against benchmark library and the dependencies
# of the parent project.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        benchmark_framework
        $<TARGET_PROPERTY:${PARENT_PROJECT},LINK_LIBRARIES>)

# Use the parent projects include directories in the unit test.
target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<TARGET_PROPERTY:${PARENT_PROJECT},INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

# Set the label for the benchmark "tests", so that they will be excluded by
# default when running the unit tests with ctest. Command:
# https://cmake.org/cmake/help/v3.10/command/set_tests_properties.html
set_tests_properties(${PROJECT_NAME} PROPERTIES LABELS "BenchmarkTest")

# Add static analysis for project.
add_static_analysis(${PROJECT_NAME})
