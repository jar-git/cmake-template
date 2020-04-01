################################################################################
# Unit tests for example shared library.
################################################################################
project(shared_unit_test)

# Define target for the unit tests.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add sources to the target.
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shared/test_shared.cpp)

# Link the unit test target against unit testing framework and the parent project.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        unit_test_framework
        lib::shared)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

# Add static analysis for project.
add_static_analysis(${PROJECT_NAME})
