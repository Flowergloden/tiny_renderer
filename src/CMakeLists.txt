cmake_minimum_required(VERSION 3.29...3.30)
project(tiny_renderer)

set(CMAKE_CXX_STANDARD 20)

add_library(image
        SHARED
        Image.h
        Image.cpp)