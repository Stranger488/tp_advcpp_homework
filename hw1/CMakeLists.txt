cmake_minimum_required(VERSION 3.15)
project(hw1)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 14)

add_library(process STATIC src/process.cpp)

target_include_directories(process PUBLIC include)

add_executable(hw1 src/main.cpp)
target_link_libraries(hw1 process)

add_executable(test_hw1 tests/test.cpp)
