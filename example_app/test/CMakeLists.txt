################################################################################
# Unit tests for example application.
################################################################################
set(test_name app_unit_test)

# Define target for the unit tests.
add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add unit test sources and parent project sources to the target. Expressions:
# https://cmake.org/cmake/help/v3.8/manual/cmake-generator-expressions.7.html
target_sources(${test_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/application.cpp
        $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_SOURCES>)

# Link the unit test target against unit testing framework and the dependencies
# of the parent project. CMake-properties:
# https://cmake.org/cmake/help/v3.8/manual/cmake-properties.7.html
target_link_libraries(${test_name}
    PRIVATE
        unit_test_framework
        $<TARGET_PROPERTY:${PROJECT_NAME},LINK_LIBRARIES>)

# Use the parent projects include directories in the unit test.
target_include_directories(${test_name}
    PRIVATE
        $<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>)

# Add a test for the parent project to be run by ctest. Command and module:
# https://cmake.org/cmake/help/v3.8/command/add_test.html
# https://cmake.org/cmake/help/v3.8/module/CTest.html
add_test(NAME ${test_name} COMMAND ${test_name})
