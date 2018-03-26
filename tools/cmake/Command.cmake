################################################################################
# Command overrides
################################################################################

# Facade function for enabling all static analysis tools.
function(add_static_analysis TARGET)
    enable_clang_tidy(${TARGET})
    enable_iwyu(${TARGET})
    enable_cpplint(${TARGET})
endfunction(add_static_analysis)

# Override add_library command.
function(add_library TARGET)
    _add_library(${TARGET} ${ARGN})
    add_static_analysis(${TARGET})
endfunction(add_library)

# Override add_executable command.
function(add_executable TARGET)
    _add_executable(${TARGET} ${ARGN})
    add_static_analysis(${TARGET})
endfunction(add_executable)

# Custom add_unit_test command.
function(add_unit_test NAME)
    # TODO: Implement custom steps for unit test.
    _add_executable(${NAME} ${ARGN})
    add_test(${NAME} ${NAME})
endfunction(add_unit_test)

# Custom add_performance_test command.
function(add_bench_test NAME)
    # TODO: Implement custom steps for benchmark test.
    add_test(${NAME} ${NAME})
    set_tests_properties(${NAME} PROPERTIES LABELS "BenchmarkTest")
endfunction(add_bench_test)
