add_library(core
  SHARED
  src/vulkan_renderer_impl.cc
  src/swap_chain_support_details.cc
  src/vulkan_renderer.cc
  src/queue_families.cc
  src/physical_device.cc
  src/rotation.cc
  src/result.cc
  src/instance.cc
)

add_subdirectory(tests)

target_include_directories(core
  PUBLIC
  include
  PRIVATE
  src
)

find_package(Vulkan REQUIRED)
find_package(VulkanUtilityLibraries REQUIRED)
find_package(glfw3 3.3 REQUIRED)

target_link_libraries(core
  PRIVATE
  vulkan
  glfw
)
