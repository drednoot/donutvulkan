#include "vulkan_renderer_impl.h"
#include "core/vulkan_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <array>
#include <cmath>
#include <cstring>
#include <expected>
#include <iostream>
#include <memory>
#include <set>

#include "consts.h"
#include "physical_device.h"

namespace core {

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
  const std::vector<const char*>& available_layers =
      TRY(GetAvailableValidationLayers());
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
  TRY_VK_SUCCESS(vkCreateInstance(&create_info, nullptr, &instance));

  return instance;
}

#ifndef NDEBUG
std::expected<std::vector<const char*>, VkResult>
VulkanRenderer::Impl::GetAvailableValidationLayers() {
  std::array<const char*, 1> validation_layers = {
      "VK_LAYER_KHRONOS_validation"};
  std::vector<const char*> enabled_layer_names;

  uint32_t layer_count;
  TRY_VK_SUCCESS(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
  std::vector<VkLayerProperties> layers_available(layer_count);
  TRY_VK_SUCCESS(vkEnumerateInstanceLayerProperties(&layer_count,
                                                    layers_available.data()));

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

std::expected<VkSurfaceKHR, VkResult> VulkanRenderer::Impl::NewSurface() {
  VkSurfaceKHR surface;
  TRY_VK_SUCCESS(
      glfwCreateWindowSurface(instance_, window_, nullptr, &surface));

  return surface;
}

std::expected<VkDevice, VkResult> VulkanRenderer::Impl::NewLogicalDevice() {
  VkPhysicalDeviceFeatures device_features{};

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  std::set<uint32_t> unique_queue_families = {
      physical_device_->GetQueueFamilies().graphics,
      physical_device_->GetQueueFamilies().present};
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  queue_create_infos.reserve(unique_queue_families.size());

  for (uint32_t queue_family_index : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family_index;
    queue_create_info.queueCount = 1;

    const float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(std::move(queue_create_info));
  }

  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  device_create_info.queueCreateInfoCount = queue_create_infos.size();

  device_create_info.pEnabledFeatures = &device_features;
  device_create_info.enabledExtensionCount = consts::kDeviceExtensions.size();
  device_create_info.ppEnabledExtensionNames = consts::kDeviceExtensions.data();

  VkDevice device;
  TRY_VK_SUCCESS(
      vkCreateDevice(*physical_device_, &device_create_info, nullptr, &device));

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
