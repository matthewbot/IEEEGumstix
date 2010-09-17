cmake_minimum_required (VERSION 2.6)
project (RoomSim)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include( ${wxWidgets_USE_FILE} )

find_package(Boost)
include( ${Boost_INCLUDE_DIRS} )

file (GLOB_RECURSE sources "src/*.cpp")
add_executable (roomsim ${sources})
target_link_libraries(roomsim ${wxWidgets_LIBRARIES})

