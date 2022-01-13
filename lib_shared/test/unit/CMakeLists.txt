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
set(TEST_NAME shared_unit_test)

# Define target for the unit tests.
add_executable(${TEST_NAME}  ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Add sources to the target.
target_sources(${TEST_NAME}
    PRIVATE
)

# Link the unit test target against unit testing framework and the parent project.
target_link_libraries(${TEST_NAME}
    PRIVATE
        unit_test_framework
        lib::shared
)

# Create domain socket paths.
set(SERVER_SOCKET_PATH ${CMAKE_CURRENT_BINARY_DIR}/socket)
set(DGRAM_SOCKET_CHANNEL_A ${CMAKE_CURRENT_BINARY_DIR}/dgram_channel_a)
set(DGRAM_SOCKET_CHANNEL_B ${CMAKE_CURRENT_BINARY_DIR}/dgram_channel_b)
set(NO_SERVER_SOCKET_PATH ${CMAKE_CURRENT_BINARY_DIR}/not_connected_socket)

file(TOUCH ${SERVER_SOCKET_PATH})
file(TOUCH ${NO_SERVER_SOCKET_PATH})
file(TOUCH ${DGRAM_SOCKET_CHANNEL_A})
file(TOUCH ${DGRAM_SOCKET_CHANNEL_B})

# Define pre-processor macro values for the target library. Command:
# https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
target_compile_definitions(${TEST_NAME}
    PRIVATE
        SOCKET_ADDRESS="${SERVER_SOCKET_PATH}"
        NO_ADDRESS="${NO_SERVER_SOCKET_PATH}"
        DGRAM_CHANNEL_A="${DGRAM_SOCKET_CHANNEL_A}"
        DGRAM_CHANNEL_B="${DGRAM_SOCKET_CHANNEL_B}"
)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

# Add static analysis for project.
# add_static_analysis(${PROJECT_NAME})
