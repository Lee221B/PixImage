cmake_minimum_required(VERSION 3.8)
project(PixImage)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp PixImage.h)

add_executable(PixImage ${SOURCE_FILES})
target_link_libraries(PixImage ${OpenCV_LIBS})