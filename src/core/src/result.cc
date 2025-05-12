#include "core/result.h"

#include <vulkan/vk_enum_string_helper.h>

#include <string>

namespace core {

const std::string CoreErrorToString(CoreError e) {
  switch (e) {
    case kCouldNotInitializeGlfwWindow:
      return "Could not initialize GLFW Window";
    case kNoGpuWithVulkanSupport:
    case kNoSuitableGpu:
      return "Could not find GPU with proper vulkan support";
    case kNotAllRequiredQueueFamiliesArePresent:
      return "Not all required Queue Families are Present in Physical Device";
    case kNoAvailableSurfaceFormats:
      return "Physical Device doesn't have any available surface formats";
    case kNoAvailableSurfacePresentModes:
      return "Physical Device doesn't have any available surface present modes";
    default:
      return "Unknown error code";
  }
}

const std::string ResultToString(Result r) {
  switch (r.kind) {
    case kNoError:
      return "No error";
    case kCoreError:
      return CoreErrorToString(r.error.core);
    case kVkError:
      return string_VkResult(r.error.vk);
    case kFileError:
      return std::string("Could not open file: ") + r.error.file.filename;
    default:
      return "Unknown Result type";
  }
}

}  // namespace core
