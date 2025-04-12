#ifndef DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_
#define DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>
#include <expected>
#include <vector>

#include "core/vulkan_renderer.h"

#include "queue_families.h"

namespace core {

struct VulkanRenderer::Impl {
  // vulkan stuff
  std::expected<VkInstance, VkResult> CreateVkInstance();
#ifndef NDEBUG
  static std::vector<const char*> GetAvailableValidationLayers();
#endif
  std::expected<VkPhysicalDevice, Result> GetSuitablePhysicalDevice();
  static bool IsDeviceSuitable(VkPhysicalDevice device);
  static QueueFamilies GetQueueFamilies(VkPhysicalDevice device);

  void DrawBuffer() const;

  void MoveCursorTo(int x, int y) const;

  VulkanRendererConfig cfg_;

  GLFWwindow* window_;
  VkInstance instance_;
  VkPhysicalDevice device_;

#ifndef NDEBUG
  bool is_validation_layers_enabled_;
#endif

  std::vector<char> buffer_;
  std::vector<double> z_buffer_;
  double screen_ratio_;

  std::chrono::nanoseconds target_ns_;
  std::chrono::nanoseconds start_time_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_
