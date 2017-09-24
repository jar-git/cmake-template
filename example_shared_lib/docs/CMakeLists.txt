# Define minimum CMake version.
cmake_minimum_required(VERSION 3.5)

# Option for building doxygen documentation.
option(BUILD_DOC "Build documentation" ON)

if (BUILD_DOC)
    # Find if Doxygen is installed. See:
    # https://cmake.org/cmake/help/v3.5/module/FindDoxygen.html
    find_package(Doxygen)

    if (DOXYGEN_FOUND)

        # Set input and output files.
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
        # Set doxygen documentation directory.
        set(DOC_DIR documentation)

        # Configure the configuration file and copy it from source directory to
        # build directory (replace variable references). See:
        # https://cmake.org/cmake/help/v3.5/command/configure_file.html
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        # note the option ALL which allows to build the docs together with
        # the application. See:
        # https://cmake.org/cmake/help/v3.5/command/add_custom_target.html
        add_custom_target(${DOC_DIR} ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM)

    endif(DOXYGEN_FOUND)

endif(BUILD_DOC)
