cmake_minimum_required(VERSION 3.15)
project(homework)

set(CMAKE_CXX_STANDARD 14)

add_executable(homework main.cpp include/process.h src/process.cpp)
