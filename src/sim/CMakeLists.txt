cmake_minimum_required(VERSION 2.6)
project(Sim)

# locate wxwidgets, link it, and put it on the include path
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

# gather all the source files and build an executable
file(GLOB sources "*.cpp")
add_executable(roomsim ${sources})
target_link_libraries(roomsim ieeeshared ieeeplanner ${wxWidgets_LIBRARIES})

