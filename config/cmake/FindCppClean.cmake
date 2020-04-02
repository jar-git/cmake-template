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

# cppclean is enabled by default.
option(cppclean "Enable cppclean." ON)

if(cppclean)

    # Check if cppclean is installed with find_program.
    find_program(CPPCLEAN_PROGRAM cppclean)

    if(CPPCLEAN_PROGRAM)

        # Define a function for enabling cppclean per target.
        function(enable_cppclean TARGET)

            # A horrible hack to get a 'space' between the generator expression parameters, see:
            # https://public.kitware.com/Bug/view.php?id=14353
            set(SPACE \t)

            # Execute ccpclean on the target's source files by taking the source files from the target properties.
            # Include directories are also set from target properties.
            add_custom_command(TARGET ${TARGET}
                PRE_BUILD COMMAND
                    ${CPPCLEAN_PROGRAM}
                    -I$<JOIN:$<TARGET_PROPERTY:${TARGET},INCLUDE_DIRECTORIES>,$<1:${SPACE}>-I>
                     $<JOIN:$<TARGET_PROPERTY:${TARGET},SOURCES>,$<1:${SPACE}>>
            )

        endfunction(enable_cppclean)

    else(CPPCLEAN_PROGRAM)
        message(WARNING "Program 'cppclean' not found, unable to run 'cppclean'.")
        function(enable_cppclean TARGET)
        endfunction(enable_cppclean)
    endif(CPPCLEAN_PROGRAM)
endif(cppclean)
