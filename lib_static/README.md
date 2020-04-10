# Sub-project lib_static

This sub-project functions as an example of a static library for the CMake template. The main purpose of this 
sub-project is to demonstrate how to use CMake features, the code is a place holder for having something to test, 
analyse and document.

Unit tests and benchmarks also demonstrate how to generate compile definitions. See the CMakeLists.txt files for more.

As for now, this sub-project only compiles if you have a posix compatible environment (e.g. unix, linux).

## Unit tests

Unit tests use [Google Test] [1] as the unit testing framework. These tests are mainly for generating test coverage 
reports with the CMake template. The unit tests can be found under the `test/unit` directory.
 
For gtest features and usage, see the [primer] [2].

### Execute

```
./lib_static/test/unit/sys_unit_test
```

## Benchmarks

Benchmarks use [(google) Benchmark] [3] as the benchmarking framework. These benchmarks are mainly for generating 
performance reports with the CMake template. The benchmarks can be found under the `test/bench` directory.

### Execute

```
sudo cpupower frequency-set --governor performance
./lib_static/test/bench/sys_benchmark --benchmark_counters_tabular=true --benchmark_format=json --benchmark_out=socket.json
sudo cpupower frequency-set --governor powersave
```

### Compare
```
./dependencies/benchmark/benchmark-prefix/src/benchmark/tools/compare.py <benchmark_baseline> <benchmark_contender>
```

If you encounter error `ModuleNotFoundError: No module named 'scipy'`, then you need to install this package. 

```
pip3 install --user scipy
```

[1]: https://github.com/google/googletest
[2]: https://github.com/google/googletest/blob/master/googletest/docs/primer.md
[3]: https://github.com/google/benchmark
