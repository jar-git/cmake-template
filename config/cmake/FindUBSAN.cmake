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

# UndefinedBehaviorSanitizer is disabled by default.
option(UBSAN "Enable UndefinedBehaviorSanitizer." OFF)

# Check that the build configuration is not imcompatible.
if(UBSAN AND (ASAN OR TSAN OR MSAN))
    # Fail out if we have incompatible configuration.
    message(FATAL_ERROR "UndefinedBehaviorSanitizer is not compatible with AddressSanitizer"
            "ThreadSanitizer or MemorySanitizer.")
endif(UBSAN AND (ASAN OR TSAN OR MSAN))

if(UBSAN)
    # Set compiler flags for ThreadSanitizer.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=undefined -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=undefined -fno-omit-frame-pointer")
endif(UBSAN)

#! add_tsan_static_link : Adds static linking to UndefinedBehaviorSanitizer library
# \param:TARGET TARGET specify the target to be linked against.
function(add_ubsan_static_link TARGET)
    if(UBSAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        target_link_libraries(${TARGET} "-static-libubsan")
    endif(UBSAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
endfunction(add_ubsan_static_link TARGET)

#! add_ubsan_env : Adds a ubsan wrapper script to target directory
# This script is used to define the LD_PRELOAD and UBSAN_OPTIONS environment
# variables while running the original executable.
# \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
# \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
function(add_ubsan_env EXECUTABLE PRELOAD)
    if(UBSAN)
        # Define sanitizer library, environment variable and options values.
        set(SANITIZER_LIBRARY)
        set(SANITIZER_ENVIRON "UBSAN_OPTIONS")

        # Check whether the libtsan should be preloaded.
        if(PRELOAD)

            # TODO: Check that executable has dynamic linking to ubsan.

            if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
                find_library(SANITIZER_LIBRARY NAMES ubsan libubsan.so.0)
            elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                find_library(SANITIZER_LIBRARY NAMES ubsan libclang_rt.ubsan-x86_64.so)
            endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

            # Assert that the libtsan was found.
            if(NOT EXISTS ${SANITIZER_LIBRARY})
                message(SEND_ERROR "Failed to create ubsan wrapper for ${EXECUTABLE},\
                        unable to find library 'ubsan'.")
            endif(NOT EXISTS ${SANITIZER_LIBRARY})
        endif(PRELOAD)

        # Add the wrapper script for sanitizer.
        add_sanitizer_script(${EXECUTABLE} ${SANITIZER_ENVIRON} ${SANITIZER_LIBRARY})
    endif(UBSAN)
endfunction(add_ubsan_env EXECUTABLE PRELOAD)
