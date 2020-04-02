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

# cppcheck is enabled by default.
option(cppcheck "Enable cppcheck." ON)

if(cppcheck)

    # Check if cppcheck is installed with find_program.
    find_program(CPPCHECK_PROGRAM cppcheck)

    if(CPPCHECK_PROGRAM)

        # Set and create cppcheck cache directory
        set(CPPCHECK_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/cppcheck_cache)
        file(MAKE_DIRECTORY ${CPPCHECK_BUILD_DIR})

        # Set cppcheck supressions file.
        set(CPPCHECK_SUPRESSIONS ${CMAKE_HOME_DIRECTORY}/config/cppcheck.cfg)

        # Set message template
        set(CPPCHECK_TEMPLATE "[{file}:{line}] ({severity}) {message} ({id})")

        # Set cppcheck program + options.
        set(CPPCHECK_RUNNABLE
                ${CPPCHECK_PROGRAM}
                --enable=warning,style
                --quiet
                --inline-suppr
                --platform=native
                --cppcheck-build-dir=${CPPCHECK_BUILD_DIR}
                --template=${CPPCHECK_TEMPLATE}
                --suppressions-list=${CPPCHECK_SUPRESSIONS}
                --std=c++17)

        # Define a function for enabling cppcheck per target. The option above makes this redundant, as it enables it
        # for all targets already. This could be used in combination with custom target commands or by  overriding
        # the add_* commands with custom implementation.
        function(enable_cppcheck TARGET)
            set_target_properties(${TARGET}
                PROPERTIES
                    C_CPPCHECK "${CPPCHECK_RUNNABLE}"
                    CXX_CPPCHECK "${CPPCHECK_RUNNABLE}"
            )
        endfunction(enable_cppcheck)

    else(CPPCHECK_PROGRAM)
        message(WARNING "Program 'cppcheck' not found, unable to run 'cppcheck'.")
    endif(CPPCHECK_PROGRAM)
endif(cppcheck)
