cmake_minimum_required(VERSION 2.8)

project(ray)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(ray main.cpp )