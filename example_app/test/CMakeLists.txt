################################################################################
# Unit tests for example application.
################################################################################

# Define target for the unit tests.
add_executable(app-unit-test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build.
target_sources(app-unit-test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/app/application.cpp
            ${APPLICATION_SOURCES})

# Add unit testing framework as a linked library to the target.
target_link_libraries(app-unit-test
        PRIVATE
            testframe
            shared::library
            static::library
            header::only)

target_include_directories(app-unit-test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../inc)

# Add unit test (runs the test).
add_test(app-unit-test app-unit-test)