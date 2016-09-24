cmake_minimum_required(VERSION 2.8)
project(disparityMapProj)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} /usr/include/libusb-1.0)
add_executable(disparityMap disparityMap.cpp)
target_link_libraries(disparityMap ${OpenCV_LIBS} usb-1.0)

