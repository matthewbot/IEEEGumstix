cmake_minimum_required(VERSION 2.6)
project(Planner)

file(GLOB sources "*.cpp")

add_library(ieeeplanner ${sources})
target_link_libraries(ieeeplanner ieeeshared)

