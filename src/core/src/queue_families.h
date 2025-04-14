#ifndef DONUTCPP_CORE_QUEUE_FAMILIES_H_
#define DONUTCPP_CORE_QUEUE_FAMILIES_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <expected>

#include "core/result.h"

namespace core {

struct QueueFamilies {
  enum SupportedFamilies {
    kGraphicsFamily = 0x1,
    kPresentSupportFamily = 0x2,
  };

  uint32_t graphics;
  uint32_t present;

  static std::expected<QueueFamilies, Result> New(
      VkPhysicalDevice physical_device,
      VkSurfaceKHR surface);
};

}  // namespace core

#endif  // DONUTCPP_CORE_QUEUE_FAMILIES_H_
