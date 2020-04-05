# Copyright 2020 Jani Arola, All rights reserved.
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
set(TEST_NAME util_unit_test)

# Define target for the unit tests.
add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add unit test sources.
target_sources(${TEST_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/util/contract_test.cpp
)

# Add unit test dependencies.
target_link_libraries(${TEST_NAME}
    PRIVATE
        unit_test_framework
        lib::util
)

# Add a test for the parent project to be run by ctest. Command and module:
# https://cmake.org/cmake/help/latest/command/add_test.html
# https://cmake.org/cmake/help/latest/module/CTest.html
add_test(NAME ${TEST_NAME} COMMAND ${PROJECT_NAME})
