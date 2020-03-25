cmake_minimum_required(VERSION 3.15)
project(hw2)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 17)

add_library(log STATIC src/)

target_include_directories(log PUBLIC include)

add_executable(hw2 src/main.cpp)
target_link_libraries(hw2 log)
