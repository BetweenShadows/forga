cmake_minimum_required(VERSION 3.5)
project(forga)

set(CMAKE_CXX_STANDARD 17)

add_executable(forga src/main.cpp src/filter.hpp src/filter.cpp)