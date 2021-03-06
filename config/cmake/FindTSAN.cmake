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

# ThreadSanitizer is disabled by default.
option(TSAN "Enable ThreadSanitizer." OFF)

# Check that the build configuration is not imcompatible.
if(TSAN AND (ASAN OR MSAN OR UBSAN))
    # Fail out if we have incompatible configuration.
    message(FATAL_ERROR "ThreadSanitizer is not compatible with AddressSanitizer"
            "MemorySanitizer or UndefinedBehaviorSanitizer.")
endif(TSAN AND (ASAN OR MSAN OR UBSAN))

if(TSAN)
    # Set compiler flags for ThreadSanitizer.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fsanitize=thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fsanitize=thread")
endif(TSAN)

#! add_tsan_static_link : Adds static linking to ThreadSanitizer library
# \param:TARGET TARGET specify the target to be linked against.
function(add_tsan_static_link TARGET)
    if(TSAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        target_link_libraries(${TARGET} "-static-libtsan")
    endif(TSAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
endfunction(add_tsan_static_link TARGET)

#! add_tsan_env : Adds a tsan wrapper script to target directory
# This script is used to define the LD_PRELOAD and TSAN_OPTIONS environment
# variables while running the original executable.
# \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
# \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
function(add_tsan_env EXECUTABLE PRELOAD)
    if(TSAN)
        # Define sanitizer library, environment variable and options values.
        set(SANITIZER_LIBRARY)
        set(SANITIZER_ENVIRON "TSAN_OPTIONS")

        # Check whether the libtsan should be preloaded.
        if(PRELOAD)

            # TODO: Check that executable has dynamic linking to tsan.

            if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
                find_library(SANITIZER_LIBRARY NAMES tsan libtsan.so.0)
            elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                find_library(SANITIZER_LIBRARY NAMES tsan libclang_rt.tsan-x86_64.so)
            endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

            # Assert that the libtsan was found.
            if(NOT EXISTS ${SANITIZER_LIBRARY})
                message(SEND_ERROR "Failed to create asan wrapper for ${EXECUTABLE},\
                        unable to find library 'libtsan'.")
            endif(NOT EXISTS ${SANITIZER_LIBRARY})
        endif(PRELOAD)

        # Add the wrapper script for sanitizer.
        add_sanitizer_script(${EXECUTABLE} ${SANITIZER_ENVIRON} ${SANITIZER_LIBRARY})
    endif(TSAN)
endfunction(add_tsan_env EXECUTABLE PRELOAD)
