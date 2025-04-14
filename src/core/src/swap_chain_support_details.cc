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
  std::vector<VkSurfaceFormatKHR> formats(format_count);
  vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count,
                                       formats.data());
  details.surface_format = GetSwapSurfaceFormat(formats);

  uint32_t present_mode_count;
  TRY_VK_SUCCESS(vkGetPhysicalDeviceSurfacePresentModesKHR(
      physical_device, surface, &present_mode_count, nullptr));
  if (present_mode_count < 1) {
    return std::unexpected(Result(CoreError::kNoAvailableSurfacePresentModes));
  }
  std::vector<VkPresentModeKHR> present_modes(format_count);
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      physical_device, surface, &present_mode_count, present_modes.data());
  details.present_mode = GetPresentMode(present_modes);

  return details;
}

VkSurfaceFormatKHR SwapChainSupportDetails::GetSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& available_formats) {
  for (const auto& available_format : available_formats) {
    if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return available_format;
    }
  }

  return available_formats[0];
}

VkPresentModeKHR SwapChainSupportDetails::GetPresentMode(
    const std::vector<VkPresentModeKHR>& available_present_modes) {
  for (const auto& available_present_mode : available_present_modes) {
    if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return available_present_mode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

}  // namespace core
