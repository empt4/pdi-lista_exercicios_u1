cmake_minimum_required(VERSION 3.10)
project(labeling_aprimorado VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(labeling_aprimorado labeling_aprimorado.cpp)
target_link_libraries(labeling_aprimorado ${OpenCV_LIBS})