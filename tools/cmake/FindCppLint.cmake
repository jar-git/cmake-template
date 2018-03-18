# https://cmake.org/cmake/help/v3.5/command/option.html
option(cpplint "Enable cpplint." ON)

# Check if clang-tidy is installed, see:
# https://cmake.org/cmake/help/v3.5/command/find_program.html
find_program(CPPLINT_PROGRAM cpplint)

if(cpplint)
    if(CPPLINT_PROGRAM)
        # Set cpplint for all sub-projects of the main project
        set(CMAKE_CXX_CPPLINT "${CPPLINT_PROGRAM}")
    else()
        message(WARNING "Program 'cpplint' not found, unable to run cpplint.")
    endif()
endif()