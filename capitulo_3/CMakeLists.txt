cmake_minimum_required(VERSION 3.10)
project(capitulo3 VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(video_cinza video_cinza.cpp)
target_link_libraries(video_cinza ${OpenCV_LIBS})

add_executable(video_negativo video_negativo.cpp)
target_link_libraries(video_negativo ${OpenCV_LIBS})