# Define library name, version and language.
project(header-only VERSION 0.0.1 LANGUAGES CXX)

# Interface library target definition as a header-only library.
add_library(${PROJECT_NAME} INTERFACE)

# Alias target for the interface library to be used outside of the project.
add_library(header::only ALIAS ${PROJECT_NAME})

# Include directories for the header-only library.
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:inc>)
