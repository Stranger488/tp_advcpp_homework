cmake_minimum_required(VERSION 3.15)
project(hw4)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 17)

set(LOG_LIB_SRCS tcp_epoll/src/Connection.cpp tcp_epoll/src/Server.cpp)

set(SRCS src/server.cpp)

add_library(tcp_epoll STATIC ${LOG_LIB_SRCS})

target_include_directories(tcp_epoll PUBLIC tcp_epoll/include)

add_executable(hw4 ${SRCS})
target_link_libraries(hw4 tcp_epoll)
