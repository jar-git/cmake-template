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

# Enable ccache by default. Command:
# https://cmake.org/cmake/help/latest/command/option.html
option(ccache "Enable ccache." ON)

# Check if ccache is installed with find_program. Command:
# https://cmake.org/cmake/help/latest/command/find_program.html
find_program(CCACHE_DIR ccache)

if(ccache)
    if(EXISTS ${CCACHE_DIR})
        set(CMAKE_C_COMPILER_LAUNCHER "ccache")
        set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")
    else()
        message(WARNING "Failed to enable ccache, program 'ccache' missing.")
    endif()
endif()
