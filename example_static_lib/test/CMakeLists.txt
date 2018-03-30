################################################################################
# Unit tests for example static library.
################################################################################
set(test_name static_unit_test)

# Define target for the unit tests.
add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add unit test sources to the target.
target_sources(${test_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/static/test_static.cpp)

# Link the unit test target against unit testing framework and the parent project.
target_link_libraries(${test_name}
    PRIVATE
        unit_test_framework
        lib::static)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${test_name} COMMAND ${test_name})
