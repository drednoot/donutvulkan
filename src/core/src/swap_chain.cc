#include "swap_chain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <memory>

#include "core/result.h"

#include "instance.h"
#include "logical_device.h"
#include "swap_chain_support_details.h"

namespace core {

std::expected<SwapChain*, Result> SwapChain::New(const LogicalDevice& device,
                                                 const Instance& instance,
                                                 GLFWwindow* window) {
  SwapChainSupportDetails swapchain_details = TRY(SwapChainSupportDetails::New(
      device.GetPhysicalDevice(), instance.GetSurface(), window));

  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = instance.GetSurface();
  create_info.minImageCount = swapchain_details.image_count;
  create_info.imageFormat = swapchain_details.surface_format.format;
  create_info.imageColorSpace = swapchain_details.surface_format.colorSpace;
  create_info.imageExtent = swapchain_details.extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  const QueueFamilies& queue_families =
      device.GetPhysicalDevice().GetQueueFamilies();
  const std::array<uint32_t, 2>& queue_family_indices =
      queue_families.GetIndices();
  if (queue_families.graphics != queue_families.present) {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = queue_family_indices.size();
    create_info.pQueueFamilyIndices = queue_family_indices.data();
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0;
    create_info.pQueueFamilyIndices = nullptr;
  }

  create_info.preTransform = swapchain_details.capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = swapchain_details.present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = VK_NULL_HANDLE;

  VkSwapchainKHR swap_chain;
  TRY_VK_SUCCESS(
      vkCreateSwapchainKHR(device, &create_info, nullptr, &swap_chain));

  std::unique_ptr<SwapChain> swap_chain_ptr(new SwapChain(device));
  swap_chain_ptr->swap_chain_ = swap_chain;

  return swap_chain_ptr.release();
}

SwapChain::~SwapChain() {
  if (swap_chain_) {
    vkDestroySwapchainKHR(device_, swap_chain_, nullptr);
  }
}

SwapChain::SwapChain(const LogicalDevice& device) : device_(device) {}

}  // namespace core
