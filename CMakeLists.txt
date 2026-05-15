cmake_minimum_required(VERSION 4.0)

# Setup project name
project(sim-gpu VERSION 0.1 LANGUAGES CXX)

# C++ compiler settings
set (CMAKE_CXX_STANDARD 17 )
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add and link executables
add_executable(sim-gpu
  src/main.cpp
  src/compute_unit.cpp
)

# Add include dir for .h files in proj
target_include_directories(sim-gpu PRIVATE include)

# Add and link SystemC
find_package(PkgConfig REQUIRED)

pkg_check_modules(SystemC REQUIRED systemc)

target_include_directories(sim-gpu PRIVATE ${SystemC_INCLUDE_DIRS})
target_link_libraries(sim-gpu PRIVATE ${SystemC_LIBRARIES})
target_link_directories(sim-gpu PRIVATE ${SystemC_LIBRARY_DIRS})
