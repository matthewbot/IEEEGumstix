cmake_minimum_required(VERSION 2.6)
project(LaserSim)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

file(GLOB sources "*.cpp")
add_executable(lasersim ${sources})
target_link_libraries(lasersim ieeeshared ieeesim ieeedrivers ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})

install(TARGETS lasersim DESTINATION bin)

