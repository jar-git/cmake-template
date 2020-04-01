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
include_guard(GLOBAL)

#! add_sanitizer_env : Adds a sanitizer wrapper script to target directory
# Facade function for creating correct sanitizer wrapper for the executable.
# \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
# \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
function(add_sanitizer_env EXECUTABLE PRELOAD)
    add_asan_env(${EXECUTABLE} ${PRELOAD})
    add_lsan_env(${EXECUTABLE} ${PRELOAD})
    add_tsan_env(${EXECUTABLE} ${PRELOAD})
    add_ubsan_env(${EXECUTABLE} ${PRELOAD})
    add_msan_env(${EXECUTABLE} ${PRELOAD})
endfunction(add_sanitizer_env EXECUTABLE PRELOAD)

#! add_sanitizer_static_link : Adds static linking to sanitizer library
# Facade function for adding static linking to the correct sanitizer library.
# \param:TARGET TARGET specify the target to be linked against.
function(add_sanitizer_static_link TARGET)
    add_asan_static_link(${TARGET})
    add_lsan_static_link(${TARGET})
    add_tsan_static_link(${TARGET})
    add_ubsan_static_link(${TARGET})
endfunction(add_sanitizer_static_link TARGET)

#! add_sanitizer_script : Adds environment script to target directory
# This script is used to export the LD_PRELOAD and <SAN>_OPTIONS environment variables.
# \param:EXECUTABLE EXECUTABLE specify the target for the wrapper.
# \param:SANITIZER_ENVIRON SANITIZER_ENVIRON Specifies the sanitizer environment variable.
# \param:SANITIZER_LIBRARY SANITIZER_LIBRARY Specifies the library file to be preloaded.
function(add_sanitizer_script EXECUTABLE SANITIZER_ENVIRON SANITIZER_LIBRARY)

    # Configure generic options for all sanitizers with environment. Environment:
    # https://cmake.org/cmake/help/latest/variable/ENV.html
    set(SANITIZER_OPTIONS "$ENV{${SANITIZER_ENVIRON}}abort_on_error=1 print_suppressions=0")

    # Configure the sanitizer environment.
    configure_file(${CMAKE_HOME_DIRECTORY}/scripts/environment.sh ${CMAKE_CURRENT_BINARY_DIR}/environment.sh @ONLY)

endfunction(add_sanitizer_script EXECUTABLE SANITIZER_ENVIRON SANITIZER_LIBRARY)
