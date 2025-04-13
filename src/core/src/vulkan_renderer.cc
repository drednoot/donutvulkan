#include "core/vulkan_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>
#include <expected>
#include <memory>
#include <thread>

#include "core/result.h"

#include "physical_device.h"
#include "vulkan_renderer_impl.h"

namespace core {

std::expected<VulkanRenderer*, Result> VulkanRenderer::New(
    const VulkanRendererConfig& config) {
  std::unique_ptr<VulkanRenderer> rend(new VulkanRenderer);

  rend->d->instance_ = TRY(rend->d->NewVkInstance());
  rend->d->surface_ = TRY(rend->d->NewSurface());
  rend->d->physical_device_.reset(
      TRY(PhysicalDevice::New(rend->d->instance_, rend->d->surface_)));
  rend->d->device_ = TRY(rend->d->NewLogicalDevice());
  vkGetDeviceQueue(rend->d->device_,
                   rend->d->physical_device_->GetQueueFamilies().graphics, 0,
                   &rend->d->graphics_queue_);
  vkGetDeviceQueue(rend->d->device_,
                   rend->d->physical_device_->GetQueueFamilies().present, 0,
                   &rend->d->present_queue_);
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(config.width, config.height,
                                        "Vulkan window", nullptr, nullptr);

  if (!window) {
    return std::unexpected(Result(CoreError::kCouldNotInitializeGlfwWindow));
  }
  rend->d->window_ = window;

  rend->d->cfg_ = config;
  rend->d->buffer_.resize(config.width * config.height);
  rend->d->z_buffer_.resize(config.width * config.height);
  rend->d->screen_ratio_ = config.width / (double)config.height;
  rend->d->target_ns_ =
      std::chrono::nanoseconds(std::chrono::seconds(1)) / config.target_fps;

  return rend.release();
}

VulkanRenderer::VulkanRenderer() : d(std::make_unique<Impl>()) {}

VulkanRenderer::~VulkanRenderer() {
  if (d->window_) {
    glfwDestroyWindow(d->window_);
  }

  if (d->instance_ && d->surface_) {
    vkDestroySurfaceKHR(d->instance_, d->surface_, nullptr);
  }

  if (d->instance_) {
    vkDestroyInstance(d->instance_, nullptr);
  }

  if (d->device_) {
    vkDestroyDevice(d->device_, nullptr);
  }

  glfwTerminate();
}

void VulkanRenderer::Start() {
  std::chrono::nanoseconds frame_time = d->target_ns_;
  // double delta = NsToSeconds(target_ns_);
  d->start_time_ = std::chrono::system_clock::now().time_since_epoch();

  while (!glfwWindowShouldClose(d->window_)) {
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

    if (frame_time < d->target_ns_) {
      std::this_thread::sleep_for(d->target_ns_ - frame_time);
    }
  }
}

void VulkanRenderer::Clear() {
  for (int i = 0; i < d->cfg_.width * d->cfg_.height; ++i) {
    d->buffer_[i] = ' ';
    d->z_buffer_[i] = 0.0;
  }
}

void VulkanRenderer::Put(int x, int y, char sym) {
  d->buffer_[Xy(x, y)] = sym;
}

void VulkanRenderer::Put(const core::Vec3& point, char sym) {
  int x_denorm = (int)(point.x * d->cfg_.width);
  if (x_denorm < 0 || x_denorm > Right()) {
    return;
  }
  int y_denorm = (int)(point.y * d->cfg_.height);
  if (y_denorm < 0 || y_denorm > Bot()) {
    return;
  }

  if (d->z_buffer_[Xy(x_denorm, y_denorm)] < point.z) {
    d->z_buffer_[Xy(x_denorm, y_denorm)] = point.z;
    d->buffer_[Xy(x_denorm, y_denorm)] = sym;
  }
}

int VulkanRenderer::GetWidth() const {
  return d->cfg_.width;
}

int VulkanRenderer::GetHeight() const {
  return d->cfg_.height;
}

double VulkanRenderer::GetRatio() const {
  return d->screen_ratio_;
}

int VulkanRenderer::Xy(int x, int y) const {
  return y * d->cfg_.width + x;
}

int VulkanRenderer::Right() const {
  return d->cfg_.width - 1;
}

int VulkanRenderer::Bot() const {
  return d->cfg_.height - 1;
}

double VulkanRenderer::ToAspect(double val) const {
  return val * GetRatio();
}

double VulkanRenderer::NsToSeconds(std::chrono::nanoseconds ns) const {
  const long count =
      std::chrono::duration_cast<std::chrono::milliseconds, long>(ns).count();
  return count / 1000.0;
}

double VulkanRenderer::TimeLived() const {
  std::chrono::nanoseconds now =
      std::chrono::system_clock::now().time_since_epoch();
  std::chrono::nanoseconds from_start = now - d->start_time_;
  return NsToSeconds(from_start);
}

}  // namespace core
