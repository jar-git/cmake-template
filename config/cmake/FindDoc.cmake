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

# Option for building doxygen documentation.
option(doc "Build documentation" OFF)

if (doc)
    # Find doxygen, Module help:
    # https://cmake.org/cmake/help/latest/module/FindDoxygen.html
    find_package(Doxygen)

    if (DOXYGEN_FOUND)

        # Set input and output file for configuration.
        set(DOXYGEN_IN ${CMAKE_HOME_DIRECTORY}/config/doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
        # Set doxygen documentation directory.
        set(DOC_DIR documentation)
        # Configure the doxygen configuration file and copy it from source directory to build
        # directory (replace variable references).
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        # Add custom target for building the documentation. Command help:
        # https://cmake.org/cmake/help/latest/command/add_custom_target.html
        add_custom_target(build_doc ${DOC_DIR}
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM
        )

    endif(DOXYGEN_FOUND)

endif(doc)
