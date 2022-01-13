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

#! add_static_analysis : add static analysis to a target
# Facade function for enabling all static analysis tools on a target.
# \param:TARGET TARGET specify the target
function(add_static_analysis TARGET)

    # Check whether the target is imported or not.
    get_target_property(IS_IMPORTED ${TARGET} IMPORTED)
    # Get the target type of the target.
    get_target_property(TARGET_TYPE ${TARGET} TYPE)

    # Do not run static analysis on imported targets or interface libraries.
    if(NOT ${IS_IMPORTED} AND NOT ${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")

        # Disable globally enabled analysers for now.
        enable_clang_tidy(${TARGET})
        #enable_iwyu(${TARGET})
        #enable_cppcheck(${TARGET})
        #enable_cppclean(${PROJECT_NAME})

    else(NOT ${IS_IMPORTED} AND NOT ${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")

        message(SEND_ERROR
            "Imported targets and interface libraries are not supported by \
             add_static_analysis function.")

    endif(NOT ${IS_IMPORTED} AND NOT ${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")

endfunction(add_static_analysis)
