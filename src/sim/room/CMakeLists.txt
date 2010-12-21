cmake_minimum_required(VERSION 2.6)
project(RoomSim)

# gather all the source files and build an executable
file(GLOB sources "*.cpp")
add_executable(roomsim ${sources})
target_link_libraries(roomsim ieeeshared ieeeplanner ieeesim ${wxWidgets_LIBRARIES})

install(TARGETS roomsim DESTINATION bin)

