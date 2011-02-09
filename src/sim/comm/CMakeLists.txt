cmake_minimum_required(VERSION 2.6)
project(IEEEComm)

file(GLOB sources "*.cpp")
add_executable(ieeecomm ${sources})
target_link_libraries(ieeecomm ieeeshared ieeesim ieeedrivers ${wxWidgets_LIBRARIES})

install(TARGETS ieeecomm DESTINATION bin)

