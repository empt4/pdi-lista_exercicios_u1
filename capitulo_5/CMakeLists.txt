cmake_minimum_required(VERSION 3.10)
project(filestorage_exercicio VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(filestorage_exercicio filestorage_exercicio.cpp)
target_link_libraries(filestorage_exercicio ${OpenCV_LIBS})