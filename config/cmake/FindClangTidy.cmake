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

# clang-tidy is enabled by default.
option(tidy "Enable clang-tidy." ON)

if(tidy)

    # Check if clang-tidy is installed with find_program.
    find_program(CLANG_TIDY_PROGRAM clang-tidy)

    if(CLANG_TIDY_PROGRAM)

        # Generate a compile definition from the clang-tidy configuration.
        file(SHA1 ${CMAKE_SOURCE_DIR}/.clang-tidy CLANG_TIDY_SHA1)
        set(CLANG_TIDY_DEFINITIONS "CLANG_TIDY_SHA1=${CLANG_TIDY_SHA1}")
        unset(CLANG_TIDY_SHA1)
        configure_file(${CMAKE_SOURCE_DIR}/.clang-tidy ${CMAKE_BINARY_DIR}/.clang-tidy COPYONLY)

        # Define a function for enabling clang-tidy per target. The option above makes this redundant, as it enables it
        # for all targets already. This could be used in combination with custom target commands or by overriding the
        # add_* commands with custom implementation.
        function(enable_clang_tidy TARGET)

            # Force re-build if clang-tidy configuration is updated.
            target_compile_definitions(${TARGET}
                PUBLIC
                    ${CLANG_TIDY_DEFINITIONS}
            )

            # Enable clang-tidy on the target.
            set_target_properties(${TARGET}
                PROPERTIES
                    C_CLANG_TIDY "${CLANG_TIDY_PROGRAM}"
                    CXX_CLANG_TIDY "${CLANG_TIDY_PROGRAM};--extra-arg-before=-std=c++17"
            )

        endfunction(enable_clang_tidy)

    else(CLANG_TIDY_PROGRAM)
        message(WARNING "Program 'clang-tidy' not found, unable to run 'clang-tidy'.")
        function(enable_clang_tidy TARGET)
        endfunction(enable_clang_tidy)
    endif(CLANG_TIDY_PROGRAM)
endif(tidy)
