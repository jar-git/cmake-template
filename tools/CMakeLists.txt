# Define minimum CMake version.
cmake_minimum_required(VERSION 3.5)

# Configure file or copy from script folder to build folder. See:
# https://cmake.org/cmake/help/v3.5/command/configure_file.html
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/example_script.sh
               ${CMAKE_CURRENT_BINARY_DIR}/script.sh COPYONLY)
