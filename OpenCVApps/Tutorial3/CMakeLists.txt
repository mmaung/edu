cmake_minimum_required(VERSION 2.8)
project(helloWorldProj)
find_package(OpenCV REQUIRED)
add_executable(helloWorld helloWorld.cpp)  # the name must be the same as in target_link_libraries
target_link_libraries(helloWorld ${OpenCV_LIBS})
