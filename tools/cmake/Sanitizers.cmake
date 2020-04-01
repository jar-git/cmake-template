################################################################################
# Sanitizers
################################################################################
# Include guard for sanitizers module.
include_guard(GLOBAL)

#! add_sanitizer_wrapper : Adds a sanitizer wrapper script to target directory
# Facade function for creating correct sanitizer wrapper for the executable.
# \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
# \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
function(add_sanitizer_wrapper EXECUTABLE PRELOAD)
    add_asan_wrapper(${EXECUTABLE} ${PRELOAD})
    add_lsan_wrapper(${EXECUTABLE} ${PRELOAD})
    add_tsan_wrapper(${EXECUTABLE} ${PRELOAD})
    add_ubsan_wrapper(${EXECUTABLE} ${PRELOAD})
    add_msan_wrapper(${EXECUTABLE} ${PRELOAD})
endfunction(add_sanitizer_wrapper EXECUTABLE PRELOAD)

#! add_sanitizer_static_link : Adds static linking to sanitizer library
# Facade function for adding static linking to the correct sanitizer library.
# \param:TARGET TARGET specify the target to be linked against.
function(add_sanitizer_static_link TARGET)
    add_asan_static_link(${TARGET})
    add_lsan_static_link(${TARGET})
    add_tsan_static_link(${TARGET})
    add_ubsan_static_link(${TARGET})
endfunction(add_sanitizer_static_link TARGET)

#! add_sanitizer_script : Adds a sanitizer wrapper script to target directory
# This script is used to define the LD_PRELOAD and <SAN>_OPTIONS environment
# variable while running the original executable.
# \param:EXECUTABLE EXECUTABLE specify the target for the wrapper.
# \param:SANITIZER_ENVIRON SANITIZER_ENVIRON Specifies the sanitizer environment variable.
# \param:SANITIZER_LIBRARY SANITIZER_LIBRARY Specifies the library file to be preloaded.
function(add_sanitizer_script EXECUTABLE SANITIZER_ENVIRON SANITIZER_LIBRARY)

    # Configure generic options for all sanitizers. Environment:
    # https://cmake.org/cmake/help/latest/variable/ENV.html
    set(SANITIZER_OPTIONS
            "$ENV{${SANITIZER_ENVIRON}} abort_on_error=1 print_suppressions=0")

    # Set asan wrapper template path.
    set(ASAN_WRAPPER ${CMAKE_HOME_DIRECTORY}/tools/sanitizer_wrapper.py)
    # Define name for the executable specific asan wrapper.
    set(EXE_ASAN_WRAPPER ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.py)
    # Add directory to the executable.
    set(EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE})

    # Configure the asan wrapper template.
    configure_file(${ASAN_WRAPPER} ${EXE_ASAN_WRAPPER} @ONLY)

    # Add execute and read permissions to the wrapper.
    add_permissions(${EXE_ASAN_WRAPPER} OWNER_EXECUTE OWNER_READ)

endfunction(add_sanitizer_script EXECUTABLE SANITIZER_ENVIRON SANITIZER_LIBRARY)

