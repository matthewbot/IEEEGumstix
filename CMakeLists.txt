cmake_minimum_required(VERSION 2.6)
project(IEEEGumstix)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/misc)

# set our include directory
include_directories(include)

# locate boost and put it on the include path
find_package(Boost COMPONENTS thread date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# read all subdirectories
add_subdirectory(src/shared)
add_subdirectory(src/planner)
add_subdirectory(src/sim)
add_subdirectory(src/drivers)

