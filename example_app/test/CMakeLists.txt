################################################################################
# Unit tests for example application.
################################################################################
project(app_unit_test)

# Define target for the unit tests.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add unit test sources and parent project sources to the target. Expressions:
# https://cmake.org/cmake/help/v3.10/manual/cmake-generator-expressions.7.html
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/test_app.cpp
        $<TARGET_PROPERTY:${PARENT_PROJECT},INTERFACE_SOURCES>)

# Link the unit test target against unit testing framework and the dependencies
# of the parent project. CMake-properties:
# https://cmake.org/cmake/help/v3.10/manual/cmake-properties.7.html
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        unit_test_framework
        $<TARGET_PROPERTY:${PARENT_PROJECT},LINK_LIBRARIES>)

# Use the parent projects include directories in the unit test.
target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<TARGET_PROPERTY:${PARENT_PROJECT},INCLUDE_DIRECTORIES>)

# Add a test for the parent project to be run by ctest. Command and module:
# https://cmake.org/cmake/help/v3.10/command/add_test.html
# https://cmake.org/cmake/help/v3.10/module/CTest.html
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

# Add static analysis for project.
add_static_analysis(${PROJECT_NAME})
