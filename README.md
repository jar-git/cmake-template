# Template project for CMake

Purpose of this project (cmake-template) is to learn how to use various CMake features, and how to embed the tools that 
are useful for C/C++ development to the project as CMake modules or scripts.

## Requirements
This template won't work without the following unless modified:
* CMake v3.10 and above.
* C++17 and above.
* GCC-7 and above (or clang-6 and above).

MSVC is not supported.

## Project structure

The main project (root) contains all of the sub-projects, third party dependencies (not sources), build-scripts 
and custom CMake modules.

<pre>
 root               - Main project
  |
  +--/build         - Temporary directory for build objects
  |
  +--/config        - Project wide configuration files
  |  |
  |  +--/cmake      - CMake modules used to embed the tools to project
  |
  +--/dependencies  - Sub-project for common 3rd party dependencies
  |
  +--/exe_service   - Sub-project with executable target
  |
  +--/lib_header    - Sub-project with header-only library target
  |
  +--/lib_shared    - Sub-project with shared library target
  |
  +--/lib_static    - Sub-project with static library target
  |
  +--/scripts       - Scripts

</pre>

## Sub-project structure

Each target (executables, libraries and header-only libraries) in the template project is a sub-project of the main 
project (root). The directory structure for each target type is displayed below.

<pre>
 exe             - Sub project
  |
  +--/inc        - Directory for the public header files of this sub-project
  |
  +--/src        - Directory for the source- and private header files of this sub-project
  |
  +--/test       - Test directory for this sub-project
    |
    +--/bench    - Benchmark directory for this sub-project (google bench)
    |
    +--/unit     - Unit test directory for this sub-project (gtest)

</pre>

## List of tools used by the template

It is expected that the tools are installed on the system and found from the PATH 
environment variable. CMake options control whether an individual tool is *ON* or 
*OFF*. Some of the tools are not compatible, and cannot be enabled with the same 
build.

### Compilers and other build tools

This category contains tools that can be used to makes the compiling and linking 
faster. Only supports GCC and Clang based toolchains.

##### [ccache][1] (default - ON)
> ccache is a compiler cache. It speeds up recompilation by caching previous 
> compilations and detecting when the same compilation is being done again.

To disable ccache pass option **_ccache_** to CMake from the command line 
`-Dccache:BOOL=OFF` or from a build script. 

##### [GNU gold][2] (default - ON)
> gold is an ELF linker.  It is intended to have complete support for ELF and to
> run as fast as possible on modern systems.  For normal use it is a drop-in 
> replacement for the older GNU linker.
  
##### [LLVM gold plugin][3] (default - ON)
> Building with link time optimization requires cooperation from the system
> linker. LTO support on Linux systems requires that you use the gold linker or
> ld.bfd from binutils >= 2.21.51.0.2, as they support LTO via plugins.
  
> The LLVM gold plugin implements the gold plugin interface on top of libLTO.

To use either of the gold linkers instead of the default linker, pass option 
**_gold_** to CMake from the command line `-Dgold:BOOL=ON` or from a build 
script. The appropriate gold linker is selected automatically depending on the 
compiler. If the link has not been installed, setting the option _ON_ will 
generate a warning.

### Static and dynamic analysis tools

Most of the tools in this category are not compatible with each other, the 
exception is clang-tidy (compatible with all others). Also, LeakSanitizer is 
integrated into AddressSanitizer and both enabled at the same time by deafult.

##### [clang-tidy][11] (default - ON)
> clang-tidy is a clang-based C++ “linter” tool. Its purpose is to provide an 
> extensible framework for diagnosing and fixing typical programming errors, 
> like style violations, interface misuse, or bugs that can be deduced via 
> static analysis. 

To disable clang-tidy pass option **_tidy_** to CMake from the command line 
`-Dtidy:BOOL=OFF` or from a build script. A configuration file exists for 
individual clang-tidy options, the file is located in the root directory 
(.clang-tidy). It is generated with the '-dump-config' switch.

The default clang-tidy flags are `-checks=\*` (from the configuration file).

##### [cgovr][4] (default - OFF)
> Gcovr provides a utility for managing the use of the GNU gcov utility and 
> generating summarized code coverage results.

##### [llvm-cov][5] (default - OFF)
> The llvm-cov tool shows code coverage information for programs that are 
> instrumented to emit profile data. It can be used to work with gcov-style
> coverage or with clang’s instrumentation based profiling.

To use either of the code coverage tools, pass option **_coverage_** to CMake 
from the command line `-Dcoverage:BOOL=ON` or from a build script. The 
appropriate coverage tool is selected automatically depending on the compiler. 
If coverage tool has not been installed, setting the option ON will generate a 
warning.

The default coverage flags are `-O0 -fprofile-arcs -ftest-coverage -fPIC`. By 
default the coverage option generates a HTML web page for each file. The pages 
are generated to the `build/coverage/` directory. 

##### [ASan][6] (default - ON)
> AddressSanitizer is a memory error detector for C/C++.

To disable AddressSanitizer pass option **_ASAN_** to CMake from the command line 
`-DASAN:BOOL=OFF` or from a build script. Enabling any other tool of this category 
will automatically disable AddressSanitizer.

The default ASan flags are `-O1 -g -fno-omit-frame-pointer`. To set [ASan specific][20] 
runtime flags, use the **ASAN_OPTIONS** environment variable, it also accepts 
[common][19] sanitizer runtime flags.


##### [LSan][7] (default - ON)
> LeakSanitizer (is a memory leak detector which is integrated into AddressSanitizer. 

To disable LeakSanitizer pass option **_LSAN_** to CMake from the command line 
`-DLSAN:BOOL=OFF` or from a build script. For stand-alone LSan use the options 
LSAN _ON_ and ASAN _OF_.

There are no separate flags set for LSan by default. To set [LSan specific][22] 
runtime flags, use the **LSAN_OPTIONS** environment variable, it also accepts 
[common][19] sanitizer runtime flags.

##### [TSan][8] (default - OFF)
> ThreadSanitizer is a data race detector for C/C++.

To enable ThreadSanitizer pass option **_TSAN_** to CMake from the command line 
`-DTSAN:BOOL=ON` or from a build script. Enabling TSan will automatically disable 
the other tools of this category.

The default TSan flags are `-O2 -g`. To set [TSan specific][21] runtime flags, 
use the **TSAN_OPTIONS** environment variable, it also accepts [common][19] 
sanitizer runtime flags.

##### [MSan][9] (default - OFF)
> MemorySanitizer is a detector of uninitialized memory reads in C/C++ programs.

To enable MemorySanitizer pass option **_MSAN_** to CMake from the command line 
`-DMSAN:BOOL=ON` or from a build script. Enabling MSan will automatically disable 
the other tools of this category. MSan is only supported by Clang compilers.

The default MSan flags are `-O1 -g -fno-omit-frame-pointer -fsanitize-memory-track-origins 
-fPIE -pie`. To set [MSan runtime][19] flags, use the **MSAN_OPTIONS** 
environment variable.

##### [UBSan][10] (default - OFF)
> UndefinedBehaviorSanitizer is a fast undefined behavior detector for C/C++. 

To enable UndefinedBehaviorSanitizer pass option **_UBSAN_** to CMake from the 
command line `-DUBSAN:BOOL=ON` or from a build script. Enabling UBSan will 
automatically disable the other tools of this category.

The default UBSan flags are `-O1 -g -fno-omit-frame-pointer`. To set [UBSan][19] 
runtime flags, use the **UBSAN_OPTIONS** environment variable.

### Debugging, profiling and documentation tools

This category contains tools that executed separately from the compiling process 
, or optionally, during the compiling process but still as an independent CMake 
target.

##### Debuggers: [gdb][15] and [LLDB][16] (external)

* GNU Project Debugger
> GDB, the GNU Project debugger, allows you to see what is going on `inside' 
> another program while it executes -- or what another program was doing at the 
> moment it crashed.

* The LLDB Debugger
> LLDB is a next generation, high-performance debugger. It is built as a set of 
> reusable components which highly leverage existing libraries in the larger LLVM 
> Project, such as the Clang expression parser and LLVM disassembler.

There are no CMake modules, or scripts for the debuggers, instead they should be 
used from an IDE or command line.

##### [callgrind][14] (default - OFF)
> Callgrind is a profiling tool that records the call history among functions in 
> a program's run as a call-graph. By default, the collected data consists of 
> the number of instructions executed, their relationship to source lines, the 
> caller/callee relationship between functions, and the numbers of such calls. 
> Optionally, cache simulation and/or branch prediction can produce further 
> information about the runtime behavior of an application.

##### [afl][13] (default - OFF)
> American fuzzy lop is a security-oriented fuzzer that employs a novel type of 
> compile-time instrumentation and genetic algorithms to automatically discover 
> clean, interesting test cases that trigger new internal states in the targeted 
> binary. This substantially improves the functional coverage for the fuzzed 
> code.

##### [doxygen][12] (default - OFF)
> Doxygen is the de facto standard tool for generating documentation from 
> annotated C++ sources, but it also supports other popular programming languages 
> such as C, Objective-C, C#, PHP, Java, Python, IDL (Corba, Microsoft, and 
> UNO/OpenOffice flavors), Fortran, VHDL, Tcl, and to some extent D.

To enable documentation pass option **_doc_** to CMake from the command line 
`-Ddoc:BOOL=ON` or from a build script. 

The default doxygen configuration file `config/doxygen.in`. By 
default the coverage option generates a HTML web page for each file. The pages 
are inside the `build/coverage/` directory. 

[1]: https://ccache.samba.org/
[2]: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
[3]: https://llvm.org/docs/GoldPlugin.html
[4]: http://gcovr.com/guide.html
[5]: https://llvm.org/docs/CommandGuide/llvm-cov.html
[6]: https://github.com/google/sanitizers/wiki/AddressSanitizer
[7]: https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer
[8]: https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual
[9]: https://github.com/google/sanitizers/wiki/MemorySanitizer
[10]: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
[11]: http://clang.llvm.org/extra/clang-tidy/
[12]: https://www.stack.nl/~dimitri/doxygen/manual/index.html
[13]: http://lcamtuf.coredump.cx/afl/
[14]: http://valgrind.org/docs/manual/cl-manual.html
[15]: https://sourceware.org/gdb/current/onlinedocs/gdb/
[16]: https://lldb.llvm.org/index.html

[19]: https://github.com/google/sanitizers/wiki/SanitizerCommonFlags
[20]: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
[21]: https://github.com/google/sanitizers/wiki/ThreadSanitizerFlags
[22]: https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer#flags
