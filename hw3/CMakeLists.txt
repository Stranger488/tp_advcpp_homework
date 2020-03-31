cmake_minimum_required(VERSION 3.15)
project(hw3)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 17)

add_library(tcp STATIC tcp/src/Connection.cpp tcp/src/Server.cpp)

target_include_directories(tcp PUBLIC tcp/include)

add_executable(hw3_client src/client.cpp)
add_executable(hw3_server src/server.cpp)

target_link_libraries(hw3_client tcp)
target_link_libraries(hw3_server tcp)
