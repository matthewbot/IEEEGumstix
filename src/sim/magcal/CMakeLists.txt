cmake_minimum_required(VERSION 2.6)
project(MagCal)

file(GLOB sources "*.cpp")
add_executable(magcal ${sources})
target_link_libraries(magcal ieeeshared ieeecontrols ieeedrivers ${Boost_LIBRARIES})

install(TARGETS magcal DESTINATION bin)

