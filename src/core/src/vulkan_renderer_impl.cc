#include "vulkan_renderer_impl.h"
#include "core/vulkan_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <expected>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "logical_device.h"
#include "physical_device.h"
#include "swap_chain.h"

namespace core {

Result VulkanRenderer::Impl::New(const VulkanRendererConfig& config) {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(config.width, config.height,
                                        "Vulkan window", nullptr, nullptr);

  if (!window) {
    return kCouldNotInitializeGlfwWindow;
  }
  window_ = window;

  instance_.reset(UNWRAP_ERR(Instance::New(window_)));
  physical_device_.reset(UNWRAP_ERR(PhysicalDevice::New(*instance_)));
  device_.reset(UNWRAP_ERR(LogicalDevice::New(*physical_device_)));
  swap_chain_.reset(UNWRAP_ERR(SwapChain::New(*device_, window_)));
  TRY_RS_ERR(CreateGraphicsPipeline());

  cfg_ = config;
  buffer_.resize(config.width * config.height);
  z_buffer_.resize(config.width * config.height);
  screen_ratio_ = config.width / (double)config.height;
  target_ns_ =
      std::chrono::nanoseconds(std::chrono::seconds(1)) / config.target_fps;

  return Result();
}

VulkanRenderer::Impl::~Impl() {
  swap_chain_.reset();
  device_.reset();
  instance_.reset();

  if (window_) {
    glfwDestroyWindow(window_);
  }

  glfwTerminate();
}

std::expected<std::vector<char>, Result> VulkanRenderer::Impl::ReadFile(
    const char* filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    return std::unexpected(FileError{filename});
  }

  size_t file_size = (size_t)file.tellg();
  std::vector<char> buf(file_size);

  file.seekg(0);
  file.read(buf.data(), file_size);

  return buf;
}

Result VulkanRenderer::Impl::CreateGraphicsPipeline() const {
  const std::vector<char> vert_shader =
      UNWRAP_ERR(ReadFile("shaders/shader.vert.spv"));
  const std::vector<char> frag_shader =
      UNWRAP_ERR(ReadFile("shaders/shader.frag.spv"));

  std::cout << vert_shader.size();
  std::cout << frag_shader.size();

  return Result();
}

void VulkanRenderer::Impl::Start() {
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

void VulkanRenderer::Impl::DrawBuffer() const {
  MoveCursorTo(0, 0);
  std::cout.write(buffer_.data(), buffer_.size());
  std::cout.flush();
}

void VulkanRenderer::Impl::MoveCursorTo(int x, int y) const {
  printf("\033[%d;%dH", y, x);
}

}  // namespace core
