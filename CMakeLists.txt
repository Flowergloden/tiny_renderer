cmake_minimum_required(VERSION 3.30)
project(tiny_renderer)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(apps)
add_subdirectory(src)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_subdirectory(test)
endif ()