cmake_minimum_required(VERSION 2.6)
project(Sim)

# locate wxwidgets, link it, and put it on the include path
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

add_subdirectory(room)
add_subdirectory(laser)
add_subdirectory(comm)

add_subdirectory(shared)

