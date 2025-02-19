cmake_minimum_required(VERSION 3.15)
project(quaternion VERSION 1.0 LANGUAGES CXX)

# Set C++ standard and requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure compiler warnings (GCC/Clang compatible)
add_compile_options(-Wall -Wextra -Wpedantic)

# Add library
add_library(quaternion
  STATIC
  quaternion.cpp
)

# Include directories
target_include_directories(quaternion PUBLIC .)

# Optional: Link libraries
target_link_libraries(quaternion PRIVATE m)
