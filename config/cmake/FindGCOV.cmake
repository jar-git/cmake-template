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

option(coverage "Enable coverage build." OFF)

function(report_uninstalled PROGRAM)
    message(FATAL_ERROR "Failed to process coverage option. Program '${PROGRAM}' has not been installed.")
endfunction()

if(coverage)

    # Set coverage report directory
    set(COVERAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/coverage)
    # Set source directories
    # set(SOURCE_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)

    file(MAKE_DIRECTORY ${COVERAGE_DIR})

    # GNU compilers use gcov for coverage.
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

        # Check that gcovr is installed.
        find_program(GCOVR gcovr)

        # cgov documentation: https://gcc.gnu.org/onlinedocs/gcc/Gcov.html
        # gcovr documentation: http://gcovr.com/guide.html
        if(GCOVR)

            set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

            set(CMAKE_C_FLAGS "-g -O0 --coverage" CACHE INTERNAL "")
            set(CMAKE_CXX_FLAGS "-g -O0 --coverage" CACHE INTERNAL "")

            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 --coverage" CACHE INTERNAL "")
            set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 --coverage" CACHE INTERNAL "")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -O0 --coverage" CACHE INTERNAL "")

            add_custom_target(coverage_${PROJECT_NAME}
                gcovr --root=${CMAKE_CURRENT_SOURCE_DIR} .
                      --exclude-directories={test|doc|bench}
                      --html --html-details
                      --output=${CMAKE_CURRENT_BINARY_DIR}/coverage/index.html
                      --object-directory=${CMAKE_CURRENT_BINARY_DIR}/impl
                      --delete --print-summary --exclude-unreachable-branches
            )

            add_dependencies(coverage coverage_${PROJECT_NAME})

        else()
            report_uninstalled("gcovr")
        endif()

    # LLVM compilers use llvm-cov for coverage.
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

        # Check that llvm-cov is installed.
        find_program(LLVM-COV llvm-cov)

        # llvm-cov documentation: https://llvm.org/docs/CommandGuide/llvm-cov.html
        # llvm coverage documentation : https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
        if(LLVM-COV)

            # Set profile output file name and directory.
            set(ENV{LLVM_PROFILE_FILE} ${COVERAGE_DIR}/coverage.profraw)

            # llvm-profdata merge -sparse coverage.profraw -o coverage.profdata
            # llvm-cov show ./foo -instr-profile=coverage.profdata -format=html -output-dir=${COVERAGE_DIR}

            set(CMAKE_C_FLAGS "-fprofile-instr-generate -fcoverage-mapping" CACHE INTERNAL "")
            set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping" CACHE INTERNAL "")

            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping" CACHE INTERNAL "")
            set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping" CACHE INTERNAL "")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping" CACHE INTERNAL "")

        else()
            report_uninstalled("llvm-cov")
        endif()

    endif()

endif()
