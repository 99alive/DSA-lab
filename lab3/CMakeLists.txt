cmake_minimum_required(VERSION 3.16)
project(RandomlyUnique CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# the Unsorted List ADT from the textbook (array version)
add_library(unsortedList unsorted.cpp ItemType.cpp)
target_include_directories(unsortedList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# the textbook test driver
add_executable(listDriver listDriver.cpp)
target_link_libraries(listDriver PRIVATE unsortedList)

# my program for step 4
add_executable(randomlyUnique randomlyUnique.cpp)
target_link_libraries(randomlyUnique PRIVATE unsortedList)
