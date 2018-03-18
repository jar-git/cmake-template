option(ccache "Enable ccache." ON)
# Check if ccache is installed, see:
# https://cmake.org/cmake/help/v3.5/command/find_program.html
find_program(CCACHE_DIR ccache)

if(ccache)
    if(EXISTS ${CCACHE_DIR})
        # ccache enables faster builds, see:
        # https://ccache.samba.org/
        set(CMAKE_C_COMPILER_LAUNCHER "ccache")
        set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")
    else()
        message(WARNING "Failed to enable ccache, program 'ccache' missing.")
    endif()
endif()
