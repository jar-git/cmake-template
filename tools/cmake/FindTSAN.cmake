# -O2 -g -fsanitize=thread

if(TSAN AND (ASAN OR MSAN OR UBSAN))
    # Fail out if we have incompatible configuration.
    message(FATAL_ERROR "ThreadSanitizer is not compatible with AddressSanitizer"
            "MemorySanitizer or UndefinedBehaviorSanitizer.")
endif()
