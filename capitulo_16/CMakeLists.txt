cmake_minimum_required(VERSION 3.10)
project(capitulo_16 VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(filestorage filestorage.cpp)
target_link_libraries(filestorage ${OpenCV_LIBS})

add_executable(dft dft.cpp)
target_link_libraries(dft ${OpenCV_LIBS})
