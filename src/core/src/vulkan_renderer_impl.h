#ifndef DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_
#define DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>
#include <expected>
#include <memory>
#include <vector>

#include "core/vulkan_renderer.h"

#include "physical_device.h"

namespace core {

struct VulkanRenderer::Impl {
  // vulkan stuff
  std::expected<VkInstance, VkResult> NewVkInstance();
#ifndef NDEBUG
  static std::vector<const char*> GetAvailableValidationLayers();
#endif
  std::expected<VkSurfaceKHR, VkResult> NewSurface();
  std::expected<VkDevice, VkResult> NewLogicalDevice();

  void DrawBuffer() const;

  void MoveCursorTo(int x, int y) const;

  VulkanRendererConfig cfg_ = {};

  GLFWwindow* window_ = nullptr;
  VkInstance instance_ = VK_NULL_HANDLE;
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
  std::unique_ptr<PhysicalDevice> physical_device_;
  VkDevice device_ = VK_NULL_HANDLE;
  VkQueue graphics_queue_ = VK_NULL_HANDLE;
  VkQueue present_queue_ = VK_NULL_HANDLE;

#ifndef NDEBUG
  bool is_validation_layers_enabled_ = false;
#endif

  std::vector<char> buffer_;
  std::vector<double> z_buffer_;
  double screen_ratio_ = 0.0;

  std::chrono::nanoseconds target_ns_;
  std::chrono::nanoseconds start_time_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_
