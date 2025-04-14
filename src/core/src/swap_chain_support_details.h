#ifndef DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_
#define DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <vector>

#include "core/result.h"

namespace core {

struct SwapChainSupportDetails {
 public:
  VkSurfaceCapabilitiesKHR capabilites;
  VkSurfaceFormatKHR surface_format;
  VkPresentModeKHR present_mode;

  static std::expected<SwapChainSupportDetails, Result> New(
      VkPhysicalDevice physical_device,
      VkSurfaceKHR surface);

 private:
  static VkSurfaceFormatKHR GetSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& available_formats);

  static VkPresentModeKHR GetPresentMode(
      const std::vector<VkPresentModeKHR>& available_present_modes);
};

}  // namespace core

#endif  // DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_
