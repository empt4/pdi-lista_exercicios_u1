cmake_minimum_required(VERSION 3.10)
project(visualizacao VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(visualizacao visualizacao.cpp)
target_link_libraries(visualizacao ${OpenCV_LIBS})