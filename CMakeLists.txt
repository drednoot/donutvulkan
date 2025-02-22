cmake_minimum_required(VERSION 3.15)
project(donutcpp VERSION 1.0 LANGUAGES CXX)

# Set C++ standard and requirements
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure compiler warnings (GCC/Clang compatible)
add_compile_options(-Wall -Wextra -Wpedantic)

# tests
include(cmake/doctest.cmake)
enable_testing()

# Add executable
add_executable(donutcpp
    src/app/main.cc
    src/app/renderer.cc
    src/app/cube.cc
    src/app/donut.cc
)

# Include directories
target_include_directories(donutcpp PRIVATE src)

add_subdirectory(src/core)
add_dependencies(donutcpp core)

target_link_libraries(donutcpp
  PRIVATE
  core
  m
)
