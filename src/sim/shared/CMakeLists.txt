cmake_minimum_required(VERSION 2.6)
project(Sim)

file(GLOB sources "*.cpp")

add_library(ieeesim ${sources})
target_link_libraries(ieeesim ieeeshared ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES})

