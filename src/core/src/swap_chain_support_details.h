#ifndef DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_
#define DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <expected>
#include <vector>

#include "core/result.h"

namespace core {

struct SwapChainSupportDetails {
 public:
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR surface_format;
  VkPresentModeKHR present_mode;
  VkExtent2D extent;
  uint32_t image_count;

  static std::expected<SwapChainSupportDetails, Result> New(
      VkPhysicalDevice physical_device,
      VkSurfaceKHR surface,
      GLFWwindow* window);

 private:
  static VkSurfaceFormatKHR GetSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& available_formats);

  static VkPresentModeKHR GetPresentMode(
      const std::vector<VkPresentModeKHR>& available_present_modes);

  VkExtent2D GetSwapExtent(GLFWwindow* window);
};

}  // namespace core

#endif  // DONUTCPP_CORE_SWAP_CHAIN_SUPPORT_DETAILS_H_
