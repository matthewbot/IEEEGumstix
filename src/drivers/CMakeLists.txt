cmake_minimum_required(VERSION 2.6)
project(Drivers)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

file(GLOB tests "*test.cpp")
file(GLOB sources "*.cpp")
list(REMOVE_ITEM sources ${tests})

add_library(ieeedrivers ${sources})

add_executable(capturetest capturetest.cpp)
target_link_libraries(capturetest ieeedrivers ieeeshared ${Boost_LIBRARIES} ${OpenCV_LIBS})

add_executable(lasertest lasertest.cpp)
target_link_libraries(lasertest ieeedrivers ieeeshared ${Boost_LIBRARIES} ${OpenCV_LIBS})

install(TARGETS capturetest lasertest DESTINATION bin)

