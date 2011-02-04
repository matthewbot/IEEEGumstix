cmake_minimum_required(VERSION 2.6)
project(Drivers)

file(GLOB sources "*.cpp")
file(GLOB tests "*test.cpp")
list(REMOVE_ITEM sources ${tests})

add_library(ieeedrivers ${sources})

add_executable(serialtest serialtest.cpp)
target_link_libraries(serialtest ieeedrivers ${Boost_LIBRARIES})
install(TARGETS serialtest DESTINATION bin)

