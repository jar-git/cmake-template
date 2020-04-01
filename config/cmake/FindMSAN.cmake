################################################################################
# MemorySanitizer
################################################################################
# Note: libraries will generate alot of false positivies. See:
# https://github.com/google/sanitizers/wiki/MemorySanitizerLibcxxHowTo

# MemorySanitizer is disabled by default.
option(MSAN "Enable MemorySanitizer." OFF)

# Check that the build configuration is not imcompatible.
if(MSAN AND (ASAN OR TSAN OR UBSAN))
    # Fail out if we have incompatible configuration.
    message(FATAL_ERROR "MemorySanitizer is not compatible with AddressSanitizer"
            "ThreadSanitizer or UndefinedBehaviorSanitizer.")
endif(MSAN AND (ASAN OR TSAN OR UBSAN))

# Check that the compiler is not imcompatible with memory sanitizer.
if(MSAN AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Fail out if we have incompatible configuration.
    message(FATAL_ERROR "MemorySanitizer is only supported by clang.")
endif(MSAN AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")

if(MSAN)
    # Set compiler flags for MemorySanitizer.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O1 -fsanitize=memory -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=memory -fno-omit-frame-pointer")
endif(MSAN)

#! add_msan_wrapper : Adds a msan wrapper script to target directory
# This script is used to define the LD_PRELOAD and MSAN_OPTIONS environment
# variables while running the original executable.
# \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
# \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
function(add_msan_wrapper EXECUTABLE PRELOAD)
    if(MSAN)
        # Define sanitizer library, environment variable and options values.
        set(SANITIZER_LIBRARY " ")
        set(SANITIZER_ENVIRON "MSAN_OPTIONS")

        # Ignore preload, there is no dynamic linking option available for msan.

        # Add the wrapper script for sanitizer.
        add_sanitizer_script(${EXECUTABLE} ${SANITIZER_ENVIRON} ${SANITIZER_LIBRARY})

    endif(MSAN)
endfunction(add_msan_wrapper EXECUTABLE PRELOAD)
