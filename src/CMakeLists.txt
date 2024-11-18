cmake_minimum_required(VERSION 3.29...3.30)
project(tiny_renderer)

set(CMAKE_CXX_STANDARD 20)

add_library(renderer
        SHARED
        Renderer.h
        Renderer.cpp)

add_library(image
        SHARED
        Image.h
        Image.cpp)
find_package(OpenCV CONFIG REQUIRED)

add_library(triangle SHARED Triangle.cpp Triangle.h)

add_library(model Model.cpp Model.h)

if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})

    link_directories(${OpenCV_LIB_DIR})

    target_link_libraries(image PUBLIC ${OpenCV_LIBS})
endif ()

find_package(tinyobjloader CONFIG REQUIRED)
if (tinyobjloader_FOUND)
    target_link_libraries(model PUBLIC tinyobjloader::tinyobjloader)
endif ()

target_link_libraries(model PUBLIC triangle)

target_link_libraries(renderer PUBLIC image model)
