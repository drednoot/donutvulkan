#include "physical_device.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

#include "consts.h"
#include "queue_families.h"
#include "swap_chain_support_details.h"

namespace core {

std::expected<PhysicalDevice*, Result> PhysicalDevice::New(
    VkInstance instance,
    VkSurfaceKHR surface) {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

  if (device_count < 1) {
    return std::unexpected(Result(CoreError::kNoGpuWithVulkanSupport));
  }

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

  std::unique_ptr<PhysicalDevice> device_ptr;

  for (const VkPhysicalDevice& device_handle : devices) {
    std::unique_ptr<PhysicalDevice> device(
        new PhysicalDevice(device_handle, surface));

    const auto& queue_families_exp = QueueFamilies::New(device_handle, surface);
    if (!queue_families_exp.has_value()) {
      if (queue_families_exp.error().kind == kCoreError &&
          queue_families_exp.error().error.core ==
              kNotAllRequiredQueueFamiliesArePresent) {
        continue;
      } else {
        return std::unexpected(Result(queue_families_exp.error()));
      }
    }
    device->queue_families_ = *queue_families_exp;

    if (device->IsSuitable()) {
      device_ptr.reset(device.release());
      break;
    }
  }

  if (device_ptr == nullptr) {
    return std::unexpected(Result(CoreError::kNoSuitableGpu));
  }

  return device_ptr.release();
}

const QueueFamilies& PhysicalDevice::GetQueueFamilies() const {
  return queue_families_;
}

PhysicalDevice::operator const VkPhysicalDevice&() const {
  return physical_device_;
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface)
    : physical_device_(device), surface_(surface) {}

bool PhysicalDevice::IsSuitable() {
  uint32_t extension_count;
  VkResult res = vkEnumerateDeviceExtensionProperties(
      physical_device_, nullptr, &extension_count, nullptr);
  if (res != VK_SUCCESS) {
    return false;
  }

  std::vector<VkExtensionProperties> extension_properties(extension_count);
  vkEnumerateDeviceExtensionProperties(
      physical_device_, nullptr, &extension_count, extension_properties.data());

  for (const char* required_extension : consts::kDeviceExtensions) {
    bool found = false;

    for (const VkExtensionProperties& available_extension :
         extension_properties) {
      if (strcmp(required_extension, available_extension.extensionName) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      return false;
    }
  }

  return true;
}

}  // namespace core
