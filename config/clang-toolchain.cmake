# Copyright 2017 Jani Arola, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Tool-chains: https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html

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
set(CMAKE_C_COMPILER clang CACHE string "Clang C compiler" FORCE)
set(CMAKE_CXX_COMPILER clang++ CACHE string "Clang C++ compiler" FORCE)

# TRY-RUN is not supported for cross compiling, so disable it.
set(THREADS_PTHREAD_ARG "ON" CACHE string "Don't try." FORCE)

# Set linkers and other build tools.
set(CMAKE_AR      "/usr/bin/llvm-ar")
set(CMAKE_LINKER  "/usr/bin/llvm-ld")
set(CMAKE_NM      "/usr/bin/llvm-nm")
set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
set(CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
