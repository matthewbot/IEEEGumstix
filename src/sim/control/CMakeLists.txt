cmake_minimum_required(VERSION 2.6)
project(ControlTest)

file(GLOB sources "*.cpp")
add_executable(controltest ${sources})
target_link_libraries(controltest ieeeshared ieeesim ieeecontrols ieeedrivers ${wxWidgets_LIBRARIES})

install(TARGETS controltest DESTINATION bin)

include(IconLib)
install_desktop_file(ieeecomm)

