cmake_minimum_required(VERSION 3.10)
project(light_painting VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(light_painting light_painting.cpp)
target_link_libraries(light_painting ${OpenCV_LIBS})