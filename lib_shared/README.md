# Sub-project lib_shared

This sub-project functions as an example of a shared library for the CMake template. The main purpose of this
sub-project is to demonstrate how to use CMake features, the C++ source code is a placeholder for having something
to test, analyse and document.

See the CMakeLists.txt files for more.

As for now, this sub-project only compiles if you have a posix compatible environment (e.g. unix, linux).

## Unit tests

Unit tests use [Google Test][1] as the unit testing framework. These tests are mainly for generating test coverage
reports with the CMake template. The unit tests can be found under the `test/unit` directory.

For gtest features and usage, see the [primer][2].

### Execute

```
./lib_shared/test/unit/shared_unit_test
```

[1]: https://github.com/google/googletest
[2]: https://github.com/google/googletest/blob/master/googletest/docs/primer.md
