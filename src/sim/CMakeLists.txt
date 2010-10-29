cmake_minimum_required(VERSION 2.6)
project(IEEEPathSim)

# locate wxwidgets, link it, and put it on the include path
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

# gather all the source files and build an executable
file(GLOB sources "*.cpp")
add_executable(ieeepathsim ${sources})
target_link_libraries(ieeepathsim ieeepathshared ieeepathplanner ${wxWidgets_LIBRARIES})

