#include "core/result.h"

#include <vulkan/vk_enum_string_helper.h>

namespace core {

const char* CoreErrorToString(CoreError e) {
  switch (e) {
    case kCouldNotInitializeGlfwWindow:
      return "Could not initialize GLFW Window";
  }

  return "Unknown error code";
}

const char* ResultToString(Result r) {
  switch (r.kind) {
    case kNoError:
      return "No error";
    case kCoreError:
      return CoreErrorToString(r.error.core);
    case kVkError:
      return string_VkResult(r.error.vk);
  }
}

}  // namespace core
