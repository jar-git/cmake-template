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

# LeakSanitizer is enabled by default.
option(LSAN "Enable LeakSanitizer." ON)

# Set flag if LeakSanitizer is running without AddressSanitizer.
if(LSAN AND NOT ASAN)
    set(ONLY_LSAN ON)
endif(LSAN AND NOT ASAN)

# Check that the build configuration is not imcompatible.
if(ONLY_LSAN AND (TSAN OR MSAN OR UBSAN))
    # Command: https://cmake.org/cmake/help/latest/command/message.html
    message(FATAL_ERROR "LeakSanitizer is not compatible with ThreadSanitizer"
            "MemorySanitizer or UndefinedBehaviorSanitizer.")
endif(ONLY_LSAN AND (TSAN OR MSAN OR UBSAN))

# Check if LeakSanitizer should be enabled without AddressSanitizer.
if(ONLY_LSAN)
    # Set compiler flags for LeakSanitizer.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O1 -fsanitize=leak -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=leak -fno-omit-frame-pointer")
endif(ONLY_LSAN)

#! add_lsan_static_link : Adds static linking to LeakSanitizer library
# \param:TARGET TARGET specify the target to be linked against.
function(add_lsan_static_link TARGET)
    if(ONLY_LSAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        target_link_libraries(${TARGET} "-static-liblsan")
    endif(ONLY_LSAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
endfunction(add_lsan_static_link TARGET)

#! add_lsan_env : Adds a lsan wrapper script to target directory
# This script is used to define the LD_PRELOAD and ASAN_OPTIONS environment
# variables while running the original executable.
# \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
# \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
function(add_lsan_env EXECUTABLE PRELOAD)
    if(ONLY_LSAN)
        # Define sanitizer library, environment variable and options values.
        set(SANITIZER_LIBRARY)
        set(SANITIZER_ENVIRON "ASAN_OPTIONS")

        # Check whether the liblsan should be preloaded.
        if(PRELOAD)

            # TODO: Check that executable has dynamic linking to asan.

            if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
                # Find asan library from the system. Command:
                # https://cmake.org/cmake/help/latest/command/find_library.html
                find_library(SANITIZER_LIBRARY NAMES lsan liblsan.so.0)
            elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                # TODO: How to find correct asan library when using clang?
                find_library(SANITIZER_LIBRARY NAMES
                        asan libclang_rt.asan-x86_64.so)
            endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

            # Assert that the libasan was found.
            if(NOT EXISTS ${SANITIZER_LIBRARY})
                message(SEND_ERROR "Failed to create asan wrapper for ${EXECUTABLE},\
                    unable to find library 'liblsan'.")
            endif(NOT EXISTS ${SANITIZER_LIBRARY})
        endif(PRELOAD)

        # Add the wrapper script for sanitizer.
        add_sanitizer_script(${EXECUTABLE} ${SANITIZER_ENVIRON} ${SANITIZER_LIBRARY})

    endif(ONLY_LSAN)
endfunction(add_lsan_env EXECUTABLE PRELOAD)
