################################################################################
# clang-tidy
################################################################################

# clang-tidy is enabled by default.
option(tidy "Enable clang-tidy." ON)

if(tidy)

    # Check if clang-tidy is installed with find_program.
    find_program(CLANG_TIDY_PROGRAM clang-tidy)

    if(CLANG_TIDY_PROGRAM)

        # Set clang-tidy for all sub-projects of the main project.
        set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_PROGRAM}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PROGRAM}")

        # Define a function for enabling clang-tidy per target. The option above
        # makes this redundant, as it enables it for all targets already. This
        # could be used in combination with custom target commands or by
        # overriding the add_* commands with custom implementation.
        function(enable_clang_tidy TARGET)
            set_target_properties(
                    ${test_name} PROPERTIES
                    C_CLANG_TIDY "${CLANG_TIDY_PROGRAM}"
                    CXX_CLANG_TIDY "${CLANG_TIDY_PROGRAM}"
            )
        endfunction(enable_clang_tidy)

    else(CLANG_TIDY_PROGRAM)
        message(WARNING "Program 'clang-tidy' not found, unable to run 'clang-tidy'.")
    endif(CLANG_TIDY_PROGRAM)
endif(tidy)
