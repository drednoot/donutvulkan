#include "queue_families.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <vector>

#include "core/result.h"

namespace core {

std::expected<QueueFamilies, Result> QueueFamilies::New(
    VkPhysicalDevice physical_device,
    VkSurfaceKHR surface) {
  QueueFamilies families;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                           nullptr);
  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                           queue_families.data());

  uint32_t required_families = kRequiredFamilies;
  for (uint32_t i = 0; i < queue_families.size(); ++i) {
    const VkQueueFamilyProperties& queue_family = queue_families[i];
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      families.graphics = i;
      required_families &= ~kGraphicsFamily;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface,
                                         &present_support);
    if (present_support) {
      families.present = i;
      required_families &= ~kPresentSupportFamily;
    }

    if (required_families == 0) {
      break;
    }
  }

  if (required_families != 0) {
    return std::unexpected(Result(kNotAllRequiredQueueFamiliesArePresent));
  }

  return families;
}

}  // namespace core
