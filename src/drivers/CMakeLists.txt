cmake_minimum_required(VERSION 2.6)
project(Drivers)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

file(GLOB sources "*.cpp")

add_library(ieeedrivers ${sources})

