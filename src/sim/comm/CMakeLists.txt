cmake_minimum_required(VERSION 2.6)
project(IEEEComm)

file(GLOB sources "*.cpp")
add_executable(ieeecomm ${sources})
target_link_libraries(ieeecomm ieeeshared ieeesim ieeedrivers ieeecontrols ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS ieeecomm DESTINATION bin)

include(IconLib)
install_desktop_file(ieeecomm)

