cmake_minimum_required(VERSION 2.6)
project(IEEEPathVision)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

file(GLOB sources "*.cpp")
list(REMOVE_ITEM sources capturetest.cpp)

add_library(ieeepathvision ${sources})

add_executable(ieeepathdrivers_capturetest capturetest.cpp)
target_link_libraries(ieeepathdrivers_capturetest ieeepathvision ${OpenCV_LIBS})

