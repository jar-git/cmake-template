# Option for building doxygen documentation.
option(doc "Build documentation" OFF)

if (doc)
    # Find doxygen, Module help:
    # https://cmake.org/cmake/help/v3.10/module/FindDoxygen.html
    find_package(Doxygen)

    if (DOXYGEN_FOUND)

        # Set input and output file for configuration.
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/config/doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
        # Set doxygen documentation directory.
        set(DOC_DIR documentation)
        # Configure the doxygen configuration file and copy it from source
        # directory to build directory (replace variable references).
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        # Add custom target for building the documentation. Command help:
        # https://cmake.org/cmake/help/v3.10/command/add_custom_target.html
        add_custom_target(build_doc ${DOC_DIR}
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM)

    endif(DOXYGEN_FOUND)

endif(doc)
