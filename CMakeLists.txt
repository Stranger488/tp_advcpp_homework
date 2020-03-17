cmake_minimum_required(VERSION 3.15)
project(homework)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 14)

add_library(process STATIC process/src/process.cpp)

target_include_directories(process PUBLIC process/include)

add_executable(homework main.cpp)

target_link_libraries(homework process)
