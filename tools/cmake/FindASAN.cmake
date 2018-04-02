################################################################################
# AddressSanitizer
################################################################################

# AddressSanitizer is enabled by default.
option(ASAN "Enable AddressSanitizer." ON)

# Check that the build configuration is not imcompatible.
if(ASAN AND (TSAN OR MSAN OR UBSAN))
    # Command: https://cmake.org/cmake/help/v3.10/command/message.html
    message(FATAL_ERROR "AddressSanitizer is not compatible with ThreadSanitizer"
            "MemorySanitizer or UndefinedBehaviorSanitizer.")
endif(ASAN AND (TSAN OR MSAN OR UBSAN))

if(ASAN)
    # Set compiler flags for AddressSanitizer.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
endif(ASAN)

#! add_asan_static_link : Adds static linking to AddressSanitizer library
# \param:TARGET TARGET specify the target to be linked against.
function(add_asan_static_link TARGET)
    if(ASAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        target_link_libraries(${TARGET} "-static-libasan")
    endif(ASAN AND CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
endfunction(add_asan_static_link TARGET)

#! add_asan_wrapper : Adds a asan wrapper script to target directory
# This script is used to define the LD_PRELOAD and ASAN_OPTIONS environment
# variables while running the original executable.
# \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
# \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
function(add_asan_wrapper EXECUTABLE PRELOAD)
    if(ASAN)
        # Define sanitizer library, environment variable and options values.
        set(SANITIZER_LIBRARY)
        set(SANITIZER_ENVIRON "ASAN_OPTIONS")

        # Check whether the libasan should be preloaded.
        if(PRELOAD)

            # TODO: Check that executable has dynamic linking to asan.

            if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
                # Find asan library from the system. Command:
                # https://cmake.org/cmake/help/v3.10/command/find_library.html
                find_library(SANITIZER_LIBRARY NAMES
                        asan libasan.so.4 libasan.so.3 libasan.so.2 libasan.so.1)
            elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                # TODO: How to find correct asan library when using clang?
                find_library(SANITIZER_LIBRARY NAMES
                        asan libclang_rt.asan-x86_64.so)
            endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

            # Assert that the libasan was found.
            if(NOT EXISTS ${SANITIZER_LIBRARY})
                message(SEND_ERROR "Failed to create asan wrapper for ${EXECUTABLE},\
                    unable to find library 'libasan'.")
            endif(NOT EXISTS ${SANITIZER_LIBRARY})
        endif(PRELOAD)

        # Check whether LeakSanitizer should be disabled or not. Usually enabled
        # with the AddressSanitizer (recommended option).
        if(NOT LSAN)
            set(ENV{${SANITIZER_ENVIRON}} "detect_leaks=0")
        endif(NOT LSAN)

        # Add the wrapper script for sanitizer.
        add_sanitizer_script(${EXECUTABLE} ${SANITIZER_ENVIRON} ${SANITIZER_LIBRARY})

    endif(ASAN)
endfunction(add_asan_wrapper EXECUTABLE PRELOAD)
