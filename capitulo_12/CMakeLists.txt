cmake_minimum_required(VERSION 3.10)
project(capitulo12 VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(equalize equalize.cpp)
target_link_libraries(equalize ${OpenCV_LIBS})

add_executable(motiondetector motiondetector.cpp)
target_link_libraries(motiondetector ${OpenCV_LIBS})