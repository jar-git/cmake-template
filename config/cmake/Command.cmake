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

# Include guard for commands module. Command:
# https://cmake.org/cmake/help/v3.11/command/include_guard.html
include_guard(GLOBAL)

#! add_library : Overrides add_library command.
# \argn: See add_library command.
# \param:TARGET TARGET specify the target
function(add_library TARGET)
    # Command may only be overridden once, otherwise the original command is lost,
    # as the overridden command is always stored with an underscore prefix.
    _add_library(${TARGET} ${ARGN})
endfunction(add_library)

#! add_library : Overrides add_executable command.
# \argn: See add_library command.
# \param:TARGET TARGET specify the target
function(add_executable TARGET)
    _add_executable(${TARGET} ${ARGN})
endfunction(add_executable)

#! add_permissions : Adds permissions to a file.
# See https://cmake.org/cmake/help/latest/command/install.html for list of
# possible permissions.
# \argn: File permissions.
# \param:FILE FILE specify the target file
function(add_permissions FILE)

    # Get filename without the extension from the input file. Command:
    # https://cmake.org/cmake/help/latest/command/get_filename_component.html
    get_filename_component(FILENAME ${FILE} NAME)

    # Create temporary directory for the temporary file.
    set(TEMPORARY_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tmp")
    file(MAKE_DIRECTORY "${TEMPORARY_DIRECTORY}")

    # Set filename for the temporary file.
    set(TEMPORARY_FILE "${TEMPORARY_DIRECTORY}/${FILENAME}")

    # Create the temporary file and remove original.
    file(READ "${FILE}" FILE_CONTENT)
    file(WRITE ${TEMPORARY_FILE} ${FILE_CONTENT})
    file(REMOVE ${FILE} ${FILE_CONTENT})

    # Re-create original with new permissions.
    file(COPY "${TEMPORARY_FILE}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" FILE_PERMISSIONS ${ARGN})

    # Remove the temporary file.
    file(REMOVE "${TEMPORARY_FILE}")

endfunction(add_permissions)
