cmake_minimum_required(VERSION 3.30)
project(tiny_renderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(tiny_renderer
        main.cpp)

target_include_directories(tiny_renderer
        PUBLIC
        ../src)