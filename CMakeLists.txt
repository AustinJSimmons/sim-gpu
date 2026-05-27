cmake_minimum_required(VERSION 4.0)

# Setup project name
project(sim-gpu VERSION 0.1 LANGUAGES CXX)

# C++ compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTBENCH "Build testbench suite" OFF)

# Create components library for linking
add_library(component_lib STATIC
  src/ALU.cpp
  src/FPU.cpp
)

# Add and link SystemC
find_package(PkgConfig REQUIRED)
pkg_check_modules(SystemC REQUIRED systemc)

# Add include dir for .h files in proj
target_include_directories(component_lib PUBLIC
  include
  ${SystemC_INCLUDE_DIRS}
)

# Add and link executables
add_executable(sim-gpu
  src/main.cpp
)

# Target link libraries ?
target_link_libraries(sim-gpu PRIVATE component_lib)

target_link_libraries(component_lib PUBLIC ${SystemC_LIBRARIES})
target_link_directories(component_lib PUBLIC ${SystemC_LIBRARY_DIRS})

if(BUILD_TESTBENCH)
  enable_testing()
  add_subdirectory(test)
endif()


