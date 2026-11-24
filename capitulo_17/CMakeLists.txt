cmake_minimum_required(VERSION 3.10)
project(homomorfico VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(homomorfico homomorfico.cpp)
target_link_libraries(homomorfico ${OpenCV_LIBS})
