cmake_minimum_required(VERSION 3.15)
project(hw5)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 17)

set(LOG_LIB_SRCS shmem/src/ShmemAlloc.cpp shmem/src/Semaphore.cpp shmem/src/Map.cpp)
set(SRCS src/main.cpp)

add_library(shmem STATIC ${LOG_LIB_SRCS})

target_include_directories(shmem PUBLIC shmem/include)
find_package(Threads)

add_executable(hw5 ${SRCS})
target_link_libraries(hw5 shmem ${CMAKE_THREAD_LIBS_INIT})
