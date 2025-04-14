#ifndef DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_
#define DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <vector>

#include "core/result.h"

namespace core {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilites;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> present_modes;

  static std::expected<SwapChainSupportDetails, Result> New(
      VkPhysicalDevice physical_device,
      VkSurfaceKHR surface);
};

}  // namespace core

#endif  // DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_
