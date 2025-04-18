#include "vulkan_renderer_impl.h"
#include "core/vulkan_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <array>
#include <cmath>
#include <cstring>
#include <expected>
#include <iostream>
#include <memory>
#include <set>
#include <thread>

#include "consts.h"
#include "physical_device.h"
#include "swap_chain_support_details.h"

namespace core {

Result VulkanRenderer::Impl::New(const VulkanRendererConfig& config) {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(config.width, config.height,
                                        "Vulkan window", nullptr, nullptr);

  if (!window) {
    return kCouldNotInitializeGlfwWindow;
  }
  window_ = window;

  instance_.reset(TRY_RESULT(Instance::New(window_)));
  physical_device_.reset(
      TRY_RESULT(PhysicalDevice::New(*instance_, instance_->GetSurface())));
  device_ = TRY_RESULT(NewLogicalDevice());
  vkGetDeviceQueue(device_, physical_device_->GetQueueFamilies().graphics, 0,
                   &graphics_queue_);
  vkGetDeviceQueue(device_, physical_device_->GetQueueFamilies().present, 0,
                   &present_queue_);

  swap_chain_ = TRY_RESULT(NewSwapChain());

  cfg_ = config;
  buffer_.resize(config.width * config.height);
  z_buffer_.resize(config.width * config.height);
  screen_ratio_ = config.width / (double)config.height;
  target_ns_ =
      std::chrono::nanoseconds(std::chrono::seconds(1)) / config.target_fps;

  return Result();
}

VulkanRenderer::Impl::~Impl() {
  if (window_) {
    glfwDestroyWindow(window_);
  }

  if (swap_chain_) {
    vkDestroySwapchainKHR(device_, swap_chain_, nullptr);
  }

  if (device_) {
    vkDestroyDevice(device_, nullptr);
  }

  glfwTerminate();
}

std::expected<VkDevice, VkResult> VulkanRenderer::Impl::NewLogicalDevice()
    const {
  VkPhysicalDeviceFeatures device_features{};

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  std::set<uint32_t> unique_queue_families = {
      physical_device_->GetQueueFamilies().graphics,
      physical_device_->GetQueueFamilies().present};
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  queue_create_infos.reserve(unique_queue_families.size());

  for (uint32_t queue_family_index : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family_index;
    queue_create_info.queueCount = 1;

    const float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(std::move(queue_create_info));
  }

  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  device_create_info.queueCreateInfoCount = queue_create_infos.size();

  device_create_info.pEnabledFeatures = &device_features;
  device_create_info.enabledExtensionCount = consts::kDeviceExtensions.size();
  device_create_info.ppEnabledExtensionNames = consts::kDeviceExtensions.data();

  VkDevice device;
  TRY_VK_SUCCESS(
      vkCreateDevice(*physical_device_, &device_create_info, nullptr, &device));

  return device;
}

std::expected<VkSwapchainKHR, Result> VulkanRenderer::Impl::NewSwapChain()
    const {
  SwapChainSupportDetails swapchain_details = TRY(SwapChainSupportDetails::New(
      *physical_device_, instance_->GetSurface(), window_));

  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = instance_->GetSurface();
  create_info.minImageCount = swapchain_details.image_count;
  create_info.imageFormat = swapchain_details.surface_format.format;
  create_info.imageColorSpace = swapchain_details.surface_format.colorSpace;
  create_info.imageExtent = swapchain_details.extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  const QueueFamilies& queue_families = physical_device_->GetQueueFamilies();
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
      vkCreateSwapchainKHR(device_, &create_info, nullptr, &swap_chain));

  return swap_chain;
}

void VulkanRenderer::Impl::Start() {
  std::chrono::nanoseconds frame_time = target_ns_;
  // double delta = NsToSeconds(target_ns_);
  start_time_ = std::chrono::system_clock::now().time_since_epoch();

  while (!glfwWindowShouldClose(window_)) {
    std::chrono::time_point before_render =
        std::chrono::high_resolution_clock::now();
    // Clear();
    // Render(delta);
    glfwPollEvents();
    // DrawBuffer();
    std::chrono::time_point after_render =
        std::chrono::high_resolution_clock::now();
    frame_time = after_render - before_render;
    // delta = frame_time > target_ns_ ? NsToSeconds(frame_time)
    //                                 : NsToSeconds(target_ns_);

    if (frame_time < target_ns_) {
      std::this_thread::sleep_for(target_ns_ - frame_time);
    }
  }
}

void VulkanRenderer::Impl::DrawBuffer() const {
  MoveCursorTo(0, 0);
  std::cout.write(buffer_.data(), buffer_.size());
  std::cout.flush();
}

void VulkanRenderer::Impl::MoveCursorTo(int x, int y) const {
  printf("\033[%d;%dH", y, x);
}

}  // namespace core
