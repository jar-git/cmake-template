################################################################################
# Unit tests for example static library.
################################################################################

# Define target for the unit tests.
add_executable(static-unit-test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build.
target_sources(static-unit-test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/static/library.cpp)

# Add unit testing framework as a linked library to the target.
target_link_libraries(static-unit-test
        PRIVATE
            unit_test_framework
            static::library)

# Add unit test (runs the test).
add_test(static-unit-test static-unit-test)