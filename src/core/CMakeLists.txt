add_library(core
  SHARED
  src/vulkan_renderer.cc
  src/rotation.cc
  src/result.cc
)

add_subdirectory(tests)

target_include_directories(core
  PUBLIC
  include
)

find_package(Vulkan REQUIRED)
find_package(VulkanUtilityLibraries REQUIRED)
find_package(glfw3 3.3 REQUIRED)

target_link_libraries(core
  PRIVATE
  vulkan
  glfw
)
