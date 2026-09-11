cmake_minimum_required(VERSION 3.16)

project(ShapeMaker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ShapeMaker shapemaker.cpp)
target_include_directories(ShapeMaker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})