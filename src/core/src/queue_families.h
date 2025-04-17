#ifndef DONUTCPP_CORE_QUEUE_FAMILIES_H_
#define DONUTCPP_CORE_QUEUE_FAMILIES_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
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

  inline const std::array<uint32_t, 2> GetIndices() const {
    return {graphics, present};
  }
};

}  // namespace core

#endif  // DONUTCPP_CORE_QUEUE_FAMILIES_H_
