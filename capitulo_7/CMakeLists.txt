cmake_minimum_required(VERSION 3.10)
project(capitulo7 VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(colorspace_mapas colorspace_mapas.cpp)
target_link_libraries(colorspace_mapas ${OpenCV_LIBS})

add_executable(colorspace_personalizado colorspace_personalizado.cpp)
target_link_libraries(colorspace_personalizado ${OpenCV_LIBS})