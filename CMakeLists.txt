cmake_minimum_required(VERSION 2.6)
project(IEEEPath)

include_directories("include")

# gather all the source files and build an executable
file(GLOB_RECURSE sources "src/*.cpp")
add_executable(ieeepathsim ${sources})

# locate wxwidgets, link it, and put it on the include path
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(ieeepathsim ${wxWidgets_LIBRARIES})

# locate boost and put it on the include path
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

