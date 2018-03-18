# https://cmake.org/cmake/help/v3.5/command/option.html
option(IWYU "Enable include-what-you-use." OFF)

# Check if IWYU-tidy is installed, see:
# https://cmake.org/cmake/help/v3.5/command/find_program.html
find_program(IWYU_PROGRAM iwyu)

if(IWYU)
    if(IWYU_PROGRAM)
        # Set IWYU for all sub-projects of the main project
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_PROGRAM}")
        set(CMAKE_C_INCLUDE_WHAT_YOU_USE "${IWYU_PROGRAM}")
    else()
        message(WARNING "Program 'iwyu' not found, unable to run include-what-you-use.")
    endif()
endif()
