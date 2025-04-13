#ifndef DONUTCPP_CORE_QUEUE_FAMILIES_H_
#define DONUTCPP_CORE_QUEUE_FAMILIES_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <expected>

#include "core/result.h"

namespace core {

inline const VkQueueFlags kRequiredFamilies = VK_QUEUE_GRAPHICS_BIT;

struct QueueFamilies {
  uint32_t graphics_family;

  static std::expected<QueueFamilies, Result> New(
      VkPhysicalDevice physical_device);
};

}  // namespace core

#endif  // DONUTCPP_CORE_QUEUE_FAMILIES_H_
