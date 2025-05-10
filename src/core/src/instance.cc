#include "instance.h"

#include <cstring>
#include <expected>
#include <iostream>
#include <memory>
#include <vector>

#include "core/result.h"

namespace core {

std::expected<Instance*, Result> Instance::New(GLFWwindow* window) {
  std::unique_ptr<Instance> instance(new Instance());

  instance->instance_ = UNWRAP(instance->NewVkInstance());
  instance->surface_ = UNWRAP(instance->NewSurface(window));

  return instance.release();
}

std::expected<VkInstance, VkResult> Instance::NewVkInstance() {
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
      UNWRAP(GetAvailableValidationLayers());
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
  TRY(vkCreateInstance(&create_info, nullptr, &instance));

  return instance;
}

std::expected<VkSurfaceKHR, VkResult> Instance::NewSurface(
    GLFWwindow* window) const {
  VkSurfaceKHR surface;
  TRY(glfwCreateWindowSurface(instance_, window, nullptr, &surface));

  return surface;
}

Instance::~Instance() {
  if (surface_ && instance_) {
    vkDestroySurfaceKHR(instance_, surface_, nullptr);
  }

  if (instance_) {
    vkDestroyInstance(instance_, nullptr);
  }
}

#ifndef NDEBUG
std::expected<std::vector<const char*>, VkResult>
Instance::GetAvailableValidationLayers() {
  std::array<const char*, 1> validation_layers = {
      "VK_LAYER_KHRONOS_validation"};
  std::vector<const char*> enabled_layer_names;

  uint32_t layer_count;
  TRY(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
  std::vector<VkLayerProperties> layers_available(layer_count);
  TRY(vkEnumerateInstanceLayerProperties(&layer_count,
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

}  // namespace core
