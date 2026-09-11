cmake_minimum_required(VERSION 3.16)  # Minimum CMake version

project(MathLibProject VERSION 1.0 LANGUAGES CXX)  # Project name and version

set(CMAKE_CXX_STANDARD 11)  # Use C++11
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build the math functions as a reusable static library
add_library(MathLib STATIC src/math_functions.cpp)

# PUBLIC so anything linking MathLib also gets the headers
target_include_directories(MathLib PUBLIC include)

# Add the executable
add_executable(MathApp src/main.cpp)
target_link_libraries(MathApp PRIVATE MathLib)

# Enable testing
enable_testing()

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

# Add test executable
add_executable(unit_tests tests/math_tests.cpp)
target_link_libraries(unit_tests PRIVATE MathLib Catch2::Catch2WithMain)

# Discover and add tests to CTest
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)

# Install the app, the library and the public headers
include(GNUInstallDirs)
install(TARGETS MathApp MathLib
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Package the project (build the "package" target, or run cpack)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A small CMake + Catch2 math library lab")
include(CPack)
