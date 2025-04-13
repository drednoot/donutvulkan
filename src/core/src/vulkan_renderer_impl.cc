#include "vulkan_renderer_impl.h"
#include "core/vulkan_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <expected>
#include <iostream>
#include <memory>
#include <thread>

#include "physical_device.h"

namespace core {

std::expected<VulkanRenderer*, Result> VulkanRenderer::New(
    const VulkanRendererConfig& config) {
  std::unique_ptr<VulkanRenderer> rend(new VulkanRenderer);

  rend->d->instance_ = TRY(rend->d->NewVkInstance());
  rend->d->physical_device_.reset(TRY(PhysicalDevice::New(rend->d->instance_)));
  rend->d->device_ = TRY(rend->d->NewLogicalDevice());

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

  if (d->instance_) {
    vkDestroyInstance(d->instance_, nullptr);
  }

  if (d->device_ != VK_NULL_HANDLE) {
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

std::expected<VkInstance, VkResult> VulkanRenderer::Impl::NewVkInstance() {
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

#ifndef NDEBUG
  std::vector<const char*> available_layers = GetAvailableValidationLayers();
  create_info.enabledLayerCount = available_layers.size();
  create_info.ppEnabledLayerNames = available_layers.data();
  if (!available_layers.empty()) {
    is_validation_layers_enabled_ = true;
  } else {
    is_validation_layers_enabled_ = false;
  }
#else
  create_info.enabledLayerCount = 0;
#endif

  VkInstance instance;
  VkResult result = vkCreateInstance(&create_info, nullptr, &instance);

  if (result != VK_SUCCESS) {
    return std::unexpected(result);
  }

  return instance;
}

#ifndef NDEBUG
std::vector<const char*> VulkanRenderer::Impl::GetAvailableValidationLayers() {
  std::array<const char*, 1> validation_layers = {
      "VK_LAYER_KHRONOS_validation"};
  std::vector<const char*> enabled_layer_names;

  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> layers_available(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, layers_available.data());

  for (const char* layer_name : validation_layers) {
    bool found = false;

    for (const auto& layer_properties : layers_available) {
      if (strcmp(layer_name, layer_properties.layerName) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      std::cerr << "Layer " << layer_name << " was not found" << std::endl;
      continue;
    }

    enabled_layer_names.push_back(layer_name);
  }

  return enabled_layer_names;
}
#endif

std::expected<VkDevice, Result> VulkanRenderer::Impl::NewLogicalDevice() {
  VkDeviceQueueCreateInfo queue_create_info{};
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueFamilyIndex =
      physical_device_->GetQueueFamilies().graphics_family;
  queue_create_info.queueCount = 1;

  float queue_priority = 1.0f;
  queue_create_info.pQueuePriorities = &queue_priority;

  VkPhysicalDeviceFeatures device_features{};

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pQueueCreateInfos = &queue_create_info;
  device_create_info.queueCreateInfoCount = 1;
  device_create_info.pEnabledFeatures = &device_features;
  device_create_info.enabledExtensionCount = 0;

  VkDevice device;
  VkResult res =
      vkCreateDevice(*physical_device_, &device_create_info, nullptr, &device);
  if (res != VK_SUCCESS) {
    return std::unexpected(Result(res));
  }

  return device;
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
