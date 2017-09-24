################################################################################
# Unit tests for example shared library.
################################################################################

# Define target for the unit tests.
add_executable(shared-unit-test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build.
target_sources(shared-unit-test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shared/library.cpp)

# Add unit testing framework as a linked library to the target.
target_link_libraries(shared-unit-test
        PRIVATE
            testframe
            shared::library)

# Add unit test (runs the test).
add_test(shared-unit-test shared-unit-test)