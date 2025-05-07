#include "logical_device.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <memory>
#include <set>

#include "core/result.h"

#include "consts.h"

namespace core {

std::expected<LogicalDevice*, VkResult> LogicalDevice::New(
    const PhysicalDevice& physical_device) {
  VkPhysicalDeviceFeatures device_features{};

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  std::set<uint32_t> unique_queue_families = {
      physical_device.GetQueueFamilies().graphics,
      physical_device.GetQueueFamilies().present};
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
      vkCreateDevice(physical_device, &device_create_info, nullptr, &device));

  std::unique_ptr<LogicalDevice> device_ptr(new LogicalDevice(physical_device));
  device_ptr->device_ = device;

  vkGetDeviceQueue(device, physical_device.GetQueueFamilies().graphics, 0,
                   &device_ptr->graphics_queue_);
  vkGetDeviceQueue(device, physical_device.GetQueueFamilies().present, 0,
                   &device_ptr->present_queue_);

  return device_ptr.release();
}

LogicalDevice::~LogicalDevice() {
  if (device_) {
    vkDestroyDevice(device_, nullptr);
  }
}

LogicalDevice::operator VkDevice() const {
  return device_;
}

const PhysicalDevice& LogicalDevice::GetPhysicalDevice() const {
  return physical_device_;
}

LogicalDevice::LogicalDevice(const PhysicalDevice& physical_device)
    : physical_device_(physical_device) {}

}  // namespace core
