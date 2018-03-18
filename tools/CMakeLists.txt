# Replace ECHO_VALUE from script with the string.
set(ECHO_VALUE "The configuration has been completed.")

# Configure file or copy from script folder to build folder. Command help:
# https://cmake.org/cmake/help/v3.6/command/configure_file.html
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/example_script.sh
               ${CMAKE_CURRENT_BINARY_DIR}/script.sh @ONLY)

# Add execution rights for the configured script. Command help:
# https://cmake.org/cmake/help/v3.6/command/execute_process.html
execute_process(
        COMMAND chmod a+x script.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        ERROR_QUIET)