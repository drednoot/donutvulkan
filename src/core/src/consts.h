#ifndef DONUTCPP_CORE_CONSTS_H_
#define DONUTCPP_CORE_CONSTS_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

#include "queue_families.h"

namespace core {
namespace consts {

const inline std::array<const char*, 1> kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

const inline uint32_t kRequiredFamilies =
    QueueFamilies::kGraphicsFamily & QueueFamilies::kPresentSupportFamily;

}  // namespace consts
}  // namespace core

#endif  // DONUTCPP_CORE_CONSTS_H_
