cmake_minimum_required(VERSION 3.16)
project(MemoryGame CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 2.6.1 still says cmake_minimum_required 3.5, which CMake 4 refuses
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# SFML is downloaded and built here by CMake, so it does not need to be
# installed on the computer (same idea as Catch2 in lab 0)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
if(WIN32 AND MINGW)
  # The freetype library that comes packaged inside SFML was built for the old
  # MinGW runtime and does not link with the UCRT MinGW I have, so on Windows
  # build freetype from source as well and tell SFML to use that one.
  set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
  set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
  set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
  set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
  set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
  FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG VER-2-13-2
  )
  FetchContent_MakeAvailable(freetype)
  set(SFML_USE_SYSTEM_DEPS ON CACHE BOOL "" FORCE)
  set(FREETYPE_LIBRARY ${freetype_BINARY_DIR}/libfreetype.a CACHE FILEPATH "" FORCE)
  set(FREETYPE_INCLUDE_DIR_ft2build ${freetype_SOURCE_DIR}/include CACHE PATH "" FORCE)
  set(FREETYPE_INCLUDE_DIR_freetype2 ${freetype_SOURCE_DIR}/include CACHE PATH "" FORCE)
endif()

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)
if(WIN32 AND MINGW)
  add_dependencies(sfml-graphics freetype)   # build freetype before sfml-graphics links it
endif()

# ncurses has to be installed already
# (Ubuntu: sudo apt install libncurses5-dev, MSYS2: pacman -S mingw-w64-ucrt-x86_64-ncurses)
set(CURSES_NEED_NCURSES TRUE)
if(WIN32)
  # on MSYS2 the header is in <compiler dir>/../include/ncurses, which CMake
  # does not look in by itself, so give it a hint
  get_filename_component(COMPILER_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
  find_path(CURSES_INCLUDE_PATH ncurses.h HINTS ${COMPILER_DIR}/../include/ncurses)
  find_library(CURSES_LIBRARY NAMES ncursesw ncurses HINTS ${COMPILER_DIR}/../lib)
endif()
find_package(Curses REQUIRED)

add_executable(game main.cpp)
target_include_directories(game PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(game PRIVATE sfml-graphics sfml-window sfml-system ${CURSES_LIBRARIES})
