################################################################################
# cpplint
################################################################################

# cpplint is enabled by default.
option(cpplint "Enable cpplint." ON)

if(cpplint)

    # Check if cpplint is installed with find_program.
    find_program(CPPLINT_PROGRAM cpplint)

    if(CPPLINT_PROGRAM)

        # Set cpplint program + options.
        set(CPPLINT_RUNNABLE
                ${CPPLINT_PROGRAM}
                --quiet)

        # Set cpplint for all sub-projects of the main project.
        set(CMAKE_C_CPPLINT "${CPPLINT_RUNNABLE}")
        set(CMAKE_CXX_CPPLINT "${CPPLINT_RUNNABLE}")

        # Define a function for enabling cpplint per target. The option above
        # makes this redundant, as it enables it for all targets already. This
        # could be used in combination with custom target commands or by
        # overriding the add_* commands with custom implementation.
        function(enable_cpplint TARGET)
            set_target_properties(
                    ${test_name} PROPERTIES
                    C_CPPLINT "${CPPLINT_RUNNABLE}"
                    CXX_CPPLINT "${CPPLINT_RUNNABLE}"
            )
        endfunction(enable_cpplint)

    else(CPPLINT_PROGRAM)
        message(WARNING "Program 'cpplint' not found, unable to run 'cpplint'.")
    endif(CPPLINT_PROGRAM)
endif(cpplint)
