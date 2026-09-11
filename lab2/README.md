# Lab 2 - Inheritance and Serialization

This lab implements an abstract `Person` interface with concrete `Student` and
`Instructor` classes. Objects serialize to JSON, YAML, and escaped CSV, and
invalid input raises `ValidationError`.

The project also includes JSON round-trip factories (`Student::from_json` and
`Instructor::from_json`) and tests for validation and deserialization errors.

## Build and test in Codespaces

```bash
cd lab2
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/app
```

The first CMake configuration downloads nlohmann/json, yaml-cpp, and Catch2
with `FetchContent`.
