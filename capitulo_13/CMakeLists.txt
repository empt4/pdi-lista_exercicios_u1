cmake_minimum_required(VERSION 3.10)
project(registro_interativo VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(registro_interativo registro_interativo.cpp)
target_link_libraries(registro_interativo ${OpenCV_LIBS})