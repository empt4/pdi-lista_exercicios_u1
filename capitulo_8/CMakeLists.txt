cmake_minimum_required(VERSION 3.10)
project(colorrange_pintura VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(colorrange_pintura colorrange_pintura.cpp)
target_link_libraries(colorrange_pintura ${OpenCV_LIBS})