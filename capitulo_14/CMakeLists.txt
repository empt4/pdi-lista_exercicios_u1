cmake_minimum_required(VERSION 3.10)
project(capitulo14 VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(convolucao2 convolucao2.cpp)
target_link_libraries(convolucao2 ${OpenCV_LIBS})

add_executable(depthoffield depthoffield.cpp)
target_link_libraries(depthoffield ${OpenCV_LIBS})