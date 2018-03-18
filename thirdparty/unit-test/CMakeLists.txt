project(unit_test_framework)

# Set googletest version.
set(GTEST_VERSION 1.8.0)

# Check if GTest exists in the system.
find_package(GTest QUIET)

if(NOT GTEST_FOUND)

    # TODO: Fix ExternalProject_Add command, won't work with Ninja generator.

    # If GTest is not found, download and install GTest to build directory, also
    # disable all output ExternalProject_Add creates as it is quite verbose.
    ExternalProject_Add(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-${GTEST_VERSION}
            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest-${GTEST_VERSION}
            INSTALL_DIR "${CMAKE_BINARY_DIR}"
            CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}"
            LOG_DOWNLOAD 1
            LOG_UPDATE 1
            LOG_CONFIGURE 1
            LOG_BUILD 1
            LOG_TEST 1
            LOG_INSTALL 1
    )

    # Define a target library for linkage.
    add_library(${PROJECT_NAME} IMPORTED STATIC GLOBAL)
    # Add dependency to googletest to force the download and install.
    add_dependencies(${PROJECT_NAME} googletest)
    # Link all the required libraries to target.
    target_link_libraries(${PROJECT_NAME}
            INTERFACE
            Threads::Threads
            ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib/libgtest.a
            ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib/libgtest_main.a)

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/include)

    # Set the target library import location and include googletest directories. See:
    # https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html
    set_target_properties(${PROJECT_NAME} PROPERTIES
            IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib/libgtest_main.a
            IMPORTED_LINK_INTERFACE_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
            INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/include)

else()

    # Untested, gtest would have to be installed to test this branch.

    # Define a target library for linkage.
    add_library(${PROJECT_NAME} IMPORTED STATIC GLOBAL)
    # Link all the required libraries to target.
    target_link_libraries(${PROJECT_NAME}
            INTERFACE
            Threads::Threads
            GTest::GTest
            GTest::Main)

    # Set the target library import location and include googletest directories. See:
    # https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html
    set_target_properties(${PROJECT_NAME} PROPERTIES
            IMPORTED_LOCATION ${GTEST_LIBRARIES}
            IMPORTED_LINK_INTERFACE_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
            INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS})

endif()
