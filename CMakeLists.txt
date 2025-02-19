cmake_minimum_required(VERSION 3.15)
project(donutcpp VERSION 1.0 LANGUAGES CXX)

# Set C++ standard and requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure compiler warnings (GCC/Clang compatible)
add_compile_options(-Wall -Wextra -Wpedantic)

# Add executable
add_executable(donutcpp
    main.cpp
    renderer.cpp
)

# Include directories
target_include_directories(donutcpp PRIVATE .)

# Optional: Link libraries
target_link_libraries(donutcpp PRIVATE m)
