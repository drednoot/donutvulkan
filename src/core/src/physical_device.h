#ifndef DONUTCPP_CORE_PHYSICAL_DEVICE_H_
#define DONUTCPP_CORE_PHYSICAL_DEVICE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>

#include "core/result.h"

#include "instance.h"
#include "queue_families.h"

namespace core {

class PhysicalDevice {
 public:
  static std::expected<PhysicalDevice*, Result> New(const Instance& instance);

  const QueueFamilies& GetQueueFamilies() const;
  const Instance& GetInstance() const;
  operator VkPhysicalDevice() const;

 private:
  PhysicalDevice(VkPhysicalDevice physical_device, const Instance& instace);

  bool IsSuitable();

  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  QueueFamilies queue_families_;
  const Instance& instance_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_PHYSICAL_DEVICE_H_
