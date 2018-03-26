################################################################################
# include-what-you-use (iwyu)
################################################################################

# include-what-you-use is enabled by default.
option(IWYU "Enable include-what-you-use." ON)

# https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUPragmas.md
# https://include-what-you-use.org/

if(IWYU)

    # Check if iwyu is installed with find_program.
    find_program(IWYU_PROGRAM iwyu)

    if(IWYU_PROGRAM)

        # Set iwyu mapping file.
        set(IWYU_MAP ${CMAKE_CURRENT_SOURCE_DIR}/config/iwyu.cfg)

        # Set iwyu program + options.
        set(IWYU_RUNNABLE
                ${IWYU_PROGRAM}
                -Xiwyu
                --mapping_file=${IWYU_MAP})

        # Set iwyu for all sub-projects of the main project.
        set(CMAKE_C_INCLUDE_WHAT_YOU_USE "${IWYU_RUNNABLE}")
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_RUNNABLE}")

        # Define a function for enabling iwyu per target. The option above makes
        # this redundant, as it enables it for all targets already. This could
        # be used in combination with custom target commands or by overriding
        # the add_* commands with custom implementation.
        function(enable_iwyu TARGET)
            set_target_properties(
                    ${test_name} PROPERTIES
                    C_INCLUDE_WHAT_YOU_USE "${IWYU_RUNNABLE}"
                    CXX_INCLUDE_WHAT_YOU_USE "${IWYU_RUNNABLE}"
            )
        endfunction(enable_iwyu)

    else(IWYU_PROGRAM)
        message(WARNING "Program 'iwyu' not found, unable to run 'include-what-you-use'.")
    endif(IWYU_PROGRAM)
endif(IWYU)
