# Define minimum CMake version. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-language.7.html
cmake_minimum_required(VERSION 3.5)

# Define project name, version and language.
project(root-project VERSION 0.0.1 LANGUAGES CXX)

# Define C++ standard version. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-variables.7.html
set(CMAKE_CXX_STANDARD 17)

# Enable testing for the sub projects.
enable_testing()

# Require threading libraries. See:
# https://cmake.org/cmake/help/v3.5/manual/cmake-modules.7.html
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(tools)
add_subdirectory(thirdparty)
add_subdirectory(example_static_lib)
add_subdirectory(example_shared_lib)
add_subdirectory(example_app)

