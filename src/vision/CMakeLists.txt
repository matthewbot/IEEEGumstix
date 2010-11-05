cmake_minimum_required(VERSION 2.6)
project(IEEEPathVision)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

file(GLOB sources "*.cpp")
list(REMOVE_ITEM sources main.cpp)

add_library(ieeepathvision ${sources})

add_executable(ieeepathvision_test main.cpp)
target_link_libraries(ieeepathvision_test ieeepathvision ${OpenCV_LIBS})

