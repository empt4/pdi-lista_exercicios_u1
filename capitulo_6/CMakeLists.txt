cmake_minimum_required(VERSION 3.10)
project(capitulo6 VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(colormaps_padrao colormaps_padrao.cpp)
target_link_libraries(colormaps_padrao ${OpenCV_LIBS})

add_executable(colormap_personalizado colormap_personalizado.cpp)
target_link_libraries(colormap_personalizado ${OpenCV_LIBS})