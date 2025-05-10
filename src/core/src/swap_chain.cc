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
                                                 GLFWwindow* window) {
  const VkSurfaceKHR surface =
      device.GetPhysicalDevice().GetInstance().GetSurface();

  SwapChainSupportDetails swapchain_details =
      UNWRAP(SwapChainSupportDetails::New(device.GetPhysicalDevice(), surface,
                                          window));

  std::unique_ptr<SwapChain> swap_chain_ptr(new SwapChain(device));
  swap_chain_ptr->details_ = swapchain_details;

  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = surface;
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
  TRY(vkCreateSwapchainKHR(device, &create_info, nullptr, &swap_chain));
  swap_chain_ptr->swap_chain_ = swap_chain;

  TRY(vkGetSwapchainImagesKHR(device, swap_chain,
                              &swap_chain_ptr->details_.image_count, nullptr));
  swap_chain_ptr->images_.resize(swap_chain_ptr->details_.image_count);
  TRY(vkGetSwapchainImagesKHR(device, swap_chain,
                              &swap_chain_ptr->details_.image_count,
                              swap_chain_ptr->images_.data()));

  TRY(swap_chain_ptr->NewImageViews());

  return swap_chain_ptr.release();
}

SwapChain::~SwapChain() {
  for (const VkImageView image_view : image_views_) {
    vkDestroyImageView(device_, image_view, nullptr);
  }

  if (swap_chain_) {
    vkDestroySwapchainKHR(device_, swap_chain_, nullptr);
  }
}

SwapChain::operator VkSwapchainKHR() const {
  return swap_chain_;
}

const SwapChainSupportDetails& SwapChain::Details() const {
  return details_;
}

SwapChain::SwapChain(const LogicalDevice& device) : device_(device) {}

VkResult SwapChain::NewImageViews() {
  image_views_.resize(images_.size());

  for (size_t i = 0; i < images_.size(); ++i) {
    VkImageViewCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = images_[i];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = details_.surface_format.format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    TRY_ERR(
        vkCreateImageView(device_, &create_info, nullptr, &image_views_[i]));
  }

  return VK_SUCCESS;
}

}  // namespace core
