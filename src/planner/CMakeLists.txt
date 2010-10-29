cmake_minimum_required(VERSION 2.6)
project(IEEEPathPlanner)

file(GLOB sources "*.cpp")

add_library(ieeepathplanner ${sources})
target_link_libraries(ieeepathplanner ieeepathshared)

