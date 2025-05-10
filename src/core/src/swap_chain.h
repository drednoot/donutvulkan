#ifndef DONUTCPP_CORE_SWAP_CHAIN_H_
#define DONUTCPP_CORE_SWAP_CHAIN_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <vector>

#include "core/result.h"

#include "logical_device.h"
#include "swap_chain_support_details.h"

namespace core {

class SwapChain {
 public:
  static std::expected<SwapChain*, Result> New(
      const LogicalDevice& logical_device,
      GLFWwindow* window);
  ~SwapChain();

  operator VkSwapchainKHR() const;
  const SwapChainSupportDetails& Details() const;

 private:
  SwapChain(const LogicalDevice& device);

  VkSwapchainKHR swap_chain_ = VK_NULL_HANDLE;
  SwapChainSupportDetails details_;
  std::vector<VkImage> images_;
  const LogicalDevice& device_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_SWAP_CHAIN_H_
