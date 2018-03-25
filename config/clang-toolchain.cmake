################################################################################
# Clang tool-chain for host system.
################################################################################

# Set host system and processor.
set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})

# Set system root directory.
# set(CMAKE_SYSROOT /)

# Clang cross compilation: https://clang.llvm.org/docs/CrossCompilation.html
# set(clang-options unknown)
# set(CMAKE_C_COMPILER_TARGET ${clang-options})
# set(CMAKE_CXX_COMPILER_TARGET ${clang-options})

# Set C and CXX compilers.
set(CMAKE_C_COMPILER clang CACHE sring "Clang C compiler" FORCE)
set(CMAKE_CXX_COMPILER clang++ CACHE string "Clang C++ compiler" FORCE)

# TRY-RUN is not supported for cross compiling, so disable it.
set(THREADS_PTHREAD_ARG "ON" CACHE string "Don't try." FORCE)

# Set linkers and other build tools.
set(CMAKE_AR      "/usr/bin/llvm-ar")
set(CMAKE_LINKER  "/usr/bin/llvm-ld")
set(CMAKE_NM      "/usr/bin/llvm-nm")
set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
set(CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
