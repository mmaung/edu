cmake_minimum_required(VERSION 2.8)
project(combineImagesProj)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(combineImages combineImages.cpp)
target_link_libraries(combineImages ${OpenCV_LIBS})

