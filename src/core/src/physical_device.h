#ifndef DONUTCPP_CORE_PHYSICAL_DEVICE_H_
#define DONUTCPP_CORE_PHYSICAL_DEVICE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <expected>

#include "core/result.h"

#include "queue_families.h"

namespace core {

class PhysicalDevice {
 public:
  static std::expected<PhysicalDevice*, Result> New(VkInstance instance,
                                                    VkSurfaceKHR surface);

  const QueueFamilies& GetQueueFamilies() const;
  operator const VkPhysicalDevice&() const;

 private:
  PhysicalDevice(VkPhysicalDevice device);

  bool IsSuitable();

  VkPhysicalDevice device_;
  QueueFamilies queue_families_;
};

}  // namespace core

namespace consts {

const inline std::array<const char*, 1> kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

}  // namespace consts

#endif  // DONUTCPP_CORE_PHYSICAL_DEVICE_H_
