################################################################################
# Link time optimization.
################################################################################

# Enable link time optimization by default.
option(gold "Enable gold linker." ON)

# Defines a function that will set gnu gold on when called if it exists. Function:
# https://cmake.org/cmake/help/v3.6/command/function.html
function(enable_gnu_gold LD_VERSION)
    if ("${LD_VERSION}" MATCHES "GNU gold")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
        set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
    else()
        message(WARNING "Failed to enable gold linker, using default linker instead.")
    endif()
endfunction()

if(gold)
    if(CMAKE_COMPILER_IS_GNUCXX)
        # Check the if gold is availalbe with execute_process. Command:
        # https://cmake.org/cmake/help/v3.6/command/execute_process.html
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE LD_VERSION_OUT)
        enable_gnu_gold(${LD_VERSION_OUT})
    elseif(CMAKE_COMPILER_IS_GNUCC)
        execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE LD_VERSION_OUT)
        enable_gnu_gold(${LD_VERSION_OUT})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Clang is using the LLVM Linker instead of the LLVM gold plugin. This is
        # because the LLVM linker is faster. Linker and plugin:
        # The LLVM-Linker     : https://lld.llvm.org/
        # The LLVM gold plugin: https://llvm.org/docs/GoldPlugin.html
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
	    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
    elseif(MSVC)
        # Microsoft Compiler.
        message(FATAL_ERROR "MSVC is not supported yet")
    endif()
endif()
