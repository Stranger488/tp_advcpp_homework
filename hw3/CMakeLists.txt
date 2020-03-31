cmake_minimum_required(VERSION 3.15)
project(hw3)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 17)

add_library(tcp STATIC src/Connection.cpp src/Server.cpp)

target_include_directories(tcp PUBLIC include)

add_executable(hw3 src/main.cpp)
target_link_libraries(hw3 tcp)
