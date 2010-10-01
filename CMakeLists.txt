cmake_minimum_required(VERSION 2.6)
project(PathSim)

# gather all the source files and build an executable
file(GLOB_RECURSE sources "src/*.cpp")
add_executable(pathsim ${sources})

# Put each source folder on the include path so code can find headers without specifying relative paths
file(GLOB folders "src/*")
foreach(folder ${folders})
	if(IS_DIRECTORY ${folder})
		include_directories(${folder})
	endif()
endforeach()

# locate wxwidgets, link it, and put it on the include path
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(pathsim ${wxWidgets_LIBRARIES})

# locate boost and put it on the include path
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

