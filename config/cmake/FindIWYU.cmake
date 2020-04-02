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

# include-what-you-use is enabled by default.
option(IWYU "Enable include-what-you-use." ON)

# https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUPragmas.md
# https://include-what-you-use.org/

if(IWYU)

    # Check if iwyu is installed with find_program.
    find_program(IWYU_PROGRAM iwyu)

    if(IWYU_PROGRAM)

        # Set iwyu mapping file.
        set(IWYU_MAP ${CMAKE_HOME_DIRECTORY}/config/iwyu.cfg)

        # Set iwyu program + options.
        set(IWYU_RUNNABLE ${IWYU_PROGRAM} -Xiwyu --mapping_file=${IWYU_MAP})

        # Define a function for enabling iwyu per target. The option above makes this redundant, as it enables it for
        # all targets already. This could be used in combination with custom target commands or by overriding
        # the add_* commands with custom implementation.
        function(enable_iwyu TARGET)
            set_target_properties(${TARGET}
                PROPERTIES
                    C_INCLUDE_WHAT_YOU_USE "${IWYU_RUNNABLE}"
                    CXX_INCLUDE_WHAT_YOU_USE "${IWYU_RUNNABLE}"
            )
        endfunction(enable_iwyu)

    else(IWYU_PROGRAM)
        message(WARNING "Program 'iwyu' not found, unable to run 'include-what-you-use'.")
        function(enable_iwyu TARGET)
        endfunction(enable_iwyu)
    endif(IWYU_PROGRAM)
endif(IWYU)
