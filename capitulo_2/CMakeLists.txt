cmake_minimum_required(VERSION 3.10)
project(atividade2 VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Configuração para compilar o regions.cpp
add_executable(regions regions.cpp)
target_link_libraries(regions ${OpenCV_LIBS})

# Configuração para compilar o trocaregioes.cpp
add_executable(trocaregioes trocaregioes.cpp)
target_link_libraries(trocaregioes ${OpenCV_LIBS})