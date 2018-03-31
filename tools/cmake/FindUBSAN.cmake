#  -fsanitize=undefined

option(UBSAN "Enable UndefinedBehaviorSanitizer." OFF)


if(UBSAN AND (ASAN OR TSAN OR MSAN))
    # Fail out if we have incompatible configuration.
    message(FATAL_ERROR "UndefinedBehaviorSanitizer is not compatible with AddressSanitizer"
            "ThreadSanitizer or MemorySanitizer.")
endif()
