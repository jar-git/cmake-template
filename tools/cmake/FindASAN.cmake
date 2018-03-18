# Enable or disable AddressSanitizer.
option(ASAN "Enable AddressSanitizer." ON)

#if(ASAN AND (TSAN OR MSAN OR UBSAN))
# Fail out if we have incompatible configuration. Command help:
# https://cmake.org/cmake/help/v3.6/command/message.html
#    message(FATAL_ERROR "AddressSanitizer is not compatible with ThreadSanitizer"
#            "MemorySanitizer or UndefinedBehaviorSanitizer.")
#endif()

if(ASAN)

    # set(ENV{ASAN_OPTIONS} "abort_on_error=1 print_suppressions=0")
    # TODO:
    # if (target executable == unsanitized and links with shared lib) then
    #find_library(LIBASAN NAMES asan libasan.so.4 libasan.so.3 libasan.so.2 libasan.so.1)

    #set(ENV{LD_PRELOAD} ${LIBASAN})

    # TODO:
    # if (gcc) -static-libasan (statically link asan).

    #set(COMPILER_ANALYZER_FLAGS -g -O1 -fsanitize=address -fno-omit-frame-pointer CACHE INTERNAL "" FORCE)
    #set(LD_ANALYZER_FLAGS -static-libasan CACHE INTERNAL "" FORCE)

    #mark_as_advanced(${COMPILER_ANALYZER_FLAGS})
    #mark_as_advanced(${LD_ANALYZER_FLAGS})

endif()
