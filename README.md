# donutcpp

# requirements to run

 - A linux operating system
 - Videodrivers that support Vulkan

# build requirements

 - CMake
 - A build system (make, ninja, etc.)
 - A C++ compiler (clang++, g++ etc.) supporting:
  - c++23 standard
  - GNU statement expressions
 - git
 - Vulkan
 - glfw 3.3

# build steps

make a build directory, run cmake inside it (substitute `Ninja` with something else if you want):

```
mkdir build
cd build
cmake -G Ninja ..
```

run your build system:

```
ninja
```

run the project:

```
./donutvulkan
```

# testing steps

Build the project, run ctest:

```
mkdir build
cd build
cmake -G Ninja ..
ninja
ctest --output-on-failure
```

Testing is done with [doctest framework](https://github.com/doctest/doctest)

# sources

 - [Khronos Vulkan® Tutorial](https://docs.vulkan.org/tutorial/latest/00_Introduction.html)
