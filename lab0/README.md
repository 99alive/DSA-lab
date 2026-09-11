# Lab 0 — MathLibProject

A small C++ project that builds a reusable math library, a demo application, and
a unit-test suite. It exists to exercise the toolchain rather than the maths:
CMake for configuration, a static library with a public header, Catch2 v3 for
testing, CTest for test discovery, and GitHub Actions for continuous integration.

## Project layout

```
lab0/
├── CMakeLists.txt              Build configuration
├── include/
│   └── math_functions.h        Public header (the library's API)
├── src/
│   ├── math_functions.cpp      Library implementation
│   └── main.cpp                Demo application
├── tests/
│   └── math_tests.cpp          Catch2 unit tests
└── .github/workflows/cmake.yml CI: build + test on Linux, Windows, macOS
```

Three build targets are produced:

| Target       | Kind           | Description                                  |
|--------------|----------------|----------------------------------------------|
| `MathLib`    | static library | The math functions, reusable by other code   |
| `MathApp`    | executable     | Demo showing both success and error paths    |
| `unit_tests` | executable     | Catch2 test suite, registered with CTest     |

## The API

```cpp
double compute_square_root(double number);
```

Returns the square root of `number`. Throws `std::domain_error` if `number` is
negative.

Error handling uses an exception rather than a sentinel return value such as
`-1`. A sentinel would be ambiguous — the caller cannot distinguish a genuine
result from an error code without checking every call — and it is easy to ignore
by accident. An exception cannot be silently discarded, and it carries a
descriptive message with it.

## Building

Requires CMake 3.16 or newer and a C++ compiler. Catch2 is downloaded
automatically by CMake's `FetchContent`, so an internet connection is needed for
the first configure.

```bash
cmake -S . -B build
cmake --build build
```

## Running the tests

```bash
cd build
ctest --output-on-failure
```

Expected output:

```
    Start 1: Square root of positive numbers
1/3 Test #1: Square root of positive numbers .....   Passed
    Start 2: Square root of negative numbers
2/3 Test #2: Square root of negative numbers .....   Passed
    Start 3: Square root of non-perfect square
3/3 Test #3: Square root of non-perfect square ...   Passed

100% tests passed, 0 tests failed out of 3
```

The tests can also be run directly, which gives more detail on failure:

```bash
./unit_tests
```

## Running the demo

```bash
./build/MathApp
```

```
Square root of 25 is 5
Error for -1: compute_square_root: negative input has no real square root
```

## Test coverage

| Test case                     | What it checks                                      |
|-------------------------------|-----------------------------------------------------|
| Square root of positive numbers | `sqrt(25) == 5`, and the `sqrt(0) == 0` boundary  |
| Square root of negative numbers | Negative input throws, and throws `std::domain_error` specifically |
| Square root of non-perfect square | `sqrt(2)` is correct to within a tolerance        |

Exact equality (`== 5.0`) is safe for the perfect squares because IEEE-754
requires `sqrt` to be correctly rounded, so those results are exact. The
irrational case uses `Catch::Approx` with a tolerance instead, since `sqrt(2)`
cannot be represented exactly in binary floating point.

## Installing and packaging

The build also defines install and package rules:

```bash
cmake --install build --prefix /some/where   # installs binary, library, headers
cd build && cpack                            # produces a distributable archive
```

## Continuous integration

`.github/workflows/cmake.yml` configures, builds, and tests the project on
Ubuntu, Windows, and macOS on every push and pull request to `main`.
