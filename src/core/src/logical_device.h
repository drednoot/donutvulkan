#ifndef DONUTCPP_CORE_LOGICAL_DEVICE_H_
#define DONUTCPP_CORE_LOGICAL_DEVICE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>

#include "physical_device.h"

namespace core {

class LogicalDevice {
 public:
  static std::expected<LogicalDevice*, VkResult> New(
      const PhysicalDevice& physical_device);
  ~LogicalDevice();

  operator VkDevice() const;

 private:
  LogicalDevice() = default;

  VkDevice device_;
  VkQueue graphics_queue_;
  VkQueue present_queue_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_LOGICAL_DEVICE_H_
