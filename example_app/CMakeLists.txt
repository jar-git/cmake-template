################################################################################
# Example application sub-project.
################################################################################

# Define library name, version and language.
project(application VERSION 0.0.1 LANGUAGES CXX)

# Executable target definition. CMake-build-system documentation:
# https://cmake.org/cmake/help/v3.8/manual/cmake-buildsystem.7.html
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Alias target for the executable to be used outside of the project. Command:
# https://cmake.org/cmake/help/v3.8/command/add_executable.html
add_executable(app::exe ALIAS ${PROJECT_NAME})

# Add sources to the target (using target_sources allows earlier definition
# of the add_library and additional sources maybe taken from anywhere in
# the system to the target build). Defining the target sources with public
# attribute allows to populate the target property INTERFACE_SOURCES, and
# this allows the test project to use the target property directly. Command:
# https://cmake.org/cmake/help/v3.8/command/target_sources.html
target_sources(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/app/application.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/application.cpp)

# Define libraries the target links against. Target property LINK_LIBRARIES
# will be populated with a list of the linked libraries. Command:
# https://cmake.org/cmake/help/v3.8/command/target_link_libraries.html
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        lib::shared
        lib::static
        lib::header
        Threads::Threads)

# Define the include directories for header files for the target. Command:
# https://cmake.org/cmake/help/v3.8/command/target_include_directories.html
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Define the compile options of the target. This will populate the target
# property COMPILE_OPTIONS, that could be used with the unit test. Command:
# https://cmake.org/cmake/help/v3.8/command/target_compile_options.html
# https://cmake.org/cmake/help/v3.8/variable/CMAKE_LANG_COMPILER_ID.html
target_compile_options(${PROJECT_NAME}
        PUBLIC
            $<$<CXX_COMPILER_ID:GNU>: -Wextra -Wall -Wpedantic>
            $<$<CXX_COMPILER_ID:Clang>: -Wextra -Wall -pedantic>
            $<$<CXX_COMPILER_ID:MSVC>: /Wall /analyze>)

# Compiler options for the compilers defined above:
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options
# https://clang.llvm.org/docs/UsersManual.html
# https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx

# Add unit tests.
add_subdirectory(test)
add_subdirectory(bench)