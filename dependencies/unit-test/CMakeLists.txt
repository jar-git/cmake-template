# Copyright 2017 Jani Arola, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
project(unit_test_framework)

# Set googletest version.
set(GTEST_VERSION 1.10.0)

# Download and install GTest, also disable all output from ExternalProject_Add. It is quite verbose.
ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-${GTEST_VERSION}
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest-${GTEST_VERSION}
    INSTALL_DIR "${CMAKE_BINARY_DIR}"
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}"
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib64/libgtest.a
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
        ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib64/libgtest.a
)

# Create directory for the includes.
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/include)

# Set the target library import location and include googletest directories. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/lib64/libgtest.a
        IMPORTED_LINK_INTERFACE_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/gtest-${GTEST_VERSION}/include
)
