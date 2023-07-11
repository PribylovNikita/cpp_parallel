cmake_minimum_required(VERSION 3.23)
project(multithreading)

set(CMAKE_CXX_STANDARD 17)

add_executable(multithreading main.cpp)
target_link_libraries(multithreading pthread)
