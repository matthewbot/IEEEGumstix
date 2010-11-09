cmake_minimum_required(VERSION 2.6)
project(IEEEPathVision)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

file(GLOB tests "*test.cpp")
file(GLOB sources "*.cpp")
list(REMOVE_ITEM sources ${tests})

add_library(ieeepathvision ${sources})

add_executable(ieeepathdrivers_capturetest capturetest.cpp)
target_link_libraries(ieeepathdrivers_capturetest ieeepathvision ${Boost_LIBRARIES} ${OpenCV_LIBS})

add_executable(ieeepathdrivers_mousetest mousetest.cpp)
target_link_libraries(ieeepathdrivers_mousetest ieeepathvision ${Boost_LIBRARIES})

