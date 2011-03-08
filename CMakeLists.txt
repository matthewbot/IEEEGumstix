cmake_minimum_required(VERSION 2.6)
project(IEEEGumstix)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/misc)

# set our include directory
include_directories(include)

# locate boost and put it on the include path
find_package(Boost COMPONENTS thread date_time filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# handle our config files
set(IEEE_CONFIG_DIR "${CMAKE_INSTALL_PREFIX}/etc/ieee" CACHE PATH "Directory where configuration .info files are kept")
set(IEEE_CONFIG_INSTALL_DEFAULTS FALSE CACHE BOOL "Whether to copy default configuration files into IEEE_CONFIG_DIR on installation or not")
if(IEEE_CONFIG_INSTALL_DEFAULTS)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/misc/config/ DESTINATION ${IEEE_CONFIG_DIR})
endif()

# read all subdirectories
add_subdirectory(src/shared)
add_subdirectory(src/planner)
add_subdirectory(src/controls)
add_subdirectory(src/sim)
add_subdirectory(src/drivers)

