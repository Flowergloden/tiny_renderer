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

find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
    target_include_directories(image PUBLIC ${OpenCV_INCLUDE_DIRS})
endif()

add_library(triangle SHARED Triangle.cpp Triangle.h)

if (OpenCV_FOUND)
    target_include_directories(triangle PUBLIC ${OpenCV_INCLUDE_DIRS})
endif()

add_library(model SHARED Model.cpp Model.h)

add_library(camera SHARED Camera.cpp Camera.h)

if (OpenCV_FOUND)
    target_include_directories(camera PUBLIC ${OpenCV_INCLUDE_DIRS})
endif()

if (OpenCV_FOUND)
    # 兼容 Arch Linux pacman 安装的 OpenCV（使用 opencv_core 而非 OpenCV::opencv_core）
    if(TARGET OpenCV::opencv_core)
        target_link_libraries(image PUBLIC OpenCV::opencv_core OpenCV::opencv_imgcodecs)
    else()
        # 只链接必要的库，避免链接 opencv_viz 和 opencv_hdf 等需要额外依赖的库
        target_link_libraries(image PUBLIC opencv_core opencv_imgcodecs)
    endif()
endif ()

find_package(tinyobjloader CONFIG REQUIRED)
if (tinyobjloader_FOUND)
    target_link_libraries(model PUBLIC tinyobjloader::tinyobjloader)
endif ()

target_link_libraries(model PUBLIC triangle)

target_link_libraries(renderer PUBLIC image model camera)
