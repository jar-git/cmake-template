################################################################################
# ccache enables faster builds
################################################################################

# Enable ccache by default. Command:
# https://cmake.org/cmake/help/v3.8/command/option.html
option(ccache "Enable ccache." ON)

# Check if ccache is installed with find_program. Command:
# https://cmake.org/cmake/help/v3.8/command/find_program.html
find_program(CCACHE_DIR ccache)

if(ccache)
    if(EXISTS ${CCACHE_DIR})
        set(CMAKE_C_COMPILER_LAUNCHER "ccache")
        set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")
    else()
        message(WARNING "Failed to enable ccache, program 'ccache' missing.")
    endif()
endif()
