cmake_minimum_required(VERSION 3.10)
project(cannypoints VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(cannypoints cannypoints.cpp)
target_link_libraries(cannypoints ${OpenCV_LIBS})
