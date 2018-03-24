set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
# set(CMAKE_SYSROOT /usr)

# See: https://clang.llvm.org/docs/CrossCompilation.html
# set(clang-options unknown)

set(CMAKE_C_COMPILER clang CACHE sring "Clang C compiler" FORCE)
# set(CMAKE_C_COMPILER_TARGET ${clang-options})
set(CMAKE_CXX_COMPILER clang++ CACHE string "Clang C++ compiler" FORCE)
# set(CMAKE_CXX_COMPILER_TARGET ${clang-options})

# Needs to be set when cross compiling.
set(THREADS_PTHREAD_ARG "ON" CACHE string "Don't try." FORCE)

# set(CMAKE_AR      "/usr/bin/llvm-ar")
# https://lld.llvm.org/
# set(CMAKE_LINKER  "/usr/bin/llvm-ld")
# set(CMAKE_NM      "/usr/bin/llvm-nm")
# set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
# set(CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

