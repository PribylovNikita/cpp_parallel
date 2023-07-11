cmake_minimum_required(VERSION 3.23)
project(cpp_parallel)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/parallel_executor)
include_directories(${SOURCE_DIR})

add_executable(multithreading test/main.cpp)
target_link_libraries(multithreading pthread)
