cmake_minimum_required(VERSION 3.6)
project(hello_world)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    host/main.c
    ta/include/hello_world_ta.h
    ta/hello_world_ta.c
    ta/user_ta_header_defines.h)

add_executable(hello_world ${SOURCE_FILES})