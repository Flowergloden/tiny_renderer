cmake_minimum_required(VERSION 3.29...3.30)
project(tiny_renderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(unit_test
        UnitTest.cpp)

target_link_libraries(unit_test
        PUBLIC
        image)

target_include_directories(unit_test
        PUBLIC
        ../src)