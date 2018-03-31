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

    #! add_asan_static_link : Adds static linking to AddressSanitizer library
    # \param:TARGET TARGET specify the target to be linked against.
    function(add_asan_static_link TARGET)
        target_link_libraries(${TARGET} "-static-libasan")
    endfunction(add_asan_static_link TARGET)

    #! add_asan_wrapper : Adds a asan wrapper script to target directory
    # This script is used to define the LD_PRELOAD and ASAN_OPTIONS environment
    # variables while running the original executable.
    # \param:EXECUTABLE EXECUTABLE specify the target to be wrapped.
    # \param:PRELOAD PRELOAD Specifies if libasan should be preloaded.
    function(add_asan_wrapper EXECUTABLE PRELOAD)

        # TODO: Check that executable has dynamic linking to asan (no point otherwise).

        # Define sanitizer library.
        set(SANITIZER_LIBRARY)

        # Set asanlib if option preload has been defined.
        if(PRELOAD)

            if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

                # Find asan library from the system. Command:
                # https://cmake.org/cmake/help/v3.10/command/find_library.html
                find_library(SANITIZER_LIBRARY NAMES
                    asan libasan.so.4 libasan.so.3 libasan.so.2 libasan.so.1)

            elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

                #TODO: Implement preloading and dynamic linking for clang.
                message(SEND_ERROR "PRELOAD has not been implemented for clang.")
                #find_library(SANITIZER_LIBRARY NAMES
                #    libclang_rt.asan-x86_64.so)

            endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

            if(NOT EXISTS ${LIBASAN})

                message(SEND_ERROR "Failed to create asan wrapper for ${EXECUTABLE},\
                    unable to find library 'libasan'.")

            endif(NOT EXISTS ${LIBASAN})

        endif(PRELOAD)

        # Configure ASAN_OPTIONS environment variable. Environment:
        # https://cmake.org/cmake/help/v3.10/variable/ENV.html
        set(SANITIZER_OPTIONS "$ENV{ASAN_OPTIONS} abort_on_error=1 print_suppressions=0")

        # Make the python wrapper for sanitizer.
        add_sanitizer_script(${EXECUTABLE} ${SANITIZER_OPTIONS} ${SANITIZER_LIBRARY})

   endfunction(add_asan_wrapper EXECUTABLE PRELOAD)

endif(ASAN)
