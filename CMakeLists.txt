cmake_minimum_required(VERSION 2.6)
project(PathSim)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE sources "src/*.cpp")
add_executable(pathsim ${sources})
target_link_libraries(pathsim ${wxWidgets_LIBRARIES})

