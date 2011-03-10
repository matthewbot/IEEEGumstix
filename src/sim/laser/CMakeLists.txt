cmake_minimum_required(VERSION 2.6)
project(LaserSim)

file(GLOB sources "*.cpp")
add_executable(lasersim ${sources})
target_link_libraries(lasersim ieeeshared ieeesim ieeedrivers ieeecontrols ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS lasersim DESTINATION bin)

include(IconLib)
install_desktop_file(lasersim)

