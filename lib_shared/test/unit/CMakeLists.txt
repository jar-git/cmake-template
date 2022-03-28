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
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/jar/concurrency/mock_sender.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/type_traits_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/latch_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/queue_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/future_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/thread_pool_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/rr_scheduler_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/details/sender_adapter_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/details/value_receiver_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/details/callback_receiver_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/then_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jar/concurrency/start_test.cpp
)

target_include_directories(${TEST_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link the unit test target against unit testing framework and the parent project.
target_link_libraries(${TEST_NAME}
    PRIVATE
        unit_test_framework
        lib::shared
)

# Add a test for the parent project to be run by ctest.
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

# Add static analysis for project.
# add_static_analysis(${PROJECT_NAME})
