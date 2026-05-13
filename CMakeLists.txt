cmake_minimum_required(VERSION 4.0)

# Variables?

# Setup project name
project(sim-gpu VERSION 0.1 LANGUAGES CXX)

# C++ compiler settings
set (CMAKE_CXX_STANDARD 17 )
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# Add and link executables
add_executable(sim-gpu src/main.cpp)

# Add and link libraries
