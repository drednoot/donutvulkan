#include "core/vulkan_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <expected>
#include <iostream>
#include <memory>
#include <thread>

namespace core {

std::expected<VulkanRenderer*, Result> VulkanRenderer::New(
    const VulkanRendererConfig& config) {
  std::unique_ptr<VulkanRenderer> rend(new VulkanRenderer);

  rend->instance_ = TRY(CreateVkInstance());

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(config.width, config.height,
                                        "Vulkan window", nullptr, nullptr);

  if (!window) {
    return std::unexpected(Result(CoreError::kCouldNotInitializeGlfwWindow));
  }
  rend->window_ = window;

  rend->cfg_ = config;
  rend->buffer_.resize(config.width * config.height);
  rend->z_buffer_.resize(config.width * config.height);
  rend->screen_ratio_ = config.width / (double)config.height;
  rend->target_ns_ =
      std::chrono::nanoseconds(std::chrono::seconds(1)) / config.target_fps;

  return rend.release();
}

VulkanRenderer::~VulkanRenderer() {
  if (window_) {
    glfwDestroyWindow(window_);
  }

  if (instance_) {
    vkDestroyInstance(instance_, nullptr);
  }

  glfwTerminate();
}

void VulkanRenderer::Start() {
  std::chrono::nanoseconds frame_time = target_ns_;
  // double delta = NsToSeconds(target_ns_);
  start_time_ = std::chrono::system_clock::now().time_since_epoch();

  while (!glfwWindowShouldClose(window_)) {
    std::chrono::time_point before_render =
        std::chrono::high_resolution_clock::now();
    // Clear();
    // Render(delta);
    glfwPollEvents();
    // DrawBuffer();
    std::chrono::time_point after_render =
        std::chrono::high_resolution_clock::now();
    frame_time = after_render - before_render;
    // delta = frame_time > target_ns_ ? NsToSeconds(frame_time)
    //                                 : NsToSeconds(target_ns_);

    if (frame_time < target_ns_) {
      std::this_thread::sleep_for(target_ns_ - frame_time);
    }
  }
}

void VulkanRenderer::Clear() {
  for (int i = 0; i < cfg_.width * cfg_.height; ++i) {
    buffer_[i] = ' ';
    z_buffer_[i] = 0.0;
  }
}

void VulkanRenderer::Put(const core::Vec3& point, char sym) {
  int x_denorm = (int)(point.x * cfg_.width);
  if (x_denorm < 0 || x_denorm > Right()) {
    return;
  }
  int y_denorm = (int)(point.y * cfg_.height);
  if (y_denorm < 0 || y_denorm > Bot()) {
    return;
  }

  if (z_buffer_[Xy(x_denorm, y_denorm)] < point.z) {
    z_buffer_[Xy(x_denorm, y_denorm)] = point.z;
    buffer_[Xy(x_denorm, y_denorm)] = sym;
  }
}

std::expected<VkInstance, VkResult> VulkanRenderer::CreateVkInstance() {
  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "donutvulkan";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "donut";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = nullptr;

  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  create_info.enabledExtensionCount = glfw_extension_count;
  create_info.ppEnabledExtensionNames = glfw_extensions;

  create_info.enabledLayerCount = 0;

  VkInstance instance;
  VkResult result = vkCreateInstance(&create_info, nullptr, &instance);

  if (result != VK_SUCCESS) {
    return std::unexpected(result);
  }

  return instance;
}

void VulkanRenderer::DrawBuffer() const {
  MoveCursorTo(0, 0);
  std::cout.write(buffer_.data(), buffer_.size());
  std::cout.flush();
}

void VulkanRenderer::MoveCursorTo(int x, int y) const {
  printf("\033[%d;%dH", y, x);
}

}  // namespace core
