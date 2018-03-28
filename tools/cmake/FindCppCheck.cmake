################################################################################
# cppcheck
################################################################################

# cppcheck is enabled by default.
option(cppcheck "Enable cppcheck." ON)

if(cppcheck)

    # Check if cppcheck is installed with find_program.
    find_program(CPPCHECK_PROGRAM cppcheck)

    if(CPPCHECK_PROGRAM)

        # Set cppcheck cache directory
        set(CPPCHECK_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/cppcheck_cache)

        # Set cppcheck supressions file.
        set(CPPCHECK_SUPRESSIONS ${CMAKE_CURRENT_SOURCE_DIR}/config/cppcheck.cfg)

        # Set message template
        set(CPPCHECK_TEMPLATE "[{file}:{line}] ({severity}) {message} ({id})")

        # Set cppcheck program + options.
        set(CPPCHECK_RUNNABLE
                ${CPPCHECK_PROGRAM}
                --enable=all
                --quiet
                --inline-suppr
                --platform=native
                --cppcheck-build-dir=${CPPCHECK_BUILD_DIR}
                --template=${CPPCHECK_TEMPLATE}
                --suppressions-list=${CPPCHECK_SUPRESSIONS}
                --std=c++14)

        # Set cppcheck for all sub-projects of the main project.
        set(CMAKE_C_CPPCHECK "${CPPCHECK_RUNNABLE}")
        set(CMAKE_CXX_CPPCHECK "${CPPCHECK_RUNNABLE}")

        # Define a function for enabling cppcheck per target. The option above
        # makes this redundant, as it enables it for all targets already. This
        # could be used in combination with custom target commands or by
        # overriding the add_* commands with custom implementation.
        function(enable_cppcheck TARGET)
            set_target_properties(
                    ${test_name} PROPERTIES
                    C_CPPCHECK "${CPPCHECK_RUNNABLE}"
                    CXX_CPPCHECK "${CPPCHECK_RUNNABLE}"
            )
        endfunction(enable_cppcheck)

    else(CPPCHECK_PROGRAM)
        message(WARNING "Program 'cppcheck' not found, unable to run 'cppcheck'.")
    endif(CPPCHECK_PROGRAM)
endif(cppcheck)
