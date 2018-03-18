# https://cmake.org/cmake/help/v3.5/command/option.html
option(tidy "Enable clang-tidy." ON)

# Check if clang-tidy is installed, see:
# https://cmake.org/cmake/help/v3.5/command/find_program.html
find_program(CLANG_TIDY_PROGRAM clang-tidy)

if(tidy)
    if(CLANG_TIDY_PROGRAM)
        # Set clang-tidy for all sub-projects of the main project
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PROGRAM}")
    else()
        message(WARNING "Program 'clang-tidy' not found, unable to run static analysis.")
    endif()
endif()