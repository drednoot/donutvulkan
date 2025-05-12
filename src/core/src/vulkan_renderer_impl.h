#ifndef DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_
#define DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>
#include <expected>
#include <memory>
#include <vector>

#include "core/vulkan_renderer.h"

#include "instance.h"
#include "logical_device.h"
#include "physical_device.h"
#include "swap_chain.h"

namespace core {

struct VulkanRenderer::Impl {
  Result New(const VulkanRendererConfig& config);
  Impl() = default;
  Impl(const Impl&) = delete;
  Impl(Impl&&) = delete;
  Impl& operator=(const Impl&) = delete;
  Impl& operator=(Impl&&) = delete;
  ~Impl();

  // vulkan stuff
  static std::expected<std::vector<char>, Result> ReadFile(
      const char* filename);
  Result CreateGraphicsPipeline() const;

  // other stuff
  void Start();

  void DrawBuffer() const;

  void MoveCursorTo(int x, int y) const;

  VulkanRendererConfig cfg_ = {};

  // vulkan members
  GLFWwindow* window_ = nullptr;
  std::unique_ptr<Instance> instance_;
  std::unique_ptr<PhysicalDevice> physical_device_;
  std::unique_ptr<LogicalDevice> device_;
  std::unique_ptr<SwapChain> swap_chain_;

  // other members
  std::vector<char> buffer_;
  std::vector<double> z_buffer_;
  double screen_ratio_ = 0.0;

  std::chrono::nanoseconds target_ns_;
  std::chrono::nanoseconds start_time_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_VULKAN_RENDERER_IMPL_H_
