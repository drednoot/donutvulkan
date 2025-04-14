#include "swap_chain_support_details.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>

#include "core/result.h"

namespace core {

std::expected<SwapChainSupportDetails, Result> SwapChainSupportDetails::New(
    VkPhysicalDevice physical_device,
    VkSurfaceKHR surface) {
  SwapChainSupportDetails details{};

  TRY_VK_SUCCESS(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      physical_device, surface, &details.capabilites));

  uint32_t format_count;
  TRY_VK_SUCCESS(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface,
                                                      &format_count, nullptr));
  if (format_count < 1) {
    return std::unexpected(Result(CoreError::kNoAvailableSurfaceFormats));
  }
  details.formats.resize(format_count);
  vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count,
                                       details.formats.data());

  uint32_t present_mode_count;
  TRY_VK_SUCCESS(vkGetPhysicalDeviceSurfacePresentModesKHR(
      physical_device, surface, &present_mode_count, nullptr));
  if (present_mode_count < 1) {
    return std::unexpected(Result(CoreError::kNoAvailableSurfacePresentModes));
  }
  details.present_modes.resize(format_count);
  vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface,
                                            &present_mode_count,
                                            details.present_modes.data());

  return details;
}

}  // namespace core
