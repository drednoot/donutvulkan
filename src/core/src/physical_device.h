#ifndef DONUTCPP_CORE_PHYSICAL_DEVICE_H_
#define DONUTCPP_CORE_PHYSICAL_DEVICE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>

#include "core/result.h"

#include "queue_families.h"
#include "swap_chain_support_details.h"

namespace core {

class PhysicalDevice {
 public:
  static std::expected<PhysicalDevice*, Result> New(VkInstance instance,
                                                    VkSurfaceKHR surface);

  const QueueFamilies& GetQueueFamilies() const;
  operator VkPhysicalDevice() const;

 private:
  PhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface);

  bool IsSuitable();

  VkPhysicalDevice physical_device_;
  VkSurfaceKHR surface_;
  QueueFamilies queue_families_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_PHYSICAL_DEVICE_H_
