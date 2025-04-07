cmake_minimum_required(VERSION 3.5)
project(donutvulkan VERSION 1.0 LANGUAGES CXX)

# Set C++ standard and requirements
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure compiler warnings (GCC/Clang compatible)
add_compile_options(-Wall -Wextra -Wpedantic -Wswitch-enum -Wno-gnu-statement-expression)

# tests
include(cmake/doctest.cmake)
enable_testing()

# Add executable
add_executable(donutvulkan
    src/app/main.cc
    src/app/renderer.cc
    src/app/cube.cc
    src/app/donut.cc
)

# Include directories
target_include_directories(donutvulkan PRIVATE src)

add_subdirectory(src/core)
add_dependencies(donutvulkan core)

target_link_libraries(donutvulkan
  PRIVATE
  core
  m
)
