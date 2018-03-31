################################################################################
# Command overrides / Other misc functions
################################################################################
# Include guard for commands module. Command:
# https://cmake.org/cmake/help/v3.11/command/include_guard.html
include_guard(GLOBAL)

#! add_library : Overrides add_library command.
# \argn: See add_library command.
# \param:TARGET TARGET specify the target
function(add_library TARGET)
    # Command may only be overridden once, otherwise the original command is lost,
    # as the overridden command is always stored with an underscore prefix.
    _add_library(${TARGET} ${ARGN})
endfunction(add_library)

#! add_library : Overrides add_executable command.
# \argn: See add_library command.
# \param:TARGET TARGET specify the target
function(add_executable TARGET)
    _add_executable(${TARGET} ${ARGN})
endfunction(add_executable)

#! add_permissions : Adds permissions to a file.
# See https://cmake.org/cmake/help/v3.10/command/install.html for list of
# possible permissions.
# \argn: File permissions.
# \param:FILE FILE specify the target file
function(add_permissions FILE)

    # Get filename without the extension from the input file. Command:
    # https://cmake.org/cmake/help/v3.10/command/get_filename_component.html
    get_filename_component(FILENAME ${FILE} NAME)

    # Create temporary directory for the temporary file.
    set(TEMPORARY_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tmp")
    file(MAKE_DIRECTORY "${TEMPORARY_DIRECTORY}")

    # Set filename for the temporary file.
    set(TEMPORARY_FILE "${TEMPORARY_DIRECTORY}/${FILENAME}")

    # Create the temporary file and remove original.
    file(READ "${FILE}" FILE_CONTENT)
    file(WRITE ${TEMPORARY_FILE} ${FILE_CONTENT})
    file(REMOVE ${FILE} ${FILE_CONTENT})

    # Re-create original with new permissions.
    file(COPY "${TEMPORARY_FILE}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" FILE_PERMISSIONS ${ARGN})

    # Remove the temporary file.
    file(REMOVE "${TEMPORARY_FILE}")

endfunction(add_permissions)
