# -fsanitize=memory -fPIE -pie -fno-omit-frame-pointer -g -O2


if(MSAN AND (ASAN OR TSAN OR UBSAN))
    # Fail out if we have incompatible configuration.
    message(FATAL_ERROR "MemorySanitizer is not compatible with AddressSanitizer"
            "ThreadSanitizer or UndefinedBehaviorSanitizer.")
endif()
