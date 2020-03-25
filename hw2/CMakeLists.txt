cmake_minimum_required(VERSION 3.15)
project(hw2)

add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 17)

set(LOG_LIB_SRCS src/BaseLogger.cpp src/FileLogger.cpp src/Global.cpp src/Logger.cpp
        src/StderrLogger.cpp src/StdoutLogger.cpp)

set(SRCS src/main.cpp)

add_library(log STATIC ${LOG_LIB_SRCS})

target_include_directories(log PUBLIC include)

add_executable(hw2 ${SRCS})
target_link_libraries(hw2 log)
